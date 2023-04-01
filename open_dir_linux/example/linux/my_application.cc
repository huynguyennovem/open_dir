#include "my_application.h"

#include <flutter_linux/flutter_linux.h>

#ifdef GDK_WINDOWING_X11
#include <gdk/gdkx.h>
#endif

#include "flutter/generated_plugin_registrant.h"

// open_dir implementation imports
#include <cstdlib>
#include <stdexcept>
#include <string>
// end

struct _MyApplication
{
  GtkApplication parent_instance;
  char **dart_entrypoint_arguments;
  FlMethodChannel *open_dir_channel;
};

G_DEFINE_TYPE(MyApplication, my_application, GTK_TYPE_APPLICATION)

// open_dir implementation methods
static bool open_directory(const gchar *path)
{
  // to prevent showing man of xdg-open command
  if (std::string(path) == "")
  {
    return false;
  }
  std::string command = "xdg-open \"" + std::string(path) + "\"";
  int result = std::system(command.c_str());
  if (result == -1)
  {
    throw std::runtime_error("Runtime error: Failed to open this path!");
  }
  else if (result != 0)
  {
    return false;
  }
  return true;
}

static gchar *open_dir_get_path_arg(FlMethodCall *method_call, GError **error)
{
  FlValue *args = fl_method_call_get_args(method_call);
  if (fl_value_get_type(args) != FL_VALUE_TYPE_MAP)
  {
    g_set_error(error, 0, 0, "Argument is not correct");
    return nullptr;
  }
  FlValue *path_value = fl_value_lookup_string(args, "path");
  if (path_value == nullptr)
  {
    g_set_error(error, 0, 0, "Path value is missing");
    return nullptr;
  }
  return g_strdup(fl_value_get_string(path_value));
}

static void open_dir_method_call_handler(FlMethodChannel *channel,
                                         FlMethodCall *method_call,
                                         gpointer user_data)
{
  g_autoptr(FlMethodResponse) response = nullptr;
  const gchar *method = fl_method_call_get_name(method_call);

  if (strcmp(method, "openNativeDir") == 0)
  {
    g_autoptr(GError) error = nullptr;
    g_autofree gchar *path = open_dir_get_path_arg(method_call, &error);
    if (path == nullptr || (path != nullptr && strlen(path) == 0))
    {
      response = FL_METHOD_RESPONSE(fl_method_error_response_new("Native error", "Path is null or empty!", nullptr));
    }
    else
    {
      int rs = open_directory(path);
      if (rs)
      {
        response = FL_METHOD_RESPONSE(fl_method_success_response_new(fl_value_new_bool(rs)));
      }
      else
      {
        response = FL_METHOD_RESPONSE(fl_method_error_response_new("Native error", "Can not open this directory!", nullptr));
      }
    }
  }
  else
  {
    response = FL_METHOD_RESPONSE(fl_method_not_implemented_response_new());
  }

  g_autoptr(GError) error = nullptr;
  if (!fl_method_call_respond(method_call, response, &error))
  {
    g_warning("Failed to send response: %s", error->message);
  }
}
// end

// Implements GApplication::activate.
static void my_application_activate(GApplication *application)
{
  MyApplication *self = MY_APPLICATION(application);
  GtkWindow *window =
      GTK_WINDOW(gtk_application_window_new(GTK_APPLICATION(application)));

  // Use a header bar when running in GNOME as this is the common style used
  // by applications and is the setup most users will be using (e.g. Ubuntu
  // desktop).
  // If running on X and not using GNOME then just use a traditional title bar
  // in case the window manager does more exotic layout, e.g. tiling.
  // If running on Wayland assume the header bar will work (may need changing
  // if future cases occur).
  gboolean use_header_bar = TRUE;
#ifdef GDK_WINDOWING_X11
  GdkScreen *screen = gtk_window_get_screen(window);
  if (GDK_IS_X11_SCREEN(screen))
  {
    const gchar *wm_name = gdk_x11_screen_get_window_manager_name(screen);
    if (g_strcmp0(wm_name, "GNOME Shell") != 0)
    {
      use_header_bar = FALSE;
    }
  }
#endif
  if (use_header_bar)
  {
    GtkHeaderBar *header_bar = GTK_HEADER_BAR(gtk_header_bar_new());
    gtk_widget_show(GTK_WIDGET(header_bar));
    gtk_header_bar_set_title(header_bar, "open_dir_linux_example");
    gtk_header_bar_set_show_close_button(header_bar, TRUE);
    gtk_window_set_titlebar(window, GTK_WIDGET(header_bar));
  }
  else
  {
    gtk_window_set_title(window, "open_dir_linux_example");
  }

  gtk_window_set_default_size(window, 1280, 720);
  gtk_widget_show(GTK_WIDGET(window));

  g_autoptr(FlDartProject) project = fl_dart_project_new();
  fl_dart_project_set_dart_entrypoint_arguments(project, self->dart_entrypoint_arguments);

  FlView *view = fl_view_new(project);
  gtk_widget_show(GTK_WIDGET(view));
  gtk_container_add(GTK_CONTAINER(window), GTK_WIDGET(view));

  fl_register_plugins(FL_PLUGIN_REGISTRY(view));

  // open_dir implementation method channel test in example
  g_autoptr(FlStandardMethodCodec) codec = fl_standard_method_codec_new();
  self->open_dir_channel = fl_method_channel_new(
      fl_engine_get_binary_messenger(fl_view_get_engine(view)),
      "com.flutter/open-dir-linux", FL_METHOD_CODEC(codec));
  fl_method_channel_set_method_call_handler(
      self->open_dir_channel, open_dir_method_call_handler, self, nullptr);
  // end

  gtk_widget_grab_focus(GTK_WIDGET(view));
}

// Implements GApplication::local_command_line.
static gboolean my_application_local_command_line(GApplication *application, gchar ***arguments, int *exit_status)
{
  MyApplication *self = MY_APPLICATION(application);
  // Strip out the first argument as it is the binary name.
  self->dart_entrypoint_arguments = g_strdupv(*arguments + 1);

  g_autoptr(GError) error = nullptr;
  if (!g_application_register(application, nullptr, &error))
  {
    g_warning("Failed to register: %s", error->message);
    *exit_status = 1;
    return TRUE;
  }

  g_application_activate(application);
  *exit_status = 0;

  return TRUE;
}

// Implements GObject::dispose.
static void my_application_dispose(GObject *object)
{
  MyApplication *self = MY_APPLICATION(object);
  g_clear_pointer(&self->dart_entrypoint_arguments, g_strfreev);

  // open_dir method channel test in example
  g_clear_object(&self->open_dir_channel);
  // end

  G_OBJECT_CLASS(my_application_parent_class)->dispose(object);
}

static void my_application_class_init(MyApplicationClass *klass)
{
  G_APPLICATION_CLASS(klass)->activate = my_application_activate;
  G_APPLICATION_CLASS(klass)->local_command_line = my_application_local_command_line;
  G_OBJECT_CLASS(klass)->dispose = my_application_dispose;
}

static void my_application_init(MyApplication *self) {}

MyApplication *my_application_new()
{
  return MY_APPLICATION(g_object_new(my_application_get_type(),
                                     "application-id", APPLICATION_ID,
                                     "flags", G_APPLICATION_NON_UNIQUE,
                                     nullptr));
}

#include "include/open_dir_linux/open_dir_linux_plugin.h"

#include <flutter_linux/flutter_linux.h>
#include <gtk/gtk.h>
#include <sys/utsname.h>

#include <cstring>

// open_dir implementation imports
#include <cstdlib>
#include <stdexcept>
#include <string>
// end

#define OPEN_DIR_LINUX_PLUGIN(obj)                                     \
  (G_TYPE_CHECK_INSTANCE_CAST((obj), open_dir_linux_plugin_get_type(), \
                              OpenDirLinuxPlugin))

struct _OpenDirLinuxPlugin
{
  GObject parent_instance;
};

G_DEFINE_TYPE(OpenDirLinuxPlugin, open_dir_linux_plugin, g_object_get_type())

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

// Called when a method call is received from Flutter.
static void open_dir_linux_plugin_handle_method_call(
    OpenDirLinuxPlugin *self,
    FlMethodCall *method_call)
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

static void open_dir_linux_plugin_dispose(GObject *object)
{
  G_OBJECT_CLASS(open_dir_linux_plugin_parent_class)->dispose(object);
}

static void open_dir_linux_plugin_class_init(OpenDirLinuxPluginClass *klass)
{
  G_OBJECT_CLASS(klass)->dispose = open_dir_linux_plugin_dispose;
}

static void open_dir_linux_plugin_init(OpenDirLinuxPlugin *self) {}

static void method_call_cb(FlMethodChannel *channel, FlMethodCall *method_call,
                           gpointer user_data)
{
  OpenDirLinuxPlugin *plugin = OPEN_DIR_LINUX_PLUGIN(user_data);
  open_dir_linux_plugin_handle_method_call(plugin, method_call);
}

void open_dir_linux_plugin_register_with_registrar(FlPluginRegistrar *registrar)
{
  OpenDirLinuxPlugin *plugin = OPEN_DIR_LINUX_PLUGIN(
      g_object_new(open_dir_linux_plugin_get_type(), nullptr));

  g_autoptr(FlStandardMethodCodec) codec = fl_standard_method_codec_new();
  g_autoptr(FlMethodChannel) channel =
      fl_method_channel_new(fl_plugin_registrar_get_messenger(registrar),
                            "com.flutter/open-dir-linux",
                            FL_METHOD_CODEC(codec));
  fl_method_channel_set_method_call_handler(channel, method_call_cb,
                                            g_object_ref(plugin),
                                            g_object_unref);

  g_object_unref(plugin);
}

//
//  Generated file. Do not edit.
//

// clang-format off

#include "generated_plugin_registrant.h"

#include <open_dir/open_dir_plugin.h>

void fl_register_plugins(FlPluginRegistry* registry) {
  g_autoptr(FlPluginRegistrar) open_dir_registrar =
      fl_plugin_registry_get_registrar_for_plugin(registry, "OpenDirPlugin");
  open_dir_plugin_register_with_registrar(open_dir_registrar);
}

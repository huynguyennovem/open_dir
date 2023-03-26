#include "include/open_dir/open_dir_plugin_c_api.h"

#include <flutter/plugin_registrar_windows.h>

#include "open_dir_plugin.h"

void OpenDirPluginCApiRegisterWithRegistrar(
    FlutterDesktopPluginRegistrarRef registrar) {
  open_dir::OpenDirPlugin::RegisterWithRegistrar(
      flutter::PluginRegistrarManager::GetInstance()
          ->GetRegistrar<flutter::PluginRegistrarWindows>(registrar));
}

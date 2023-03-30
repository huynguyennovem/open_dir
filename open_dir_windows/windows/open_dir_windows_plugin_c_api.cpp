#include "include/open_dir_windows/open_dir_windows_plugin_c_api.h"

#include <flutter/plugin_registrar_windows.h>

#include "open_dir_windows_plugin.h"

void OpenDirWindowsPluginCApiRegisterWithRegistrar(
    FlutterDesktopPluginRegistrarRef registrar) {
  open_dir_windows::OpenDirWindowsPlugin::RegisterWithRegistrar(
      flutter::PluginRegistrarManager::GetInstance()
          ->GetRegistrar<flutter::PluginRegistrarWindows>(registrar));
}

#ifndef FLUTTER_PLUGIN_OPEN_DIR_WINDOWS_PLUGIN_H_
#define FLUTTER_PLUGIN_OPEN_DIR_WINDOWS_PLUGIN_H_

#include <flutter/method_channel.h>
#include <flutter/plugin_registrar_windows.h>

#include <memory>

namespace open_dir_windows {

class OpenDirWindowsPlugin : public flutter::Plugin {
 public:
  static void RegisterWithRegistrar(flutter::PluginRegistrarWindows *registrar);

  OpenDirWindowsPlugin();

  virtual ~OpenDirWindowsPlugin();

  // Disallow copy and assign.
  OpenDirWindowsPlugin(const OpenDirWindowsPlugin&) = delete;
  OpenDirWindowsPlugin& operator=(const OpenDirWindowsPlugin&) = delete;

 private:
  // Called when a method is called on this plugin's channel from Dart.
  void HandleMethodCall(
      const flutter::MethodCall<flutter::EncodableValue> &method_call,
      std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
};

}  // namespace open_dir_windows

#endif  // FLUTTER_PLUGIN_OPEN_DIR_WINDOWS_PLUGIN_H_

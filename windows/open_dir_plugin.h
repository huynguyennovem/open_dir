#ifndef FLUTTER_PLUGIN_OPEN_DIR_PLUGIN_H_
#define FLUTTER_PLUGIN_OPEN_DIR_PLUGIN_H_

#include <flutter/method_channel.h>
#include <flutter/plugin_registrar_windows.h>

#include <memory>

namespace open_dir {

class OpenDirPlugin : public flutter::Plugin {
 public:
  static void RegisterWithRegistrar(flutter::PluginRegistrarWindows *registrar);

  OpenDirPlugin();

  virtual ~OpenDirPlugin();

  // Disallow copy and assign.
  OpenDirPlugin(const OpenDirPlugin&) = delete;
  OpenDirPlugin& operator=(const OpenDirPlugin&) = delete;

 private:
  // Called when a method is called on this plugin's channel from Dart.
  void HandleMethodCall(
      const flutter::MethodCall<flutter::EncodableValue> &method_call,
      std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
};

}  // namespace open_dir

#endif  // FLUTTER_PLUGIN_OPEN_DIR_PLUGIN_H_

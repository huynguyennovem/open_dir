#include "open_dir_windows_plugin.h"

// This must be included before many other Windows headers.
#include <windows.h>

// For getPlatformVersion; remove unless needed for your plugin implementation.
#include <VersionHelpers.h>

#include <flutter/method_channel.h>
#include <flutter/plugin_registrar_windows.h>
#include <flutter/standard_method_codec.h>

#include <memory>
#include <sstream>

using namespace flutter;
using namespace std;

namespace open_dir_windows {

// static
void OpenDirWindowsPlugin::RegisterWithRegistrar(
    flutter::PluginRegistrarWindows *registrar) {
  auto channel =
      std::make_unique<flutter::MethodChannel<flutter::EncodableValue>>(
          registrar->messenger(), "com.flutter/open-dir-windows",
          &flutter::StandardMethodCodec::GetInstance());

  auto plugin = std::make_unique<OpenDirWindowsPlugin>();

  channel->SetMethodCallHandler(
      [plugin_pointer = plugin.get()](const auto &call, auto result) {
        plugin_pointer->HandleMethodCall(call, std::move(result));
      });

  registrar->AddPlugin(std::move(plugin));
}

OpenDirWindowsPlugin::OpenDirWindowsPlugin() {}

OpenDirWindowsPlugin::~OpenDirWindowsPlugin() {}


static bool OpenDir(string path) {
	wstring temp = wstring(path.begin(), path.end());
	ShellExecute(NULL, L"open", temp.c_str(), NULL, NULL, SW_NORMAL);
	return true;
}

std::string GetPathArgument(const flutter::MethodCall<>& method_call) {
	std::string path;
	const auto* arguments = std::get_if<EncodableMap>(method_call.arguments());
	if (arguments) {
		auto url_it = arguments->find(EncodableValue("path"));
		if (url_it != arguments->end()) {
			path = std::get<std::string>(url_it->second);
		}
	}
	return path;
}

void OpenDirWindowsPlugin::HandleMethodCall(
    const flutter::MethodCall<flutter::EncodableValue> &method_call,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
  if (method_call.method_name().compare("openNativeDir") == 0) {
    std::string path = GetPathArgument(method_call);
    int rs = OpenDir(path);
    if (rs) {
      result->Success();
    }
    else {
      result->Error("Error", "Can not open this path!");
    }
  } else {
    result->NotImplemented();
  }
}

}  // namespace open_dir_windows

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

static std::wstring Utf8ToWide(const std::string& str) {
    if (str.empty()) {
        return std::wstring();
    }
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
    std::wstring wstrTo(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
    return wstrTo;
}

static bool OpenDir(const std::string& path, const std::string highlightedFileName = "") {
    std::wstring wpath = Utf8ToWide(path);
    
    HINSTANCE result;
    if (highlightedFileName.empty()) {
        result = ShellExecuteW(NULL, L"open", wpath.c_str(), NULL, NULL, SW_SHOW);
    } else {
        std::wstring wHighlightedFileName = Utf8ToWide(highlightedFileName);
        std::wstring wCommand = L"/select,\"" + wpath + L"\\" + wHighlightedFileName + L"\"";
        result = ShellExecuteW(NULL, L"open", L"explorer.exe", wCommand.c_str(), NULL, SW_SHOW);
    }
    return (result > reinterpret_cast<HINSTANCE>(32));
}

std::string GetStringArgument(const flutter::MethodCall<>& method_call, const std::string& key) {
    std::string value;
    const auto* arguments = std::get_if<EncodableMap>(method_call.arguments());
    if (arguments) {
        auto it = arguments->find(EncodableValue(key));
        if (it != arguments->end() && !it->second.IsNull()) {
            try {
                value = std::get<std::string>(it->second);
            } catch (const std::bad_variant_access&) {
                // Handle conversion error gracefully
                value = "";
            }
        }
    }
    return value;
}

void OpenDirWindowsPlugin::HandleMethodCall(
    const flutter::MethodCall<flutter::EncodableValue> &method_call,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
  if (method_call.method_name().compare("openNativeDir") == 0) {
    std::string path = GetStringArgument(method_call, "path");
    std::string highlightedFileName = GetStringArgument(method_call, "highlightedFileName");
    bool rs = OpenDir(path, highlightedFileName);
    result->Success(EncodableValue(rs));
  } else {
    result->NotImplemented();
  }
}

}  // namespace open_dir_windows

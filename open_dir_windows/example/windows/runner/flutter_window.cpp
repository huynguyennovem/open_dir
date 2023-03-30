#include "flutter_window.h"
#include <flutter/method_channel.h>
#include <flutter/standard_method_codec.h>

#include <optional>

#include "flutter/generated_plugin_registrant.h"
using namespace flutter;
using namespace std;

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

void initMethodChannel(flutter::FlutterEngine* flutter_instance) {
	const static std::string channel_name("com.flutter/open-dir-windows");
	auto channel =
		std::make_unique<flutter::MethodChannel<>>(
			flutter_instance->messenger(), channel_name,
			&flutter::StandardMethodCodec::GetInstance());
	channel->SetMethodCallHandler(
		[](const flutter::MethodCall<>& call, std::unique_ptr<flutter::MethodResult<>> result) {
			if (call.method_name() == "openNativeDir") {
				std::string path = GetPathArgument(call);
				int rs = OpenDir(path);
				if (rs) {
					result->Success();
				}
				else {
					result->Error("Error", "Can not open this path!");
				}
			}
			else {
				result->NotImplemented();
			}
		});
}

FlutterWindow::FlutterWindow(const flutter::DartProject& project)
	: project_(project) {}

FlutterWindow::~FlutterWindow() {}

bool FlutterWindow::OnCreate() {
	if (!Win32Window::OnCreate()) {
		return false;
	}

	RECT frame = GetClientArea();

	// The size here must match the window dimensions to avoid unnecessary surface
	// creation / destruction in the startup path.
	flutter_controller_ = std::make_unique<flutter::FlutterViewController>(
		frame.right - frame.left, frame.bottom - frame.top, project_);
	// Ensure that basic setup of the controller was successful.
	if (!flutter_controller_->engine() || !flutter_controller_->view()) {
		return false;
	}
	RegisterPlugins(flutter_controller_->engine());

	// initMethodChannel(flutter_controller_->engine());

	SetChildContent(flutter_controller_->view()->GetNativeWindow());

	flutter_controller_->engine()->SetNextFrameCallback([&]() {
		this->Show();
		});

	return true;
}

void FlutterWindow::OnDestroy() {
	if (flutter_controller_) {
		flutter_controller_ = nullptr;
	}

	Win32Window::OnDestroy();
}

LRESULT
FlutterWindow::MessageHandler(HWND hwnd, UINT const message,
	WPARAM const wparam,
	LPARAM const lparam) noexcept {
	// Give Flutter, including plugins, an opportunity to handle window messages.
	if (flutter_controller_) {
		std::optional<LRESULT> result =
			flutter_controller_->HandleTopLevelWindowProc(hwnd, message, wparam,
				lparam);
		if (result) {
			return *result;
		}
	}

	switch (message) {
	case WM_FONTCHANGE:
		flutter_controller_->engine()->ReloadSystemFonts();
		break;
	}

	return Win32Window::MessageHandler(hwnd, message, wparam, lparam);
}

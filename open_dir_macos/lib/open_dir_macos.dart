import 'package:flutter/services.dart';
import 'package:open_dir_platform_interface/open_dir_platform_interface.dart';

const MethodChannel _channel =
    MethodChannel('com.flutter/open-dir-macos');

class OpenDirMacOS extends OpenDirPlatform {

	@override
	Future<bool?> openNativeDir({required String path}) async {
		final result = await _channel.invokeMethod<bool>('openNativeDir', {'path': path});
    return result;
  }
}

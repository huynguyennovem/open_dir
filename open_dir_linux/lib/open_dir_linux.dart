import 'package:flutter/services.dart';
import 'package:open_dir_platform_interface/open_dir_platform_interface.dart';

const MethodChannel _channel = MethodChannel('com.flutter/open-dir-linux');

class OpenDirLinux extends OpenDirPlatform {
  /// Registers this class as the default instance of [OpenDirPlatform].
  static void registerWith() {
    OpenDirPlatform.instance = OpenDirLinux();
  }

  @override
  Future<bool?> openNativeDir({required String path}) async {
    try {
      final result =
          await _channel.invokeMethod<bool>('openNativeDir', {'path': path});
      return result;
    } on Exception catch (_) {
      return false;
    }
  }
}

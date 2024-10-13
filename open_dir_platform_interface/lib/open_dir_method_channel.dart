import 'package:flutter/foundation.dart';
import 'package:flutter/services.dart';

import 'open_dir_platform_interface.dart';

/// An implementation of [OpenDirPlatform] that uses method channels.
class MethodChannelOpenDir extends OpenDirPlatform {
  /// The method channel used to interact with the native platform.
  @visibleForTesting
  final methodChannel = const MethodChannel('com.flutter/open-dir');

  @override
  Future<bool?> openNativeDir({required String path, String? highlightedFileName}) async {
    final result = await methodChannel.invokeMethod<bool>('openNativeDir', {
      'path': path,
      'highlightedFileName': highlightedFileName,
    });
    return result;
  }
}

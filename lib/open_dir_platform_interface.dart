import 'package:plugin_platform_interface/plugin_platform_interface.dart';

import 'open_dir_method_channel.dart';

abstract class OpenDirPlatform extends PlatformInterface {
  /// Constructs a OpenDirPlatform.
  OpenDirPlatform() : super(token: _token);

  static final Object _token = Object();

  static OpenDirPlatform _instance = MethodChannelOpenDir();

  /// The default instance of [OpenDirPlatform] to use.
  ///
  /// Defaults to [MethodChannelOpenDir].
  static OpenDirPlatform get instance => _instance;

  /// Platform-specific implementations should set this with their own
  /// platform-specific class that extends [OpenDirPlatform] when
  /// they register themselves.
  static set instance(OpenDirPlatform instance) {
    PlatformInterface.verifyToken(instance, _token);
    _instance = instance;
  }

  Future<bool?> openNativeDir({required String path}) {
    throw UnimplementedError('openNativeDir() has not been implemented.');
  }
}

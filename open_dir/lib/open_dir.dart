import 'package:open_dir_platform_interface/open_dir_platform_interface.dart';

/// Open directory on native desktop platform
class OpenDir {
  /// Open native directory via calling channel defined on platform interface
  ///
  /// The given parameter is the [path] to the directory on the respective platform
  ///
  /// This returns true if the path can be opened. Otherwise, it maybe null or false.
  Future<bool?> openNativeDir({required String path}) {
    return OpenDirPlatform.instance.openNativeDir(path: path);
  }
}

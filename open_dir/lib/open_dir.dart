import 'package:open_dir_platform_interface/open_dir_platform_interface.dart';

class OpenDir {
  Future<bool?> openNativeDir({required String path}) {
    return OpenDirPlatform.instance.openNativeDir(path: path);
  }
}

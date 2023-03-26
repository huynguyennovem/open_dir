
import 'open_dir_platform_interface.dart';

class OpenDir {
  Future<bool?> openNativeDir({required String path}) {
    return OpenDirPlatform.instance.openNativeDir(path: path);
  }
}

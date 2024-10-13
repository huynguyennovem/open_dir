import 'dart:io';

import 'package:flutter_test/flutter_test.dart';
import 'package:open_dir/open_dir.dart';
import 'package:open_dir_platform_interface/open_dir_platform_interface.dart';
import 'package:open_dir_platform_interface/open_dir_method_channel.dart';
import 'package:plugin_platform_interface/plugin_platform_interface.dart';

class MockOpenDirPlatform
    with MockPlatformInterfaceMixin
    implements OpenDirPlatform {

  @override
  Future<bool?> openNativeDir({required String path, String? highlightedFileName}) {
    final dir = Directory(path);
    return dir.existsSync() ? Future.value(true) : Future.value(false);
  }
}

void main() {
  final OpenDirPlatform initialPlatform = OpenDirPlatform.instance;

  test('$MethodChannelOpenDir is the default instance', () {
    expect(initialPlatform, isInstanceOf<MethodChannelOpenDir>());
  });

  test('openNativeDir successfully', () async {
    OpenDir openDirPlugin = OpenDir();
    MockOpenDirPlatform fakePlatform = MockOpenDirPlatform();
    OpenDirPlatform.instance = fakePlatform;

    expect(await openDirPlugin.openNativeDir(path: '/Users/huynq/Desktop'), true);
  });

  test('openNativeDir failed by wrong path', () async {
    OpenDir openDirPlugin = OpenDir();
    MockOpenDirPlatform fakePlatform = MockOpenDirPlatform();
    OpenDirPlatform.instance = fakePlatform;

    expect(await openDirPlugin.openNativeDir(path: '/not-existing-path'), false);
  });
}

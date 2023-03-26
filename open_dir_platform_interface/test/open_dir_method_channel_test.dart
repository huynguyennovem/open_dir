import 'dart:io';

import 'package:flutter/services.dart';
import 'package:flutter_test/flutter_test.dart';
import 'package:open_dir_platform_interface/open_dir_method_channel.dart';

void main() {
  MethodChannelOpenDir platform = MethodChannelOpenDir();
  const MethodChannel channel = MethodChannel('com.flutter/open-dir');

  TestWidgetsFlutterBinding.ensureInitialized();

  setUp(() {
    channel.setMockMethodCallHandler((MethodCall methodCall) async {
      final path = methodCall.arguments['path'] as String;
      final dir = Directory(path);
      return dir.existsSync() ? Future.value(true) : Future.value(false);
    });
  });

  tearDown(() {
    channel.setMockMethodCallHandler(null);
  });

  test('openNativeDir', () async {
    expect(await platform.openNativeDir(path: '/Users/huynq/Desktop'), true);
  });
}

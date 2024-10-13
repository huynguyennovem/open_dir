import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'package:open_dir_windows/open_dir_windows.dart';

void main() {
  runApp(const MyApp());
}

class MyApp extends StatefulWidget {
  const MyApp({super.key});

  @override
  State<MyApp> createState() => _MyAppState();
}

class _MyAppState extends State<MyApp> {
  final _openDirPlugin = OpenDirWindows();

  final _tfDirPathController = TextEditingController();
  final _tfHighlightedFilenameController = TextEditingController();
  final _messengerKey = GlobalKey<ScaffoldMessengerState>();

  @override
  void dispose() {
    _tfDirPathController.dispose();
    _tfHighlightedFilenameController.dispose();
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      scaffoldMessengerKey: _messengerKey,
      home: Scaffold(
        appBar: AppBar(
          title: const Text('Open dir example'),
        ),
        body: Center(
          child: Container(
            margin: const EdgeInsets.symmetric(horizontal: 16.0),
            child: Column(
              mainAxisAlignment: MainAxisAlignment.center,
              children: [
                TextField(
                  controller: _tfDirPathController,
                  decoration: InputDecoration(
                    hintText: 'Directory path here',
                    hintStyle: TextStyle(color: Colors.grey.withOpacity(0.8)),
                  ),
                ),
                TextField(
                  controller: _tfHighlightedFilenameController,
                  decoration: InputDecoration(
                    hintText: 'Highlighted file name here',
                    hintStyle: TextStyle(color: Colors.grey.withOpacity(0.8)),
                  ),
                ),
                const SizedBox(height: 16.0),
                ElevatedButton(
                  onPressed: () => _openNativeDirectory(
                    path: _tfDirPathController.text,
                    highlightedFileName: _tfHighlightedFilenameController.text,
                  ),
                  child: const Padding(
                    padding: EdgeInsets.all(8.0),
                    child: Text('Open dir'),
                  ),
                ),
              ],
            ),
          ),
        ),
      ),
    );
  }

  _openNativeDirectory({required String path, String? highlightedFileName}) async {
    try {
      final rs = await _openDirPlugin.openNativeDir(
        path: path,
        highlightedFileName: highlightedFileName,
      );
      if (null == rs) {
        throw PlatformException(code: '404', message: 'Platform error while opening directory: $path');
      }
      if (rs) {
        if (highlightedFileName != null && highlightedFileName.isNotEmpty) {
          debugPrint('Opened file: $path with highlighted file name: $highlightedFileName');
          return;
        }
        debugPrint('Opened directory: $path');
      } else {
        debugPrint('Can not open directory: $path');
        if (mounted) {
          _messengerKey.currentState?.showSnackBar(SnackBar(
            duration: const Duration(seconds: 1),
            content: Text('Can not open directory: $path'),
          ));
        }
      }
    } on PlatformException catch (e) {
      debugPrint('Failed to open native directory: ${e.message}');
    }
  }
}

import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'package:open_dir_macos/open_dir_macos.dart';

void main() {
  runApp(const MyApp());
}

class MyApp extends StatefulWidget {
  const MyApp({super.key});

  @override
  State<MyApp> createState() => _MyAppState();
}

class _MyAppState extends State<MyApp> {
  final _openDirPlugin = OpenDirMacOS();

  final _tfController = TextEditingController();
  final _messengerKey = GlobalKey<ScaffoldMessengerState>();

  @override
  void dispose() {
    _tfController.dispose();
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
          child: Column(
            mainAxisAlignment: MainAxisAlignment.center,
            children: [
              TextField(
                controller: _tfController,
                decoration: const InputDecoration(
                  hintText: 'Paste the native directory path here',
                ),
              ),
              const SizedBox(height: 16.0),
              MaterialButton(
                color: Theme.of(context).colorScheme.primaryContainer,
                onPressed: () => _openNativeDirectory(_tfController.text),
                child: const Padding(
                  padding: EdgeInsets.all(8.0),
                  child: Text('Open dir', style: TextStyle(color: Colors.white)),
                ),
              ),
            ],
          ),
        ),
      ),
    );
  }

  _openNativeDirectory(String path) async {
    try {
      final rs = await _openDirPlugin.openNativeDir(path: path);
      if (null == rs) {
        throw PlatformException(
            code: '404', message: 'Platform error while opening directory: $path');
      }
      if (rs) {
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

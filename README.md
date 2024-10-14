## open_dir

Open directory on the native desktop platform with its app (Finder on MacOS, Files on Linux, File Explorer on Windows)

[![Twitter URL](https://img.shields.io/twitter/url/https/twitter.com/blueaquilae.svg?style=social&label=Follow%20HuyNguyenTw)](https://twitter.com/HuyNguyenTw)

### Supported platforms

MacOS | Linux | Windows | Android | iOS | Web |
| :-: | :---: | :-----: | :-----: | :-----: | :-----: |
| ✔️  |  ✔️   |   ✔️    |   ❌️️    |   ❌️️    |   ❌️️    |

### Usage

Add `open_dir` as a dependency in your `pubspec.yaml` file. This is [Endorsed federated plugin](https://docs.flutter.dev/development/packages-and-plugins/developing-packages#endorsed-federated-plugin).

This plugin is also supported to highlight/select a specific file after opening the directory.

### Example

```dart
final _openDirPlugin = OpenDir();
final path = '/path/to/directory';
final highlightedFileName = 'file.txt';
await _openDirPlugin.openNativeDir(path: path, highlightedFileName: highlightedFileName);
```

### Demo

| Open dir | Open dir and highlight file |
| --------------- | --------------- |
<video src="https://user-images.githubusercontent.com/29337364/229332980-eb111200-b13a-44b6-8f56-a1793b24d863.mp4"/> | <video src="https://github.com/user-attachments/assets/8aa591d8-ee08-4dea-9294-be0defe00098"/>

## open_dir

Open directory on the native desktop platform with its app (Finder on MacOS, Files on Linux, File Explorer on Windows)

### Supported platforms

MacOS | Linux | Windows | Android | iOS | Web |
| :-: | :---: | :-----: | :-----: | :-----: | :-----: |
| ✔️  |  ✔️   |   ✔️    |   ❌️️    |   ❌️️    |   ❌️️    |

### Usage

Add `open_dir` as a dependency in your `pubspec.yaml` file. This is [Endorsed federated plugin](https://docs.flutter.dev/development/packages-and-plugins/developing-packages#endorsed-federated-plugin).

### Example

```dart
final _openDirPlugin = OpenDir();
await _openDirPlugin.openNativeDir(path: path);
```
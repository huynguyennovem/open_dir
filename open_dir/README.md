## open_dir

Open directory on native desktop platform with its app(Finder on MacOS, Files on Linux, File Explorer on Windows)

### Platform Support

MacOS | Linux | Windows |
| :-: | :---: | :-----: |
| ✔️  |  ✔️   |   ✔️    |

### Usage

`open_dir` can be used easily by calling `OpenDir` class.

```dart
final _openDirPlugin = OpenDir();
await _openDirPlugin.openNativeDir(path: path);
```
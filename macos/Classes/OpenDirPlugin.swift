import Cocoa
import FlutterMacOS

public class OpenDirPlugin: NSObject, FlutterPlugin {
  public static func register(with registrar: FlutterPluginRegistrar) {
    let channel = FlutterMethodChannel(name: "com.flutter/open-dir", binaryMessenger: registrar.messenger)
    let instance = OpenDirPlugin()
    registrar.addMethodCallDelegate(instance, channel: channel)
  }

  public func handle(_ call: FlutterMethodCall, result: @escaping FlutterResult) {
    guard let path = (call.arguments as? NSDictionary)?["path"] as? String else {
        result(FlutterMethodNotImplemented)
        return
    }
    switch call.method {
        case "openNativeDir":
          let rs = self.openNativeDir(path: path, result: result);
          result(rs)
        default:
          result(FlutterMethodNotImplemented)
    }
  }

  func openNativeDir(path: String, result: FlutterResult) -> Bool {
    if(!isPathExist(path: path)) {
        return false
    }
    let url = URL(fileURLWithPath: path, isDirectory: true)
    NSWorkspace.shared.activateFileViewerSelecting([url])
    return true
  }

  func isPathExist(path: String) -> Bool {
    var isDir : ObjCBool = true
    return FileManager.default.fileExists(atPath: path, isDirectory: &isDir)
  }
}

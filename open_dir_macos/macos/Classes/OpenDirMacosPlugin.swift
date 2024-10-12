import Cocoa
import FlutterMacOS

public class OpenDirMacosPlugin: NSObject, FlutterPlugin {
  public static func register(with registrar: FlutterPluginRegistrar) {
    let channel = FlutterMethodChannel(name: "com.flutter/open-dir-macos", binaryMessenger: registrar.messenger)
    let instance = OpenDirMacosPlugin()
    registrar.addMethodCallDelegate(instance, channel: channel)
  }

  public func handle(_ call: FlutterMethodCall, result: @escaping FlutterResult) {
    guard let args = call.arguments as? NSDictionary,
          let path = args["path"] as? String else {
        result(FlutterMethodNotImplemented)
        return
    }
    let highlightedFileName = args["highlightedFileName"] as? String
    
    switch call.method {
    case "openNativeDir":
        let rs = self.openNativeDir(path: path, highlightedFileName: highlightedFileName, result: result)
        result(rs)
    default:
        result(FlutterMethodNotImplemented)
    }
  }

  func openNativeDir(path: String, highlightedFileName: String?, result: FlutterResult) -> Bool {
    if (!isPathExist(path: path)) {
        return false
    }
    
    let url = URL(fileURLWithPath: path, isDirectory: true)
    
    if let highlightedFileName = highlightedFileName {
        let fileURL = url.appendingPathComponent(highlightedFileName)
        if isPathExist(path: fileURL.path, isDirectory: false) {
            NSWorkspace.shared.activateFileViewerSelecting([fileURL])
        } else {
            NSWorkspace.shared.activateFileViewerSelecting([url])
        }
    } else {
        NSWorkspace.shared.activateFileViewerSelecting([url])
    }
    
    return true
  }

  func isPathExist(path: String, isDirectory: Bool? = nil) -> Bool {
    var isDir: ObjCBool = false
    let exists = FileManager.default.fileExists(atPath: path, isDirectory: &isDir)
    if let isDirectory = isDirectory {
      return exists && isDirectory == isDir.boolValue
    }
    return exists
  }
}

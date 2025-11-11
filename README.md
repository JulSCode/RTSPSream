# RTSP Player for Android - Qt and libVLC

A minimal example of an RTSP player for Android, developed with **Qt** and **libVLC** to provide low-latency video streaming (0.5 - 1 second without buffer accumulation).

## Features

- Real-time RTSP stream playback with minimal latency
- Optimized for low-latency transmission
- Built on Qt 6.x and libVLC
- Full Android ARM64 support
- Clean and educational codebase

## Requirements

### Build Dependencies

- Qt 6.x (minimum 6.6)
- CMake 3.16+
- Android SDK
- Android NDK
- libVLC for Android

### libVLC Library Dependencies

The project requires specific libVLC components that must be placed in the correct Android directories.

## Installation and Configuration

### 1. Clone the Repository

```bash
git clone https://github.com/JulSCode/RTSPSream.git
cd RTSPSream
```

### 2. Adding libVLC Files

Download the required libVLC files from [Maven Repository](https://mvnrepository.com/artifact/org.videolan.android/libvlc-all) and place them in the following directory structure:

#### Project File Structure

```
RTSPSream/
├── android/
│   ├── libs/
│   │   └── libvlc-all-4.0.0-eap23.aar          ← Download from Maven
│   ├── jniLibs/
│   │   └── arm64-v8a/
│   │       ├── libc++_shared.so                 ← Download from Maven
│   │       ├── libvlc.so                        ← Download from Maven
│   │       └── libvlcjni.so                     ← Download from Maven
│   ├── src/
│   │   └── org/qtproject/example/
│   │       ├── SystemUIHelper.java
│   │       └── ... (other Java files)
│   └── AndroidManifest.xml
├── include/
│   ├── androidhelper.h
│   └── vlcbridge.h
├── src/
│   ├── main.cpp
│   ├── androidhelper.cpp
│   └── ... (other C++ files)
├── res/
│   ├── icons/
│   │   ├── icon1.png
│   │   ├── icon2.png
│   │   ├── pause.png
│   │   ├── play.png
│   │   └── stop.png
│   └── resources.qrc
├── CMakeLists.txt
└── README.md
```

### 3. Extracting libVLC Files from AAR

The file `libvlc-all-4.0.0-eap23.aar` is a compressed archive (ZIP format). To extract the native libraries:

1. **Unpack the AAR file:**
   ```bash
   # On Windows (PowerShell)
   Expand-Archive android/libs/libvlc-all-4.0.0-eap23.aar -DestinationPath libvlc_temp
   
   # On Linux/macOS
   unzip android/libs/libvlc-all-4.0.0-eap23.aar -d libvlc_temp
   ```

2. **Copy the required files:**
   ```bash
   # On Windows (PowerShell)
   Copy-Item libvlc_temp/jni/arm64-v8a/libc++_shared.so android/jniLibs/arm64-v8a/
   Copy-Item libvlc_temp/jni/arm64-v8a/libvlc.so android/jniLibs/arm64-v8a/
   Copy-Item libvlc_temp/jni/arm64-v8a/libvlcjni.so android/jniLibs/arm64-v8a/
   
   # On Linux/macOS
   cp libvlc_temp/jni/arm64-v8a/*.so android/jniLibs/arm64-v8a/
   ```

3. **Clean up the temporary directory:**
   ```bash
   rm -r libvlc_temp
   ```

### 4. Building the Project

```bash
mkdir build
cd build
cmake -GNinja -DCMAKE_TOOLCHAIN_FILE=<path_to_NDK>/build/cmake/android.cmake \
      -DANDROID_PLATFORM=android-28 \
      -DANDROID_ABI=arm64-v8a ..
cmake --build .
```

Or use Qt Creator:
1. Open `CMakeLists.txt` in Qt Creator
2. Select the Android profile
3. Choose the ARM64 configuration
4. Click "Build"

## Usage

After successful build, install the application on an Android device:

```bash
# If built with CMake
cmake --install . --config Release

# Or using adb
adb install app.apk
```

Run the application and enter the RTSP stream URL for playback.

## Project Structure

- **`src/`** - C++ source code
- **`include/`** - Header files
- **`android/`** - Android-specific files and resources
- **`res/`** - Application resources (icons, QML files)
- **`CMakeLists.txt`** - Build configuration

## Latency Optimization Notes

To achieve minimum latency (0.5 - 1 second):

- Uses libVLC 4.0.0 EAP23 with streaming optimizations
- ARM64 devices are recommended for better performance
- Ensure stable network connectivity
- Depending on the RTSP source, additional buffer configuration may be required

## License

MIT License

## Author

JulSCode

## Contributing

Pull requests and issues are welcome. For major changes, please open an issue first to discuss the proposed changes.

## Useful Links

- [Qt for Android Documentation](https://doc.qt.io/qt-6/android.html)
- [libVLC Android Documentation](https://wiki.videolan.org/AndroidCompile/)
- [Maven Repository libvlc-all](https://mvnrepository.com/artifact/org.videolan.android/libvlc-all)
- [RTSP Protocol Specification](https://tools.ietf.org/html/rfc2326)

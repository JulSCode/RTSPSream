cmake_minimum_required(VERSION 3.16)

project(RTSPStream VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick Multimedia Core Gui Qml)

qt_standard_project_setup(REQUIRES 6.8)

# Ресурсы - упрощенный подход без OUTPUT_DIRECTORY
set(QRC_FILE "${CMAKE_CURRENT_SOURCE_DIR}/res/resources.qrc")
qt_add_resources(TRGT_qrc_cpp ${QRC_FILE})

qt_add_executable(appRTSPStream
    src/main.cpp
    ${TRGT_qrc_cpp}
)

qt_add_qml_module(appRTSPStream
    URI RTSPStream
    VERSION 1.0
    QML_FILES
        qml/Main.qml
        qml/CustomTextInput.qml
    SOURCES
        include/vlcbridge.h
        src/vlcbridge.cpp
        include/androidhelper.h
        src/androidhelper.cpp
    RESOURCES
        res/resources.qrc
        android/AndroidManifest.xml
        android/build.gradle
        android/res/values/libs.xml
        android/res/xml/qtprovider_paths.xml
        android/gradle/wrapper/gradle-wrapper.jar
        android/gradle/wrapper/gradle-wrapper.properties
        android/gradle.properties
        android/gradlew
        android/gradlew.bat
)

set_target_properties(appRTSPStream PROPERTIES
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# === Общие заголовки: VLC ===
target_include_directories(appRTSPStream PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include  # для vlc/vlc.h
)

# === Стандартные Qt-библиотеки ===
target_link_libraries(appRTSPStream
    PRIVATE
        Qt6::Quick
        Qt6::Multimedia
        Qt6::Core
        Qt6::Gui
        Qt6::Qml
)

# === Android-специфичная настройка:  libvlc (линковка + упаковка) ===
if(ANDROID)
    # Пути к нативным библиотекам
    set(VLC_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/android/jniLibs/arm64-v8a)

    # Линковка: libvlc
    target_link_libraries(appRTSPStream PRIVATE
        ${VLC_LIB_DIR}/libvlc.so
    )

    # Свойства Android-пакета
    set_target_properties(appRTSPStream PROPERTIES
        QT_ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android"
        QT_ANDROID_MIN_SDK_VERSION 21
        QT_ANDROID_TARGET_SDK_VERSION 33
    )

    # Упаковка .so-файлов в APK (все необходимые нативные библиотеки)
    set_property(TARGET appRTSPStream APPEND PROPERTY
        QT_ANDROID_EXTRA_LIBS
        "${VLC_LIB_DIR}/libvlc.so"
        "${VLC_LIB_DIR}/libvlcjni.so"      # для Java-моста
        "${VLC_LIB_DIR}/libc++_shared.so"  # STL от NDK
    )
endif()

include(GNUInstallDirs)
install(TARGETS appRTSPStream
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

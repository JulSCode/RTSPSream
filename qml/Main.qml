import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Window {
    id: root
    width: 1080
    height: 1920
    visible: true
    title: qsTr("RTSP Viewer")

    // Define color constants for consistent UI styling across the application
    // Определить цветовые константы для согласованного стиля UI во всем приложении
    readonly property color backgroundDark: "#000000"
    readonly property color inputFieldBackground: "#ffffff"
    readonly property color inputFieldText: "#333333"
    readonly property color inputFieldPlaceholder: "#888888"
    readonly property color buttonPlayGreen: "#4caf50"
    readonly property color buttonPauseYellow: "#ff9800"
    readonly property color buttonStopRed: "#f44336"
    readonly property color buttonGreenDisabled: "#2a8d20"
    readonly property color buttonYellowDisabled: "#cc7a00"
    readonly property color buttonRedDisabled: "#cc3333"

    // Maintain application state variables that are bound to UI elements
    // Поддерживать переменные состояния приложения, привязанные к элементам UI
    property string streamUrl: ""
    property bool showControls: true
    property string statusText: "Stopped"
    property int fpsValue: 0
    property bool isPlaying: false
    property bool isPaused: false

    // Connect to signals from the C++ VLCBridge class to update QML state
    // Подключиться к сигналам из C++ класса VLCBridge для обновления состояния QML
    Connections {
        target: vlcBridge

        // Receive playing state changes from C++
        // Получить изменения состояния воспроизведения из C++
        function onIsPlayingChanged(value) { root.isPlaying = value }

        // Receive paused state changes from C++
        // Получить изменения состояния паузы из C++
        function onIsPausedChanged(value) { root.isPaused = value }

        // Receive status text updates (Playing, Paused, Stopped, etc.)
        // Получить обновления текста статуса (Playing, Paused, Stopped, и т.д.)
        function onStatusChanged(status) { root.statusText = status }

        // Receive error messages from C++ and log them to console
        // Получить сообщения об ошибках из C++ и залогировать их в консоль
        function onError(message) { console.error("VLC Error:", message) }
    }

    // Variables and timer for detecting double-tap gesture to toggle controls visibility
    // Переменные и таймер для обнаружения жеста двойного нажатия для переключения видимости элементов управления
    property real lastTapTime: 0
    Timer { id: doubleTapTimer; interval: 300 }

    // Root rectangle covering the entire window with dark background
    // Корневой прямоугольник, покрывающий все окно с темным фоном
    Rectangle {
        anchors.fill: parent
        color: root.backgroundDark

        // Capture mouse clicks to detect double-tap and toggle control panel visibility
        // Захватить клики мыши для обнаружения двойного нажатия и переключения видимости панели управления
        MouseArea {
            anchors.fill: parent
            onClicked: {
                let currentTime = new Date().getTime()
                if (currentTime - root.lastTapTime < 300) {
                    root.showControls = !root.showControls
                    root.lastTapTime = 0
                } else {
                    root.lastTapTime = currentTime
                }
            }
        }

        // Main layout container organizing video display and control panel vertically
        // Основной контейнер макета, организующий дисплей видео и панель управления вертикально
        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 5
            spacing: 5

            // Container where the VLC video surface will be rendered
            // Контейнер, где будет отображаться видеоповерхность VLC
            Item {
                id: videoContainer
                Layout.fillWidth: true
                Layout.fillHeight: true

                // Monitor width changes and update VLC surface position if playing
                // Контролировать изменения ширины и обновить позицию поверхности VLC, если воспроизведение
                onWidthChanged: {
                    if (vlcBridge.isPlaying) {
                        var rect = videoContainer.mapToItem(null, 0, 0);
                        vlcBridge.updatePosition(videoContainer, rect.x, rect.y, videoContainer.width, videoContainer.height);
                        console.log("videoContainer width:", videoContainer.width)
                    }
                }
                // Monitor height changes and update VLC surface position if playing
                // Контролировать изменения высоты и обновить позицию поверхности VLC, если воспроизведение
                onHeightChanged: {
                    if (vlcBridge.isPlaying) {
                        var rect = videoContainer.mapToItem(null, 0, 0);
                        vlcBridge.updatePosition(videoContainer, rect.x, rect.y, videoContainer.width, videoContainer.height);
                        console.log("videoContainer height:", videoContainer.height)
                    }
                }
                // Monitor X position changes and update VLC surface position if playing
                // Контролировать изменения позиции X и обновить позицию поверхности VLC, если воспроизведение
                onXChanged: {
                    if (vlcBridge.isPlaying) {
                        var rect = videoContainer.mapToItem(null, 0, 0);
                        vlcBridge.updatePosition(videoContainer, rect.x, rect.y, videoContainer.width, videoContainer.height);
                    }
                }
                // Monitor Y position changes and update VLC surface position if playing
                // Контролировать изменения позиции Y и обновить позицию поверхности VLC, если воспроизведение
                onYChanged: {
                    if (vlcBridge.isPlaying) {
                        var rect = videoContainer.mapToItem(null, 0, 0);
                        vlcBridge.updatePosition(videoContainer, rect.x, rect.y, videoContainer.width, videoContainer.height);
                    }
                }
            }

            // Bottom control panel containing URL input field and playback buttons
            // Нижняя панель управления, содержащая поле ввода URL и кнопки воспроизведения
            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 50
                color: "transparent"
                visible: root.showControls
                radius: 0

                // Horizontal layout for control elements
                // Горизонтальный макет для элементов управления
                RowLayout {
                    anchors.fill: parent
                    spacing: 5

                    // Custom text input for entering RTSP stream URL
                    // Пользовательское текстовое поле для ввода URL RTSP потока
                    CustomTextInput {
                        id: urlInput
                        Layout.fillWidth: true
                        Layout.fillHeight: true

                        text: root.streamUrl
                        placeholderText: "rtsp://example.com/stream"
                        textColor: root.inputFieldText
                        placeholderColor: root.inputFieldPlaceholder
                        backgroundColor: root.inputFieldBackground
                        radius: 5

                        onTextChanged: root.streamUrl = text
                    }

                    // Button to start RTSP stream playback
                    // Кнопка для запуска воспроизведения RTSP потока
                    Button {
                        Layout.preferredWidth: 50
                        Layout.fillHeight: true
                        padding: 0
                        background: Rectangle {
                            anchors.fill: parent
                            // Change color based on pressed state for visual feedback
                            // Изменить цвет в зависимости от состояния нажатия для визуальной обратной связи
                            color: parent.pressed ? root.buttonGreenDisabled : root.buttonPlayGreen
                            radius: 5
                        }
                        contentItem: Image {
                            source: "qrc:/icons/play.png"
                            anchors.centerIn: parent
                            fillMode: Image.PreserveAspectFit
                        }
                        onClicked: {
                            // Only attempt playback if URL is provided
                            // Попытаться воспроизвести только если предоставлен URL
                            if (root.streamUrl.length > 0) {
                                // Use Qt.callLater to defer execution until next event cycle
                                // Использовать Qt.callLater для отсрочки выполнения до следующего цикла событий
                                Qt.callLater(function() {
                                    // Get container position relative to screen root
                                    // Получить позицию контейнера относительно корневого экрана
                                    var rect = videoContainer.mapToItem(null, 0, 0);
                                    console.log("Starting playback at", rect.x, rect.y, videoContainer.width, videoContainer.height);
                                    // Call C++ VLCBridge to start playback with stream URL and position
                                    // Вызвать C++ VLCBridge для запуска воспроизведения с URL потока и позицией
                                    vlcBridge.play(root.streamUrl, videoContainer, rect.x, rect.y, videoContainer.width, videoContainer.height);
                                });
                            }
                        }
                    }

                    // Button to pause current RTSP stream playback
                    // Кнопка для паузы текущего воспроизведения RTSP потока
                    Button {
                        Layout.preferredWidth: 50
                        Layout.fillHeight: true
                        // Only enable pause button when video is actually playing
                        // Включить кнопку паузы только когда видео действительно воспроизводится
                        enabled: root.isPlaying
                        padding: 0
                        background: Rectangle {
                            anchors.fill: parent
                            // Adjust color based on enabled and pressed states
                            // Отрегулировать цвет в зависимости от состояния включения и нажатия
                            color: {
                                if (!enabled) return root.buttonYellowDisabled
                                if (parent.pressed) return root.buttonYellowDisabled
                                return root.buttonPauseYellow
                            }
                            radius: 5
                        }
                        contentItem: Image {
                            source: "qrc:/icons/pause.png"
                            anchors.centerIn: parent
                            fillMode: Image.PreserveAspectFit
                            // Reduce opacity when button is disabled
                            // Уменьшить прозрачность, когда кнопка отключена
                            opacity: enabled ? 1.0 : 0.5
                        }
                        onClicked: vlcBridge.pause()
                    }

                    // Button to stop current RTSP stream playback
                    // Кнопка для остановки текущего воспроизведения RTSP потока
                    Button {
                        Layout.preferredWidth: 50
                        Layout.fillHeight: true
                        // Enable stop button when video is playing or paused
                        // Включить кнопку остановки, когда видео воспроизводится или приостановлено
                        enabled: root.isPlaying || root.isPaused
                        padding: 0
                        background: Rectangle {
                            anchors.fill: parent
                            // Adjust color based on enabled and pressed states
                            // Отрегулировать цвет в зависимости от состояния включения и нажатия
                            color: {
                                if (!enabled) return root.buttonRedDisabled
                                if (parent.pressed) return root.buttonRedDisabled
                                return root.buttonStopRed
                            }
                            radius: 5
                        }
                        contentItem: Image {
                            source: "qrc:/icons/stop.png"
                            anchors.centerIn: parent
                            fillMode: Image.PreserveAspectFit
                            // Reduce opacity when button is disabled
                            // Уменьшить прозрачность, когда кнопка отключена
                            opacity: enabled ? 1.0 : 0.5
                        }
                        onClicked: vlcBridge.stop()
                    }
                }
            }
        }
    }
}

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Window {
    id: root
    width: 1080
    height: 1920
    visible: true
    title: qsTr("RTSP Viewer")

    // ========== COLOR PALETTE ==========
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

    // ========== STATE MANAGEMENT ==========
    property string streamUrl: ""
    property bool showControls: true
    property string statusText: "Stopped"
    property int fpsValue: 0
    property bool isPlaying: false
    property bool isPaused: false

    // ========== CONNECTIONS TO C++ ==========
    Connections {
        target: vlcBridge

        function onIsPlayingChanged(value) { root.isPlaying = value }
        function onIsPausedChanged(value) { root.isPaused = value }
        function onStatusChanged(status) { root.statusText = status }
        function onError(message) { console.error("VLC Error:", message) }
    }

    // ========== DOUBLE TAP TIMER ==========
    property real lastTapTime: 0
    Timer { id: doubleTapTimer; interval: 300 }

    // ========== MAIN BACKGROUND ==========
    Rectangle {
        anchors.fill: parent
        color: root.backgroundDark

        // ========== MOUSE AREA FOR DOUBLE TAP ==========
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

        // ========== MAIN COLUMN LAYOUT ==========
        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 5
            spacing: 5

            // ========== VIDEO DISPLAY AREA ==========
            Item {
                id: videoContainer
                Layout.fillWidth: true
                Layout.fillHeight: true

                // Слушаем изменения размера и позиции
                onWidthChanged: {
                    if (vlcBridge.isPlaying) {
                        var rect = videoContainer.mapToItem(null, 0, 0);
                        vlcBridge.updatePosition(videoContainer, rect.x, rect.y, videoContainer.width, videoContainer.height);
                        console.log("videoContainer width:", videoContainer.width)
                    }
                }
                onHeightChanged: {
                    if (vlcBridge.isPlaying) {
                        var rect = videoContainer.mapToItem(null, 0, 0);
                        vlcBridge.updatePosition(videoContainer, rect.x, rect.y, videoContainer.width, videoContainer.height);
                        console.log("videoContainer height:", videoContainer.height)
                    }
                }
                onXChanged: {
                    if (vlcBridge.isPlaying) {
                        var rect = videoContainer.mapToItem(null, 0, 0);
                        vlcBridge.updatePosition(videoContainer, rect.x, rect.y, videoContainer.width, videoContainer.height);
                    }
                }
                onYChanged: {
                    if (vlcBridge.isPlaying) {
                        var rect = videoContainer.mapToItem(null, 0, 0);
                        vlcBridge.updatePosition(videoContainer, rect.x, rect.y, videoContainer.width, videoContainer.height);
                    }
                }
            }

            // ========== CONTROL PANEL ==========
            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 50
                color: "transparent"
                visible: root.showControls
                radius: 0

                RowLayout {
                    anchors.fill: parent
                    //anchors.margins: 5
                    spacing: 5

                    // URL INPUT FIELD
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

                    // PLAY BUTTON
                    Button {
                        Layout.preferredWidth: 50
                        Layout.fillHeight: true
                        padding: 0
                        background: Rectangle {
                            anchors.fill: parent
                            color: parent.pressed ? root.buttonGreenDisabled : root.buttonPlayGreen
                            radius: 5
                        }
                        contentItem: Image {
                            source: "qrc:/icons/play.png"
                            anchors.centerIn: parent
                            fillMode: Image.PreserveAspectFit
                        }
                        onClicked: {
                            if (root.streamUrl.length > 0) {
                                Qt.callLater(function() {
                                    var rect = videoContainer.mapToItem(null, 0, 0);
                                    console.log("Starting playback at", rect.x, rect.y, videoContainer.width, videoContainer.height);
                                    vlcBridge.play(root.streamUrl, videoContainer, rect.x, rect.y, videoContainer.width, videoContainer.height);
                                });
                            }
                        }
                    }

                    // PAUSE BUTTON
                    Button {
                        Layout.preferredWidth: 50
                        Layout.fillHeight: true
                        enabled: root.isPlaying
                        padding: 0
                        background: Rectangle {
                            anchors.fill: parent
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
                            opacity: enabled ? 1.0 : 0.5
                        }
                        onClicked: vlcBridge.pause()
                    }

                    // STOP BUTTON
                    Button {
                        Layout.preferredWidth: 50
                        Layout.fillHeight: true
                        enabled: root.isPlaying || root.isPaused
                        padding: 0
                        background: Rectangle {
                            anchors.fill: parent
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
                            opacity: enabled ? 1.0 : 0.5
                        }
                        onClicked: vlcBridge.stop()
                    }
                }
            }
        }
    }
}

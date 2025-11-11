import QtQuick

Rectangle {
    id: customTextInput

    // Свойства компонента
    property alias text: input.text
    property string placeholderText: "rtsp://example.com/stream"
    property color textColor: "#000000"
    property color placeholderColor: "#999999"
    property color backgroundColor: "#FFFFFF"
    property color borderColor: "#CCCCCC"
    property real borderWidth: 1
    property real borderRadius: 5
    property alias inputMethodHints: input.inputMethodHints

    // Сигналы для внешней логики (переименованы, чтобы не конфликтовать с встроенными)
    signal onTextValueChanged(string newText)
    signal onAcceptedTriggered()
    signal onEditingComplete()

    width: 300
    height: 50
    color: backgroundColor
    border.color: borderColor
    border.width: borderWidth
    radius: borderRadius

    // TextInput занимает всю ширину
    TextInput {
        id: input

        anchors {
            fill: parent
            leftMargin: 8
            rightMargin: 8
        }

        font.pixelSize: 14
        color: textColor

        // Текст по-левому, вертикально по-центру
        horizontalAlignment: TextInput.AlignLeft
        verticalAlignment: TextInput.AlignVCenter

        // Для работы на мобильных
        inputMethodHints: Qt.ImhUrlCharactersOnly | Qt.ImhNoPredictiveText
        selectByMouse: true
        clip: true

        onTextChanged: customTextInput.onTextValueChanged(text)
        onAccepted: customTextInput.onAcceptedTriggered()
        onEditingFinished: customTextInput.onEditingComplete()
    }

    // Placeholder текст — пропадает при вводе ИЛИ при активации поля
    Text {
        id: placeholder

        anchors {
            left: parent.left
            leftMargin: 8
            verticalCenter: parent.verticalCenter
        }

        text: customTextInput.placeholderText
        color: placeholderColor
        font.pixelSize: 14

        // Видим только если поле пусто И не в фокусе
        visible: input.text.length === 0 && !input.activeFocus
    }
}

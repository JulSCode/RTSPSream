import QtQuick

// Custom text input component for entering RTSP stream URLs
// Пользовательский компонент текстового ввода для введения URL RTSP потоков
Rectangle {
    id: customTextInput

    // Alias to TextInput's text property for direct access from parent
    // Псевдоним свойства text TextInput для прямого доступа из родителя
    property alias text: input.text

    // Placeholder text displayed when input field is empty
    // Текст-заполнитель, отображаемый, когда поле ввода пусто
    property string placeholderText: "rtsp://example.com/stream"

    // Color of the actual text typed by user
    // Цвет фактического текста, введенного пользователем
    property color textColor: "#000000"

    // Color of the placeholder hint text
    // Цвет текста подсказки-заполнителя
    property color placeholderColor: "#999999"

    // Background fill color of the input field
    // Цвет фонового заполнения поля ввода
    property color backgroundColor: "#FFFFFF"

    // Border color of the input field rectangle
    // Цвет границы прямоугольника поля ввода
    property color borderColor: "#CCCCCC"

    // Width of the border stroke
    // Ширина обводки границы
    property real borderWidth: 1

    // Corner radius for rounded rectangle appearance
    // Радиус угла для закругленного прямоугольного вида
    property real borderRadius: 5

    // Alias to TextInput's input method hints for mobile keyboard configuration
    // Псевдоним для подсказок метода ввода TextInput для конфигурации мобильной клавиатуры
    property alias inputMethodHints: input.inputMethodHints

    // Signal emitted whenever the text content changes
    // Сигнал, выпущенный при каждом изменении содержимого текста
    signal onTextValueChanged(string newText)

    // Signal emitted when user presses Enter/Return key
    // Сигнал, выпущенный, когда пользователь нажимает клавишу Enter/Return
    signal onAcceptedTriggered()

    // Signal emitted when user leaves the input field (loses focus)
    // Сигнал, выпущенный, когда пользователь покидает поле ввода (теряет фокус)
    signal onEditingComplete()

    width: 300
    height: 50

    // Set background color and border properties
    // Установить цвет фона и свойства границы
    color: backgroundColor
    border.color: borderColor
    border.width: borderWidth
    radius: borderRadius

    // Main TextInput component for user text entry
    // Основной компонент TextInput для ввода текста пользователем
    TextInput {
        id: input

        // Fill entire parent rectangle with left/right margins for text padding
        // Заполнить весь родительский прямоугольник с левым/правым отступом для заполнения текста
        anchors {
            fill: parent
            leftMargin: 8
            rightMargin: 8
        }

        // Set font size for readability
        // Установить размер шрифта для читаемости
        font.pixelSize: 14

        // Use the user-defined text color
        // Использовать цвет текста, определенный пользователем
        color: textColor

        // Align text to the left side and vertically center it
        // Выровнять текст к левому краю и вертикально центрировать его
        horizontalAlignment: TextInput.AlignLeft
        verticalAlignment: TextInput.AlignVCenter

        // Configure input method hints for mobile keyboards
        // Конфигурировать подсказки метода ввода для мобильных клавиатур
        // ImhUrlCharactersOnly: Show URL-specific keyboard (includes :, /, @, etc.)
        // ImhUrlCharactersOnly: Показать клавиатуру, специфичную для URL (включает :, /, @, и т.д.)
        // ImhNoPredictiveText: Disable autocomplete/prediction on mobile devices
        // ImhNoPredictiveText: Отключить автозавершение/предсказание на мобильных устройствах
        inputMethodHints: Qt.ImhUrlCharactersOnly | Qt.ImhNoPredictiveText

        // Enable text selection by mouse/touch
        // Включить выделение текста мышью/касанием
        selectByMouse: true

        // Clip text that extends beyond component boundaries
        // Обрезать текст, который выходит за границы компонента
        clip: true

        // Emit signal when text is modified by user
        // Выпустить сигнал, когда текст изменен пользователем
        onTextChanged: customTextInput.onTextValueChanged(text)

        // Emit signal when user confirms input (presses Enter/Return)
        // Выпустить сигнал, когда пользователь подтверждает ввод (нажимает Enter/Return)
        onAccepted: customTextInput.onAcceptedTriggered()

        // Emit signal when input field loses focus
        // Выпустить сигнал, когда поле ввода теряет фокус
        onEditingFinished: customTextInput.onEditingComplete()
    }

    // Hint text displayed when input field is empty and not focused
    // Текст подсказки, отображаемый, когда поле ввода пусто и не в фокусе
    Text {
        id: placeholder

        // Position placeholder text at left margin, vertically centered
        // Позиционировать текст заполнителя на левый отступ, вертикально центрировано
        anchors {
            left: parent.left
            leftMargin: 8
            verticalCenter: parent.verticalCenter
        }

        // Display the placeholder text
        // Отобразить текст заполнителя
        text: customTextInput.placeholderText

        // Use the placeholder-specific color for visual distinction
        // Использовать специфичный для заполнителя цвет для визуального различия
        color: placeholderColor

        // Match TextInput font size for alignment consistency
        // Соответствовать размеру шрифта TextInput для согласованности выравнивания
        font.pixelSize: 14

        // Show placeholder only when:
        // 1. Input text is completely empty (length === 0)
        // 2. AND the input field is not in focus (not actively being edited)
        // Показать заполнитель только когда:
        // 1. Текст ввода полностью пуст (length === 0)
        // 2. И поле ввода не в фокусе (не активно редактируется)
        visible: input.text.length === 0 && !input.activeFocus
    }
}

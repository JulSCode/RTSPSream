// ========== HEADER GUARD ==========
// Prevent multiple inclusion of this header file in the same compilation unit
// Предотвратить множественное включение этого файла заголовка в той же единице компиляции
#pragma once

#include <QObject>
#include <QString>
#include <QQuickItem>
#include <QJniObject>
#include <QJniEnvironment>

// ========== CLASS DECLARATION ==========
// VLCBridge: Bridge class connecting QML (C++ side) with Android VLC playback via JNI
// VLCBridge: Класс-мост, соединяющий QML (сторона C++) с воспроизведением VLC Android через JNI
class VLCBridge : public QObject
{
    // Enable Qt's meta-object system for signals, slots, and properties
    // Включить систему мета-объектов Qt для сигналов, слотов и свойств
    Q_OBJECT

    // ========== Q_PROPERTY DECLARATIONS ==========
    // Expose C++ properties to QML with automatic change notifications
    // Expose свойства C++ для QML с автоматическим уведомлением об изменениях

    // Read-only property indicating whether video is currently playing
    // READ: getter method isPlaying()
    // NOTIFY: emitted signal isPlayingChanged(bool)
    // Свойство только для чтения, указывающее, воспроизводится ли видео в данный момент
    // READ: метод геттера isPlaying()
    // NOTIFY: выпущенный сигнал isPlayingChanged(bool)
    Q_PROPERTY(bool isPlaying READ isPlaying NOTIFY isPlayingChanged)

    // Read-only property indicating whether video is currently paused
    // READ: getter method isPaused()
    // NOTIFY: emitted signal isPausedChanged(bool)
    // Свойство только для чтения, указывающее, приостановлено ли видео в данный момент
    // READ: метод геттера isPaused()
    // NOTIFY: выпущенный сигнал isPausedChanged(bool)
    Q_PROPERTY(bool isPaused READ isPaused NOTIFY isPausedChanged)

public:
    // ========== CONSTRUCTOR ==========
    // Initialize VLCBridge with optional parent QObject for memory management
    // Инициализировать VLCBridge с опциональным родительским QObject для управления памятью
    explicit VLCBridge(QObject *parent = nullptr);

    // ========== Q_INVOKABLE METHODS ==========
    // These methods are callable from QML with automatic type conversion
    // Эти методы вызываются из QML с автоматическим преобразованием типа

    // Start RTSP stream playback at specified position and size
    // Parameters: url - stream URL, videoContainer - QML item for positioning,
    //             x, y - position in device-independent pixels,
    //             width, height - size in device-independent pixels
    // Начать воспроизведение RTSP потока в указанной позиции и размере
    // Параметры: url - URL потока, videoContainer - элемент QML для позиционирования,
    //            x, y - позиция в аппаратно-независимых пиксельях,
    //            width, height - размер в аппаратно-независимых пиксельях
    Q_INVOKABLE void play(const QString &url, QObject *videoContainer, qreal x, qreal y, qreal width, qreal height);

    // Update the position and size of the active video playback surface
    // This method is called when the video container is resized or moved
    // Parameters: same as play() except url (uses currently playing stream)
    // Обновить позицию и размер активной поверхности воспроизведения видео
    // Этот метод вызывается, когда контейнер видео изменяет размер или перемещается
    // Параметры: как play() кроме url (использует текущий воспроизводящийся поток)
    Q_INVOKABLE void updatePosition(QObject *videoContainer, qreal x, qreal y, qreal width, qreal height);

    // Pause the currently playing video stream
    // Приостановить текущий воспроизводящийся видео поток
    Q_INVOKABLE void pause();

    // Stop the currently playing or paused video stream and release resources
    // Остановить текущий воспроизводящийся или приостановленный видео поток и освободить ресурсы
    Q_INVOKABLE void stop();

    // ========== PROPERTY GETTER METHODS ==========
    // Inline getters for Q_PROPERTY read access
    // Встроенные геттеры для доступа для чтения Q_PROPERTY

    // Return the current playing state
    // Вернуть текущее состояние воспроизведения
    bool isPlaying() const;

    // Return the current paused state
    // Вернуть текущее состояние паузы
    bool isPaused() const;

    // ========== SIGNALS SECTION ==========
    // Signals are emitted to notify connected slots of state changes
    // Сигналы выпускаются для уведомления подключенных слотов об изменениях состояния
signals:
    // Emitted when the playing state changes (started or stopped playback)
    // Выпущено при изменении состояния воспроизведения (запуск или остановка воспроизведения)
    void isPlayingChanged(bool value);

    // Emitted when the paused state changes (paused or resumed)
    // Выпущено при изменении состояния паузы (пауза или возобновление)
    void isPausedChanged(bool value);

    // Emitted with status updates (e.g., "Playing", "Paused", "Stopped")
    // Allows QML to display current playback status to user
    // Выпущено с обновлениями состояния (например, "Playing", "Paused", "Stopped")
    // Позволяет QML отображать текущее состояние воспроизведения пользователю
    void statusChanged(const QString &status);

    // Emitted when errors occur (e.g., invalid Activity, JNI call failures)
    // Contains human-readable error message for logging or user notification
    // Выпущено при возникновении ошибок (например, недействительный Activity, сбои вызова JNI)
    // Содержит удобочитаемое сообщение об ошибке для логирования или уведомления пользователя
    void error(const QString &message);

private:
    // ========== PRIVATE HELPER METHOD ==========
    // Static method to convert device-independent pixels (DP) to physical pixels (PX)
    // based on the screen's device pixel ratio
    // Параметры: videoContainer - QML элемент для получения DPI, rectDp - прямоугольник в DP
    // Возвращает: QRectF с координатами, масштабированными по коэффициенту DPI
    // Статический метод для преобразования аппаратно-независимых пиксельей (DP) в физические пиксели (PX)
    // на основе коэффициента масштабирования пиксели устройства экрана
    // Parameters: videoContainer - QML item for obtaining DPI, rectDp - rectangle in DP
    // Returns: QRectF with coordinates scaled by DPI ratio
    static QRectF toPx(QObject *videoContainer, const QRectF &rectDp);

    // ========== MEMBER VARIABLES ==========
    // Track the current playback state to validate operations and emit signals
    // Отслеживать текущее состояние воспроизведения для проверки операций и выпуска сигналов

    // Flag indicating whether video playback is active
    // Флаг, указывающий, активно ли воспроизведение видео
    bool m_isPlaying = false;

    // Flag indicating whether video playback is currently paused
    // Флаг, указывающий, приостановлено ли воспроизведение видео в данный момент
    bool m_isPaused = false;
};

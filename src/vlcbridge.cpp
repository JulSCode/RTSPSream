#include "vlcbridge.h"
#include <QDebug>
#include <QQuickWindow>
#include <QTimer>

// Constructor: Initialize the VLCBridge object and log its creation
// Конструктор: Инициализировать объект VLCBridge и залогировать его создание
VLCBridge::VLCBridge(QObject *parent)
    : QObject(parent)
{
    qDebug("✅ VLCBridge constructed");
}

// Getter: Returns the current playing state
// Геттер: Возвращает текущее состояние воспроизведения
bool VLCBridge::isPlaying() const
{
    return m_isPlaying;
}

// Getter: Returns the current paused state
// Геттер: Возвращает текущее состояние паузы
bool VLCBridge::isPaused() const
{
    return m_isPaused;
}

// Convert device-independent pixels (DP) to physical pixels (PX)
// based on the screen's device pixel ratio (DPI scaling)
// Преобразовать аппаратно-независимые пиксели (DP) в физические пиксели (PX)
// на основе коэффициента масштабирования пиксели устройства (DPI масштабирование)
QRectF VLCBridge::toPx(QObject *videoContainer, const QRectF &rectDp)
{
    // Default DPI ratio is 1.0 (no scaling)
    // DPI коэффициент по умолчанию 1.0 (без масштабирования)
    qreal dpi = 1.0;

    // Try to get the device pixel ratio from the window's screen
    // Попытаться получить коэффициент пиксели устройства из экрана окна
    if (auto *item = qobject_cast<QQuickItem *>(videoContainer)) {
        if (item->window() && item->window()->screen())
            dpi = item->window()->screen()->devicePixelRatio();
    }

    // Return rectangle with all coordinates scaled by DPI ratio
    // Вернуть прямоугольник со всеми координатами, масштабированными по коэффициенту DPI
    return QRectF(rectDp.x() * dpi,
                  rectDp.y() * dpi,
                  rectDp.width() * dpi,
                  rectDp.height() * dpi);
}

// Start playback of RTSP stream at specified position and size
// Parameters: url - stream URL, videoContainer - QML item for DPI scaling,
//             x, y - position in DP, width, height - size in DP
// Начать воспроизведение RTSP потока в указанной позиции и размере
// Параметры: url - URL потока, videoContainer - элемент QML для масштабирования DPI,
//            x, y - позиция в DP, width, height - размер в DP
void VLCBridge::play(const QString &url, QObject *videoContainer, qreal x, qreal y, qreal width, qreal height)
{
#ifdef Q_OS_ANDROID
    // Get the current Android Activity from QtNative
    // Получить текущий объект Activity из QtNative
    QJniObject activity = QJniObject::callStaticObjectMethod(
        "org/qtproject/qt/android/QtNative", "activity", "()Landroid/app/Activity;");
    // Check if Activity was successfully retrieved
    // Проверить, был ли Activity успешно получен
    if (!activity.isValid()) {
        emit error("Failed to get Android activity");
        return;
    }

    // Convert QString to JNI string
    // Преобразовать QString в JNI строку
    QJniObject jUrl = QJniObject::fromString(url);

    // Convert DP coordinates to physical pixels using DPI scaling
    // Преобразовать DP координаты в физические пиксели с использованием DPI масштабирования
    auto rect = toPx(videoContainer, QRectF(x, y, width, height));

    // Call the native Java method to start VLC playback in a SurfaceView
    // JNI signature: (Activity, URL string, x, y, width, height) -> void
    // Вызвать нативный Java метод для запуска воспроизведения VLC в SurfaceView
    // JNI сигнатура: (Activity, URL строка, x, y, ширина, высота) -> void
    QJniObject::callStaticMethod<void>(
        "org/qtproject/example/vlc/VlcSurfaceHelper",
        "startPlaybackInSurface",
        "(Landroid/app/Activity;Ljava/lang/String;FFFF)V",
        activity.object<jobject>(),
        jUrl.object<jstring>(),
        static_cast<jfloat>(rect.x()),
        static_cast<jfloat>(rect.y()),
        static_cast<jfloat>(rect.width()),
        static_cast<jfloat>(rect.height())
        );

    // Update internal state to reflect that playback has started
    // Обновить внутреннее состояние, чтобы отразить, что воспроизведение началось
    m_isPlaying = true;
    m_isPaused = false;

    // Emit signals to notify QML layer of state changes
    // Отправить сигналы для уведомления QML слоя об изменениях состояния
    emit isPlayingChanged(m_isPlaying);
    emit isPausedChanged(m_isPaused);
    emit statusChanged("Playing");
#else
    // Mark unused parameters to avoid compiler warnings on non-Android platforms
    // Отметить неиспользуемые параметры, чтобы избежать предупреждений компилятора на платформах, отличных от Android
    Q_UNUSED(url) Q_UNUSED(videoContainer) Q_UNUSED(x) Q_UNUSED(y) Q_UNUSED(width) Q_UNUSED(height)
    emit error("Android only");
#endif
}

// Update the position and size of the currently playing video surface
// Parameters: x, y - new position in DP, width, height - new size in DP
// Обновить позицию и размер текущей поверхности воспроизведения видео
// Параметры: x, y - новая позиция в DP, width, height - новый размер в DP
void VLCBridge::updatePosition(QObject *videoContainer, qreal x, qreal y, qreal width, qreal height)
{
#ifdef Q_OS_ANDROID
    // Convert DP coordinates to physical pixels using DPI scaling
    // Преобразовать DP координаты в физические пиксели с использованием DPI масштабирования
    auto rect = toPx(videoContainer, QRectF(x, y, width, height));

    // Call the native Java method to update the video surface position
    // JNI signature: (x, y, width, height) -> void
    // Вызвать нативный Java метод для обновления позиции видеоповерхности
    // JNI сигнатура: (x, y, ширина, высота) -> void
    QJniObject::callStaticMethod<void>(
        "org/qtproject/example/vlc/VlcSurfaceHelper",
        "updateSurfacePosition",
        "(FFFF)V",
        static_cast<jfloat>(rect.x()),
        static_cast<jfloat>(rect.y()),
        static_cast<jfloat>(rect.width()),
        static_cast<jfloat>(rect.height())
        );
#else
    // Mark unused parameters to avoid compiler warnings on non-Android platforms
    // Отметить неиспользуемые параметры, чтобы избежать предупреждений компилятора на платформах, отличных от Android
    Q_UNUSED(x) Q_UNUSED(y) Q_UNUSED(width) Q_UNUSED(height)
#endif
}

// Pause the currently playing video stream
// Приостановить текущий воспроизводящийся видео поток
void VLCBridge::pause()
{
#ifdef Q_OS_ANDROID
    // Call the native Java method to pause VLC playback
    // JNI signature: () -> void (no parameters)
    // Вызвать нативный Java метод для паузы воспроизведения VLC
    // JNI сигнатура: () -> void (без параметров)
    QJniObject::callStaticMethod<void>(
        "org/qtproject/example/vlc/VlcSurfaceHelper",
        "pause",
        "()V");

    // Update internal state to reflect that playback is paused
    // Обновить внутреннее состояние, чтобы отразить, что воспроизведение приостановлено
    m_isPaused = true;
    m_isPlaying = false;

    // Emit signals to notify QML layer of state changes
    // Отправить сигналы для уведомления QML слоя об изменениях состояния
    emit isPausedChanged(m_isPaused);
    emit isPlayingChanged(m_isPlaying);
    emit statusChanged("Paused");
#else
    emit error("Android only");
#endif
}

// Stop the currently playing video stream and release resources
// Остановить текущий воспроизводящийся видео поток и освободить ресурсы
void VLCBridge::stop()
{
#ifdef Q_OS_ANDROID
    // Call the native Java method to stop VLC playback
    // JNI signature: () -> void (no parameters)
    // Вызвать нативный Java метод для остановки воспроизведения VLC
    // JNI сигнатура: () -> void (без параметров)
    QJniObject::callStaticMethod<void>(
        "org/qtproject/example/vlc/VlcSurfaceHelper",
        "stop",
        "()V");

    // Update internal state to reflect that playback has stopped
    // Обновить внутреннее состояние, чтобы отразить, что воспроизведение остановлено
    m_isPlaying = false;
    m_isPaused = false;

    // Emit signals to notify QML layer of state changes
    // Отправить сигналы для уведомления QML слоя об изменениях состояния
    emit isPlayingChanged(m_isPlaying);
    emit isPausedChanged(m_isPaused);
    emit statusChanged("Stopped");
#else
    emit error("Android only");
#endif
}

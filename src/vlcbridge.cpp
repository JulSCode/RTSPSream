#include "vlcbridge.h"
#include <QDebug>
#include <QQuickWindow>
#include <QTimer>

VLCBridge::VLCBridge(QObject *parent)
    : QObject(parent)
{
    qDebug("✅ VLCBridge constructed");
}

bool VLCBridge::isPlaying() const
{
    return m_isPlaying;
}

bool VLCBridge::isPaused() const
{
    return m_isPaused;
}

QRectF VLCBridge::toPx(QObject *videoContainer, const QRectF &rectDp)
{
    qreal dpi = 1.0;
    if (auto *item = qobject_cast<QQuickItem *>(videoContainer)) {
        if (item->window() && item->window()->screen())
            dpi = item->window()->screen()->devicePixelRatio();
    }
    return QRectF(rectDp.x() * dpi,
                  rectDp.y() * dpi,
                  rectDp.width() * dpi,
                  rectDp.height() * dpi);
}

void VLCBridge::play(const QString &url, QObject *videoContainer, qreal x, qreal y, qreal width, qreal height)
{
#ifdef Q_OS_ANDROID
    QJniObject activity = QJniObject::callStaticObjectMethod(
        "org/qtproject/qt/android/QtNative", "activity", "()Landroid/app/Activity;");
    if (!activity.isValid()) {
        emit error("Failed to get Android activity");
        return;
    }

    QJniObject jUrl = QJniObject::fromString(url);
    auto rect = toPx(videoContainer, QRectF(x, y, width, height));

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

    m_isPlaying = true;
    m_isPaused = false;
    emit isPlayingChanged(m_isPlaying);
    emit isPausedChanged(m_isPaused);
    emit statusChanged("Playing");
#else
    Q_UNUSED(url) Q_UNUSED(videoContainer) Q_UNUSED(x) Q_UNUSED(y) Q_UNUSED(width) Q_UNUSED(height)
    emit error("Android only");
#endif
}

void VLCBridge::updatePosition(QObject *videoContainer, qreal x, qreal y, qreal width, qreal height)
{
#ifdef Q_OS_ANDROID
    auto rect = toPx(videoContainer, QRectF(x, y, width, height));

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
    Q_UNUSED(x) Q_UNUSED(y) Q_UNUSED(width) Q_UNUSED(height)
#endif
}

void VLCBridge::pause()
{
#ifdef Q_OS_ANDROID
    QJniObject::callStaticMethod<void>(
        "org/qtproject/example/vlc/VlcSurfaceHelper",
        "pause",
        "()V");

    m_isPaused = true;
    m_isPlaying = false;
    emit isPausedChanged(m_isPaused);
    emit isPlayingChanged(m_isPlaying);
    emit statusChanged("Paused");
#else
    emit error("Android only");
#endif
}

void VLCBridge::stop()
{
#ifdef Q_OS_ANDROID
    QJniObject::callStaticMethod<void>(
        "org/qtproject/example/vlc/VlcSurfaceHelper",
        "stop",
        "()V");

    m_isPlaying = false;
    m_isPaused = false;
    emit isPlayingChanged(m_isPlaying);
    emit isPausedChanged(m_isPaused);
    emit statusChanged("Stopped");
#else
    emit error("Android only");
#endif
}

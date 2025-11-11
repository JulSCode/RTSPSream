#pragma once
#include <QObject>
#include <QString>
#include <QQuickItem>
#include <QJniObject>
#include <QJniEnvironment>

class VLCBridge : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool isPlaying READ isPlaying NOTIFY isPlayingChanged)
    Q_PROPERTY(bool isPaused READ isPaused NOTIFY isPausedChanged)

public:
    explicit VLCBridge(QObject *parent = nullptr);

    Q_INVOKABLE void play(const QString &url, QObject *videoContainer, qreal x, qreal y, qreal width, qreal height);
    Q_INVOKABLE void updatePosition(QObject *videoContainer, qreal x, qreal y, qreal width, qreal height);
    Q_INVOKABLE void pause();
    Q_INVOKABLE void stop();

    bool isPlaying() const;
    bool isPaused() const;

signals:
    void isPlayingChanged(bool value);
    void isPausedChanged(bool value);
    void statusChanged(const QString &status);
    void error(const QString &message);

private:
    static QRectF toPx(QObject *videoContainer, const QRectF &rectDp);
    bool m_isPlaying = false;
    bool m_isPaused = false;
};

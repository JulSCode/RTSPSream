#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <vlc/vlc.h>
#include "vlcbridge.h"
#include "androidhelper.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    AndroidHelper::hideNavigationBar();

    VLCBridge vlcBridge;

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("vlcBridge", &vlcBridge);

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("RTSPStream", "Main");

    return app.exec();
}

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <vlc/vlc.h>
#include "vlcbridge.h"
#include "androidhelper.h"

int main(int argc, char *argv[])
{
    // Create the main GUI application object that manages the application's control flow and settings
    // Создать основной объект GUI приложения, который управляет потоком управления и параметрами приложения
    QGuiApplication app(argc, argv);
    // Hide the Android system navigation bar (status bar and navigation buttons)
    // Скрыть системную навигационную панель Android (строка состояния и кнопки навигации)
    AndroidHelper::hideNavigationBar();

    // Create an instance of VLCBridge to handle VLC streaming functionality
    // Создать экземпляр VLCBridge для обработки функциональности потокового вещания VLC
    VLCBridge vlcBridge;

    // Create the QML engine that will load and manage the user interface
    // Создать движок QML, который будет загружать и управлять пользовательским интерфейсом
    QQmlApplicationEngine engine;
    // Register the vlcBridge object as a context property accessible from QML
    // This allows QML code to call methods on vlcBridge directly
    // Зарегистрировать объект vlcBridge как контекстное свойство, доступное из QML
    // Это позволяет коду QML вызывать методы vlcBridge напрямую
    engine.rootContext()->setContextProperty("vlcBridge", &vlcBridge);

    // Connect to the objectCreationFailed signal to handle QML loading errors
    // If QML fails to load, exit the application with error code -1
    // Подключиться к сигналу objectCreationFailed для обработки ошибок загрузки QML
    // Если QML не загрузится, выйти из приложения с кодом ошибки -1
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("RTSPStream", "Main");

    // Start the event loop and return the exit code when the application closes
    // Запустить цикл обработки событий и вернуть код выхода при закрытии приложения
    return app.exec();
}

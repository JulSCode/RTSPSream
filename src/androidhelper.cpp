#include "androidhelper.h"

void AndroidHelper::hideNavigationBar() {
    // Only compile and execute this code on Android platform
    // Запускать и выполнять этот код только на платформе Android
#ifdef Q_OS_ANDROID
    // Get the current Activity object from QtNative
    // Получить текущий объект Activity из QtNative
    QJniObject activity = QJniObject::callStaticObjectMethod(
        "org/qtproject/qt/android/QtNative",
        "activity",
        "()Landroid/app/Activity;"
        );

    // Check if the Activity object was successfully retrieved
    // Проверить, был ли объект Activity успешно получен
    if (!activity.isValid()) {
        qWarning() << "Failed to get Activity from QtNative";
        // Failed to retrieve Activity, print warning and exit the function
        // Не удалось получить Activity, вывести предупреждение и выйти из функции
        return;
    }

    // Call the hideSystemUI method from the native Java class
    // This hides the system UI elements including the navigation bar
    // Вызвать метод hideSystemUI из нативного Java класса
    // Это скрывает элементы системного интерфейса, включая навигационную панель
    QJniObject::callStaticMethod<void>(
        "org/qtproject/example/SystemUIHelper",
        "hideSystemUI",
        "(Landroid/app/Activity;)V",
        activity.object<jobject>()
        );

    // Log successful execution of the navigation bar hiding
    // Логировать успешное выполнение скрытия навигационной панели
    qDebug() << "Navigation bar hidden!";
#endif
}

void AndroidHelper::vibrate(int milliseconds) {
    // Only compile and execute this code on Android platform
    // Запускать и выполнять этот код только на платформе Android
#ifdef Q_OS_ANDROID
    // Get the current Activity object from QtNative
    // Получить текущий объект Activity из QtNative
    QJniObject activity = QJniObject::callStaticObjectMethod(
        "org/qtproject/qt/android/QtNative",
        "activity",
        "()Landroid/app/Activity;"
        );

    // Check if the Activity object was successfully retrieved
    // Проверить, был ли объект Activity успешно получен
    if (!activity.isValid()) {
        qWarning() << "Failed to get Activity from QtNative";
        // Failed to retrieve Activity, print warning and exit the function
        // Не удалось получить Activity, вывести предупреждение и выйти из функции
        return;
    }

    // Call the vibrate method from the native Java class with duration in milliseconds
    // The JNI signature indicates: void method, takes Activity and int (milliseconds) parameters
    // Вызвать метод vibrate из нативного Java класса с длительностью в миллисекундах
    // Сигнатура JNI указывает: метод void, принимает параметры Activity и int (миллисекунды)
    QJniObject::callStaticMethod<void>(
        "org/qtproject/example/SystemUIHelper",
        "vibrate",
        "(Landroid/app/Activity;I)V",
        activity.object<jobject>(),
        jint(milliseconds)
        );

    // Log successful call with the specified vibration duration
    // Логировать успешный вызов с указанной длительностью вибрации
    qDebug() << "Vibration called for" << milliseconds << "ms";
#endif
}

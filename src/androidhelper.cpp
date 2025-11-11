#include "androidhelper.h"

void AndroidHelper::hideNavigationBar() {
#ifdef Q_OS_ANDROID
    QJniObject activity = QJniObject::callStaticObjectMethod(
        "org/qtproject/qt/android/QtNative",
        "activity",
        "()Landroid/app/Activity;"
        );

    if (!activity.isValid()) {
        qWarning() << "Failed to get Activity from QtNative";
        return;
    }

    QJniObject::callStaticMethod<void>(
        "org/qtproject/example/SystemUIHelper",
        "hideSystemUI",
        "(Landroid/app/Activity;)V",
        activity.object<jobject>()
        );

    qDebug() << "Navigation bar hidden!";
#endif
}

void AndroidHelper::vibrate(int milliseconds) {
#ifdef Q_OS_ANDROID
    QJniObject activity = QJniObject::callStaticObjectMethod(
        "org/qtproject/qt/android/QtNative",
        "activity",
        "()Landroid/app/Activity;"
        );

    if (!activity.isValid()) {
        qWarning() << "Failed to get Activity from QtNative";
        return;
    }

    QJniObject::callStaticMethod<void>(
        "org/qtproject/example/SystemUIHelper",
        "vibrate",
        "(Landroid/app/Activity;I)V",
        activity.object<jobject>(),
        jint(milliseconds)
        );

    qDebug() << "Vibration called for" << milliseconds << "ms";
#endif
}


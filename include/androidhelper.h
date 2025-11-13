#ifndef ANDROIDHELPER_H
#define ANDROIDHELPER_H

#include <QJniObject>
#include <QDebug>

// AndroidHelper: Static utility class for Android system operations via JNI
// Provides methods to control Android system UI and device haptics
// AndroidHelper: Статический класс-утилита для операций системы Android через JNI
// Предоставляет методы для управления системным UI Android и тактильной обратной связью устройства
class AndroidHelper {
public:
    // All methods are static - no instance of AndroidHelper needs to be created
    // Все методы статические - не требуется создание экземпляра AndroidHelper

    // Hide the Android system navigation bar (bottom navigation buttons and gestures area)
    // This is commonly used for immersive full-screen applications like video players
    // Скрыть системную навигационную панель Android (кнопки нижней навигации и область жестов)
    // Обычно используется для полноэкранных приложений, погруженных в просмотр, например видеоплееры
    static void hideNavigationBar();

    // Trigger device vibration for haptic feedback
    // Parameter: milliseconds - duration of vibration in milliseconds (e.g., 100 for 100ms pulse)
    // Вызвать вибрацию устройства для тактильной обратной связи
    // Параметр: milliseconds - длительность вибрации в миллисекундах (например, 100 для импульса 100мс)
    static void vibrate(int milliseconds);
};

#endif // ANDROIDHELPER_H

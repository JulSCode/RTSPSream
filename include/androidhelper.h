#ifndef ANDROIDHELPER_H
#define ANDROIDHELPER_H

#include <QJniObject>
#include <QDebug>

class AndroidHelper {
public:
    static void hideNavigationBar();
    static void vibrate(int milliseconds);
};

#endif // ANDROIDHELPER_H

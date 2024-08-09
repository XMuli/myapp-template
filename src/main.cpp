#include <QApplication>
#include "ui/main/mainwin.h"
#include "ui/setting/settingui.h"
#include "ui/tray/tray.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    TRAY;
    // MainWin mainW;
    // mainW.show();

    // SettingUI w;
    // w.show();
    return a.exec();
}

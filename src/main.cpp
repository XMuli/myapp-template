#include <QApplication>
#include "ui/main/mainwin.h"
#include "ui/setting/settingui.h"
#include "ui/tray/tray.h"
#include "communication.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    COMM.loadTranslation("简体中文");
    TRAY;
    TRAY.openWindow();
    return a.exec();
}

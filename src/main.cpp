#include <QApplication>
#include "ui/main/mainwin.h"
#include "ui/setting/settingui.h"
#include "ui/tray/tray.h"
#include "communication.h"
#include "configjson.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    CJ.defaultConfigJson();

#if defined (Q_OS_LINUX)
    CJ.initPrepare();
#endif

    CJ.readFromFile();
    CJ.writeToFile(); // 写入完整的 m_j
    CJ.initOutputDefaulValue(); // 完整的 m_j = 读取 .josn + 填充为空 path 的一些路径

    COMM.loadTranslation("");
    TRAY;
    TRAY.openWindow();
    return a.exec();
}

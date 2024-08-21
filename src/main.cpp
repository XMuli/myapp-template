#include <QApplication>
#include "ui/main/mainwin.h"
#include "ui/setting/settingui.h"
#include "ui/tray/tray.h"
#include "communication.h"
#include "configjson.h"
#include "basics/xapphelper.h"

#include <QSystemSemaphore>
int main(int argc, char *argv[])
{
    qApp->setAttribute(Qt::AA_UseHighDpiPixmaps);
    qApp->setOrganizationName(QStringLiteral("XMuli"));                  // 因多处使用 QSettings，故声明组织等信息
    qApp->setOrganizationDomain(QStringLiteral("github.com/XMuli"));
    QApplication a(argc, argv);
    a.setQuitOnLastWindowClosed(false); // fix: 默认情况下，当关闭最后一个窗口时，Qt 应用程序会自动退出

    QSharedMemory sharedMemory;
    QSystemSemaphore systemSemaphore(szUniqueKey, 1, QSystemSemaphore::Open);
    detectionSingApp(sharedMemory, systemSemaphore);
#if defined (Q_OS_LINUX)
    CJ.initPrepare();
#endif

    CJ.readFromFile();
    CJ.writeToFile();           // 写入完整的 m_j
    CJ.initOutputDefaulValue(); // 完整的 m_j = 读取 .josn + 填充为空 path 的一些路径

    COMM.loadTranslation("");
    TRAY;
    TRAY.openWindow();

    systemSemaphore.release();
    return a.exec();
}

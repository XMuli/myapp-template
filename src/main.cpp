// SPDX-FileCopyrightText: 2023-2024 XMuli
// SPDX-GitHub: https://github.com/XMuli
// SPDX-Author: XMuli <xmulitech@gmail.com>
#include <QApplication>
#include <QSystemSemaphore>
#include "ui/main/mainwin.h"
#include "ui/setting/settingui.h"
#include "ui/tray/tray.h"
#include "communication.h"
#include "configjson.h"
#include "basics/xapphelper.h"

int main(int argc, char *argv[])
{

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    qDebug() << "Using Qt 6 or newer";
    // qputenv("QT_SCALE_FACTOR", "1");              // 全局设置缩放比例
    // qputenv("QT_ENABLE_HIGHDPI_SCALING", "0");
    // qputenv("QT_ENABLE_HIGHDPI_SCALING", "1");       // 0-关闭，恢复为qt5的效果（对于截图程序，推荐此）； 1 开启，使用qt6的效果
    // qputenv("QT_SCREEN_SCALE_FACTORS", "1.5;2");  // 针对多个屏幕设置
    // QApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough); // support 1.25 1.5
#else
    qDebug() << "Using Qt 5";
    // 启用高 DPI 缩放
    // QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

#endif

    qApp->setOrganizationName(QStringLiteral("XMuli"));                 // 因多处使用 QSettings，故声明组织等信息
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
    TRAY.onMainWin();
    TRAY.onSetting();

    systemSemaphore.release();
    return a.exec();
}

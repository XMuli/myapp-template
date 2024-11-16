#include "tray.h"
#include <QAction>
#include <QCoreApplication>
#include <QGuiApplication>
#include <QIcon>
#include <QKeySequence>
#include <QShortcut>
#include <QTimer>
#include <QFont>
#include <QProcess>
#include <QDebug>
#include <QScreen>
#include "communication.h"
#include "configjson.h"

void Tray::setAppFont(const QString &tFont)
{
    QStringList list = tFont.isEmpty() ? CJ_GET_QSTR("general.font").split(",") : tFont.split(",");
    if (list.size() < 2) {
        list .clear();

#if defined(Q_OS_WIN)
        list << "Microsoft YaHei" << "9";
#elif defined(Q_OS_LINUX)
        list << "WenQuanYi Micro Hei" << "9";
#elif defined(Q_OS_MAC)
        list << "PingFang SC" << "11";
#endif

    }
    const QFont font(list.at(0), list.at(1).toInt());
    qApp->setFont(font);
    m_trayMenu->setFont(font);
}

void Tray::initUI()
{
    setAppFont("");
    QAction* function1 = new QAction(this);
    QAction* function2 = new QAction(this);
    QAction* mainWin = new QAction(this);
    QAction* setting_tabpage = new QAction(this);
    QAction* setting_nav = new QAction(this);

    QAction* restart = new QAction(this);
    QAction* quit    = new QAction(this);
    function1->setObjectName("act_function1");
    function2->setObjectName("act_function2");
    mainWin->setObjectName("actMainWin");
    setting_tabpage->setObjectName("actSettingTab");
    setting_nav->setObjectName("actSettingNav");
    restart->setObjectName("actRestart");
    quit->setObjectName("actQuit");

    m_trayMenu->addAction(function1);
    m_trayMenu->addAction(function2);
    m_trayMenu->addSeparator();
    m_trayMenu->addAction(mainWin);
    m_trayMenu->addAction(setting_tabpage);
    m_trayMenu->addAction(setting_nav);
    m_trayMenu->addSeparator();
    m_trayMenu->addAction(restart);
    m_trayMenu->addAction(quit);

    QString suffix = ".svg";
#ifdef Q_OS_LINUX
    suffix = ".png";
#endif

    m_trayIcon->setIcon(QIcon(":/logo/logo" + suffix));
    m_trayIcon->setContextMenu(m_trayMenu);

    onLanguageChange("");
    m_trayIcon->show();

    connect(m_trayIcon, &QSystemTrayIcon::activated, this, &Tray::onTrayIcon);
    connect(function1, &QAction::triggered, this, &Tray::onFunction1);
    connect(mainWin, &QAction::triggered, this, &Tray::onMainWin);
    connect(setting_tabpage, &QAction::triggered, this, &Tray::onSettingTab);
    connect(setting_nav, &QAction::triggered, this, &Tray::onSettingNav);
    connect(restart, &QAction::triggered, this, &Tray::onRestart);
    connect(quit, &QAction::triggered, [](){ qApp->quit(); });
    connect(&COMM, &Communication::sigLanguageChange, this, [this]() { onLanguageChange("");});
    connect(&COMM, &Communication::sigShowSystemMessagebox, this, &Tray::onShowSystemMessagebox);

#ifdef Q_OS_WIN  // Ensure proper removal of tray icon when program quits on Windows.
    connect(qApp, &QCoreApplication::aboutToQuit, m_trayIcon, &QSystemTrayIcon::hide);
#endif

    if (CJ_GET("about.first_run").get<bool>()) {
        emit COMM.sigShowSystemMessagebox(tr("%1 is runing").arg(XPROJECT_NAME), tr("You can control it via the tray icon."), 10 * 1000);
        CJ_SET("about.first_run", false);
    }

    // 1 分钟后开始自动检测新版本
    QTimer::singleShot(1 * 30 * 1000, this, []() {
        if (COMM.verUpdate()->userAllowCheckUpdate()) {
            COMM.verUpdate()->checkForUpdate();
        }
    });
}

void Tray::onFunction1()
{
    qDebug() << "Run onFunction1().";
}

void Tray::onMainWin()
{
    if (!m_mainWin) m_mainWin = new MainWin();
    m_mainWin->show();

    // 获取鼠标所在屏幕的几何信息
    QScreen *screen = QGuiApplication::screenAt(QCursor::pos());
    if (screen)
    {
        QRect screenGeometry = screen->geometry();
        QSize windowSize = m_mainWin->size();
        int x = screenGeometry.width() / 3 - windowSize.width() / 2;
        int y = screenGeometry.height() / 2 - windowSize.height() / 2;
        m_mainWin->move(x, y);
    }
}

void Tray::onSettingTab()
{
    if (!m_settingTab) m_settingTab = new SettingUI();
    // m_settingTab->adjustSize();
    m_settingTab->show();

    // 获取鼠标所在屏幕的几何信息
    QScreen *screen = QGuiApplication::screenAt(QCursor::pos());
    if (screen)
    {
        QRect screenGeometry = screen->geometry();
        QSize windowSize = m_settingTab->size();
        int x = screenGeometry.width() * 2 / 3 - windowSize.width() / 2;
        int y = screenGeometry.height() / 2 - windowSize.height() / 2;
        m_settingTab->move(x, y);
    }
}

void Tray::onSettingNav()
{
    if (!m_settingNav) m_settingNav = new SettingNavUI();
    m_settingNav->show();

    // 获取鼠标所在屏幕的几何信息
    QScreen *screen = QGuiApplication::screenAt(QCursor::pos());
    if (screen)
    {
        QRect screenGeometry = screen->geometry();
        QSize windowSize = m_settingNav->size();
        int x = screenGeometry.width() * 2 / 3 - windowSize.width() / 2;
        int y = screenGeometry.height() / 2 - windowSize.height() / 2;
        m_settingNav->move(x, y);
    }
}

void Tray::onRestart()
{
    const QString& path = qApp->applicationFilePath();
    QProcess::startDetached(path, QStringList());
    QCoreApplication::quit();
}

void Tray::onShowSystemMessagebox(const QString &title, const QString &msg, const int& msecs)
{
    m_trayIcon->showMessage(title, msg, QIcon(":/resources/logo/logo.svg"), msecs);
    m_trayIcon->showMessage(title, msg, QIcon(":/logo/logo.svg"), msecs);
}

void Tray::onTrayIcon(QSystemTrayIcon::ActivationReason reason)
{
    if (QSystemTrayIcon::Trigger == reason) {  //  鼠标单击
        qDebug() << "this mouse Trigger on tray.";
    }
}

void Tray::onLanguageChange(const QString qmName)
{
    Q_UNUSED(qmName)
    QAction * act_function1 = findChild<QAction*>("act_function1");
    QAction * act_function2 = findChild<QAction*>("act_function2");
    QAction * actMainWin = findChild<QAction*>("actMainWin");
    QAction * actSettingTab = findChild<QAction*>("actSettingTab");
    QAction * actSettingNav = findChild<QAction*>("actSettingNav");
    QAction * actRestart = findChild<QAction*>("actRestart");
    QAction * actQuit = findChild<QAction*>("actQuit");

    m_trayIcon->setToolTip(tr("this is my app."));
    if (act_function1) act_function1->setText(tr("function1 empty"));
    if (act_function2) act_function2->setText(tr("function2 empty"));
    if (actMainWin) actMainWin->setText(tr("Open Main Windows"));
    if (actSettingTab) actSettingTab->setText(tr("Setting (tabpage)"));
    if (actSettingNav) actSettingNav->setText(tr("Setting (navigation)"));
    if (actRestart) actRestart->setText(tr("Restart"));
    if (actQuit) actQuit->setText(tr("Quit"));
}

Tray::Tray(QObject *parent)
    : QObject(parent)
    , m_trayMenu(new QMenu())
    , m_trayIcon(new QSystemTrayIcon(this))
    , m_settingTab(nullptr)
    , m_settingNav(nullptr)
    , m_mainWin(nullptr)
{
    initUI();
}

Tray::~Tray()
{
    if (m_trayMenu) m_trayMenu->deleteLater();
    if (m_trayIcon) m_trayIcon->deleteLater();
    if (m_settingTab) m_settingTab->deleteLater();
    if (m_settingNav) m_settingNav->deleteLater();
    if (m_mainWin) m_mainWin->deleteLater();
    CJ.onSyncToFile();
}

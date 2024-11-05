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
#include "communication.h"
#include "configjson.h"

void Tray::openWindow()
{
    // onMainWin();
    onSetting();
}

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
    QAction* setting = new QAction(this);
    QAction* restart = new QAction(this);
    QAction* quit    = new QAction(this);
    function1->setObjectName("act_function1");
    function2->setObjectName("act_function2");
    mainWin->setObjectName("actMainWin");
    setting->setObjectName("actSetting");
    restart->setObjectName("actRestart");
    quit->setObjectName("actQuit");

    m_trayMenu->addAction(function1);
    m_trayMenu->addAction(function2);
    m_trayMenu->addAction(mainWin);
    m_trayMenu->addAction(setting);
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
    connect(setting, &QAction::triggered, this, &Tray::onSetting);
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
    // QTimer::singleShot(1 * 1 * 1000, this, []() {
    //     if (COMM.verUpdate()->userAllowCheckUpdate()) {
    //         COMM.verUpdate()->checkForUpdate();
    //     }
    // });
}

void Tray::onFunction1()
{
    qDebug() << "Run onFunction1().";
}

void Tray::onMainWin()
{
    if (!m_mainWin) m_mainWin = new MainWin();
    m_mainWin->show();
}

void Tray::onSetting()
{
    if (!m_setting) m_setting = new SettingNavUI();
    // m_setting->adjustSize();
    m_setting->show();
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
    QAction * actSetting = findChild<QAction*>("actSetting");
    QAction * actRestart = findChild<QAction*>("actRestart");
    QAction * actQuit = findChild<QAction*>("actQuit");

    m_trayIcon->setToolTip(tr("this is my app."));
    if (act_function1) act_function1->setText(tr("function1"));
    if (act_function2) act_function2->setText(tr("function2"));
    if (actMainWin) actMainWin->setText(tr("Open Main Windows"));
    if (actSetting) actSetting->setText(tr("Setting"));
    if (actRestart) actRestart->setText(tr("Restart"));
    if (actQuit) actQuit->setText(tr("Quit"));
}

Tray::Tray(QObject *parent)
    : QObject(parent)
    , m_trayMenu(new QMenu())
    , m_trayIcon(new QSystemTrayIcon(this))
    , m_setting(nullptr)
    , m_mainWin(nullptr)
{
    initUI();
}

Tray::~Tray()
{
    if (m_trayMenu) m_trayMenu->deleteLater();
    if (m_trayIcon) m_trayIcon->deleteLater();
    if (m_setting) m_setting->deleteLater();
    if (m_mainWin) m_mainWin->deleteLater();
    CJ.onSyncToFile();
}

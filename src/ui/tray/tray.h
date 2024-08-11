#ifndef TRAY_H
#define TRAY_H

#include <QMenu>
#include <QObject>
#include <QPointer>
#include <QSystemTrayIcon>
#include "../setting/settingui.h"
#include "../main/mainwin.h"


#define TRAY Tray::instance()

class Tray : public QObject
{
    Q_OBJECT

public:
    void openWindow();
    void setAppFont(const QString& tFont);

private:
    void initUI();

public slots:
    void onFunction1();
    void onMainWin();
    void onSetting();
    void onRestart();
    void onShowSystemMessagebox(const QString &title, const QString &msg, const int &msecs = 6000); // 系统消息通知
    void onTrayIcon(QSystemTrayIcon::ActivationReason reason);
    void onLanguageChange(const QString qmName);

public:
    static Tray &instance() {
        static Tray instance;
        return instance;
    };

protected:
    explicit Tray(QObject *parent = nullptr);
    virtual ~Tray();

    Tray(const Tray&) = delete;
    Tray& operator=(const Tray&) = delete;
    Tray(Tray&&) = delete;
    Tray& operator=(Tray&&) = delete;

private:
    QPointer<QMenu>                  m_trayMenu;
    QPointer<QSystemTrayIcon>        m_trayIcon;
    QPointer<SettingUI>              m_setting;
    QPointer<MainWin>                m_mainWin;
};

#endif // TRAY_H

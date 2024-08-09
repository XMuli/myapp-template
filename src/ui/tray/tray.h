#ifndef TRAY_H
#define TRAY_H

#include <QMenu>
#include <QObject>
#include <QPointer>
#include <QSystemTrayIcon>
#include "../setting/settingui.h"

#define TRAY Tray::instance()

class Tray : public QObject
{
    Q_OBJECT

private:
    void initUI();

public slots:
    void onFunction1();
    void onRestart();
    void onShowSystemMessagebox(const QString &title, const QString &msg, const int &msecs = 6000); // 系统消息通知
    void onTrayIcon(QSystemTrayIcon::ActivationReason reason);
    void onLanguageChange(const QString qmName);
    void onSetting();

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
};

#endif // TRAY_H

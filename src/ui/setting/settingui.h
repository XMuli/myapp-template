#ifndef SETTINGUI_H
#define SETTINGUI_H

#include <QWidget>
#include <QBoxLayout>
#include <QPointer>
#include "../../network/version_updater/versionupdater.h"

namespace Ui {
class SettingUI;
class SubGeneral;
class SubUpdate;
class SubAbout;
}

class SettingUI : public QWidget
{
    Q_OBJECT

public:
    explicit SettingUI(QWidget *parent = nullptr);
    ~SettingUI();

private slots:
    // sub_general.ui
    void onLanguageChanged(const QString &arg1);
    void onFontChanged();
    void onAutostart(bool checked);
    // sub_update.ui
    void onAutoCheckUpdateClicked(bool checked);
    void onDayValueChanged(int arg1);
    void onJoinInsiderProgramToggled(bool checked);

    void onSetProxy();
    void onTestReleased();
    void onCheckUpdateReleased();
    // sub_about.ui
    void onLicensesRelease();

private:
    void initUI();
    void setServerIPStatus();

protected:
    void closeEvent(QCloseEvent *e) override;

private:
    Ui::SettingUI*   ui;
    Ui::SubGeneral*  uiSubGeneral;
    Ui::SubUpdate*   uiSubUpdate;
    Ui::SubAbout*    uiSubAbout;

    QPointer<VersionUpdater>  m_verUpdate;
};

#endif // SETTINGUI_H

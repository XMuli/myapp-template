#ifndef SETTINGUI_H
#define SETTINGUI_H

#include <QWidget>
#include <qboxlayout.h>

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
    void onLanguageChanged(const QString &arg1);
    void onFontChanged();
    void onAutostart(bool checked);

private:
    void initUI();

protected:
    void closeEvent(QCloseEvent *e) override;

private:
    Ui::SettingUI  *ui;
    Ui::SubGeneral *uiSubGeneral;
    Ui::SubUpdate  *uiSubUpdate;
    Ui::SubAbout   *uiSubAbout;

};

#endif // SETTINGUI_H

#ifndef SETTINGUI_H
#define SETTINGUI_H

#include <QWidget>

namespace Ui {
class SettingUI;
class SubGeneral;
}

class SettingUI : public QWidget
{
    Q_OBJECT

public:
    explicit SettingUI(QWidget *parent = nullptr);
    ~SettingUI();

private:
    void initUI();

private:
    Ui::SettingUI *ui;
    Ui::SubGeneral *uiSubGeneral;
};

#endif // SETTINGUI_H

#ifndef SETTINGNAVUI_H
#define SETTINGNAVUI_H

#include <QTreeWidgetItem>
#include <QWidget>

namespace Ui {
class SettingNavUI;
class SubGeneral;
class SubUpdate;
class SubAbout;
}

class SettingNavUI : public QWidget
{
    Q_OBJECT

public:
    explicit SettingNavUI(QWidget *parent = nullptr);
    ~SettingNavUI();

public slots:
    void onItemClicked(QTreeWidgetItem *item, int column);

private:
    void initUI();

private:
    Ui::SettingNavUI *ui;
    Ui::SubGeneral*  uiSubGeneral;
    Ui::SubUpdate*   uiSubUpdate;
    Ui::SubAbout*    uiSubAbout;
};

#endif // SETTINGNAVUI_H

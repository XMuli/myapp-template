#include "settingui.h"
#include "ui_settingui.h"
#include "ui_sub_general.h"
#include "ui_sub_update.h"
#include "ui_sub_about.h"
#include <QVBoxLayout>

SettingUI::SettingUI(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SettingUI)
    , uiSubGeneral(new Ui::SubGeneral)
    , uiSubUpdate(new Ui::SubUpdate)
    , uiSubAbout(new Ui::SubAbout)
{
    ui->setupUi(this);

    initUI();
}

SettingUI::~SettingUI()
{
    delete ui;
    delete uiSubGeneral;
    delete uiSubUpdate;
    delete uiSubAbout;
}

void SettingUI::initUI()
{
    // 单独的 .ui 添加到 TabWidget 子对象中
    uiSubGeneral->setupUi(ui->generalDlg);
    uiSubUpdate->setupUi(ui->updateDlg);
    uiSubAbout->setupUi(ui->aboutDlg);


    // ui->labProject->setText(QString("%1").arg(XPROJECT_NAME));
    // ui->labBit->setText(tr("%1-bit").arg(XARCH_BIT));
    // ui->labVersion->setText(tr("Version %1 (%2) - %3 %4").arg(XPROJECT_VERSION).arg(XBUILD_TIME).arg(XCOMPILER).arg(XCOMPILER_ID));


}

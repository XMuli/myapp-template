#include "settingui.h"
#include "ui_settingui.h"

SettingUI::SettingUI(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SettingUI)
{
    ui->setupUi(this);

    initUI();
}

SettingUI::~SettingUI()
{
    delete ui;
}

void SettingUI::initUI()
{
    ui->labProject->setText(QString("%1").arg(XPROJECT_NAME));
    ui->labBit->setText(tr("%1-bit").arg(XARCH_BIT));
    ui->labVersion->setText(tr("Version %1 (%2) - %3 %4").arg(XPROJECT_VERSION).arg(XBUILD_TIME).arg(XCOMPILER).arg(XCOMPILER_ID));
}

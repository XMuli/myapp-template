#include "settingui.h"
#include "ui_settingui.h"

SettingUI::SettingUI(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SettingUI)
{
    ui->setupUi(this);
}

SettingUI::~SettingUI()
{
    delete ui;
}

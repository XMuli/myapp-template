// SPDX-FileCopyrightText: 2023-2024 XMuli
// SPDX-GitHub: https://github.com/XMuli
// SPDX-Author: XMuli <xmulitech@gmail.com>

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

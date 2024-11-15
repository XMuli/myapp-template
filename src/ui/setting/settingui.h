// SPDX-FileCopyrightText: 2023-2024 XMuli
// SPDX-GitHub: https://github.com/XMuli
// SPDX-Author: XMuli <xmulitech@gmail.com>

#ifndef SETTINGUI_H
#define SETTINGUI_H

#include <QWidget>

namespace Ui {
class SettingUI;
}

class SettingUI : public QWidget
{
    Q_OBJECT

public:
    explicit SettingUI(QWidget *parent = nullptr);
    ~SettingUI();

private:
    Ui::SettingUI *ui;
};

#endif // SETTINGUI_H

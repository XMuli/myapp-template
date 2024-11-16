#include "mainwin.h"
#include "ui_mainwin.h"
#include <QString>
#include "../tray/tray.h"

MainWin::MainWin(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWin)
{
    ui->setupUi(this);

    initUI();
}

MainWin::~MainWin()
{
    delete ui;
}

void MainWin::initUI()
{
    QString text = QString("project:          %1\n"
                           "version:          %2\n"
                           "arch_bit:         %3\n"
                           "compiler:         %4\n"
                           "compiler ID:      %5\n"
                           "Qt version:       %6\n"
                           "Qt install path:  %7\n\n"
                           "Author:           XMuli\n"
                           "Site:             https://myapp-template.xmuli.tech\n"
                           "GitHub:           https://github.com/XMuli/myapp-template\n")
                       .arg(XPROJECT_NAME, -20)  // 设置最小宽度为20，左对齐
                       .arg(XPROJECT_VERSION, -20)
                       .arg(XARCH_BIT, -20)
                       .arg(XCOMPILER, -20)
                       .arg(XCOMPILER_ID, -20)
                       .arg(QT_VERSION_STR, -20)
                       .arg(XQT_INSTALL_PATH, -20);


    ui->plainTextEdit->setFont(QFont("Courier New"));  // 设置等宽字体
    ui->plainTextEdit->appendPlainText(text);
    setWindowTitle(tr("My app template by Qt %1").arg(QT_VERSION_STR));
}

void MainWin::on_btn_setting_tab_released()
{
    TRAY.onSettingTab();
}


void MainWin::on_btn_setting_nav_released()
{
    TRAY.onSettingNav();
}


void MainWin::on_btn_exit_released()
{
    close();
}


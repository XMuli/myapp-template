#include "mainwin.h"
#include "ui_mainwin.h"
#include <QString>

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

void MainWin::on_pushButton_released()
{

}

void MainWin::initUI()
{
    QString text = QString("project:          %1\n"
                           "version:          %2\n"
                           "arch_bit:         %3\n"
                           "compiler:         %4\n"
                           "compiler ID:      %5\n"
                           "Qt version:       %6\n"
                           "Qt install path:  %7\n")
                       .arg(XPROJECT_NAME)
                       .arg(XPROJECT_VERSION)
                       .arg(XARCH_BIT)
                       .arg(XCOMPILER)
                       .arg(XCOMPILER_ID)
                       .arg(QT_VERSION_STR)
                       .arg(XQT_INSTALL_PATH);

    ui->plainTextEdit->appendPlainText(text);
    setWindowTitle(tr("My app template by Qt %1").arg(QT_VERSION_STR));
}


#include "widget.h"
#include "./ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    initUI();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::initUI()
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
                       .arg(QT_VERSION_MAJOR)
                       .arg(XQT_INSTALL_PATH);

    ui->plainTextEdit->appendPlainText(text);
}

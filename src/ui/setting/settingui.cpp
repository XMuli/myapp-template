#include "settingui.h"
#include "ui_settingui.h"
#include "ui_sub_general.h"
#include <QVBoxLayout>

SettingUI::SettingUI(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SettingUI)
    , uiSubGeneral(new Ui::SubGeneral)
{
    ui->setupUi(this);

    initUI();
}

SettingUI::~SettingUI()
{
    delete ui;
    delete uiSubGeneral;
}

void SettingUI::initUI()
{
    ui->labProject->setText(QString("%1").arg(XPROJECT_NAME));
    ui->labBit->setText(tr("%1-bit").arg(XARCH_BIT));
    ui->labVersion->setText(tr("Version %1 (%2) - %3 %4").arg(XPROJECT_VERSION).arg(XBUILD_TIME).arg(XCOMPILER).arg(XCOMPILER_ID));

    // 创建子 widget 并将其放入 uiSubGeneral 对象中
    QWidget *subGeneralWidget = new QWidget(this);
    uiSubGeneral->setupUi(subGeneralWidget);


    // 将 subGeneralWidget 作为 tabWidget 的子 widget 添加
    // ui->tabWidget->addTab(subGeneralWidget, tr("Sub General"));

    QVBoxLayout * vLayout = new QVBoxLayout();
    ui->tab_4->setLayout(vLayout);
    vLayout->addWidget(subGeneralWidget);
}

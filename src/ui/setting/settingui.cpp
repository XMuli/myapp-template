#include "settingui.h"
#include "ui_settingui.h"
#include "ui_sub_general.h"
#include "ui_sub_update.h"
#include "ui_sub_about.h"
#include <QVBoxLayout>
#include "communication.h"

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

    const auto& currLanguage = "简体中文"/*CJ_GET_QSTR("general.language")*/;
    const auto& languages = languageMap();
    for (const auto& it : languages) uiSubGeneral->cbbLanguage->addItem(it.first, it.second);
    uiSubGeneral->cbbLanguage->setCurrentText(currLanguage);

    uiSubAbout->labProject->setText(QString("%1").arg(XPROJECT_NAME));
    uiSubAbout->labBit->setText(tr("%1-bit").arg(XARCH_BIT));
    uiSubAbout->labVersion->setText(tr("Version %1 (%2) - %3 %4").arg(XPROJECT_VERSION).arg(XBUILD_TIME).arg(XCOMPILER).arg(XCOMPILER_ID));


    connect(uiSubGeneral->cbbLanguage, &QComboBox::currentTextChanged, this, &SettingUI::onLanguageChanged);



}

void SettingUI::onLanguageChanged(const QString &arg1)
{
    COMM.loadTranslation(arg1);
    ui->retranslateUi(this);

    // ui->btnFont->setText(CJ_GET_QSTR("general.font")); // fix: 切换语言后会被刷新掉
    // CJ_SET("general.language", arg1.toStdString());
}


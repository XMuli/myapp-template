#include "settingui.h"
#include "ui_settingui.h"
#include "ui_sub_general.h"
#include "ui_sub_update.h"
#include "ui_sub_about.h"
#include <QDir>
#include <QFontDialog>
#include <QSettings>
#include <QVBoxLayout>
#include "communication.h"
#include "configjson.h"

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

    const auto& currLanguage = CJ_GET_QSTR("general.language");
    const auto& languages = languageMap();
    for (const auto& it : languages) uiSubGeneral->cbbLanguage->addItem(it.first, it.second);
    uiSubGeneral->cbbLanguage->setCurrentText(currLanguage);
    uiSubGeneral->btnFont->setText(CJ_GET_QSTR("general.font"));
    uiSubGeneral->cbAutostart->setChecked(CJ_GET("general.autostart").get<bool>());
    uiSubGeneral->btnFont->resize(uiSubGeneral->cbbLanguage->size());

#if defined(Q_OS_MACOS)
    uiSubGeneral->cbAutostart->hide();
#endif

    uiSubAbout->labProject->setText(QString("%1").arg(XPROJECT_NAME));
    uiSubAbout->labBit->setText(tr("%1-bit").arg(XARCH_BIT));
    uiSubAbout->labVersion->setText(tr("Version %1 (%2) - %3 %4").arg(XPROJECT_VERSION).arg(XBUILD_TIME).arg(XCOMPILER).arg(XCOMPILER_ID));

    connect(uiSubGeneral->cbbLanguage, &QComboBox::currentTextChanged, this, &SettingUI::onLanguageChanged);
    connect(uiSubGeneral->btnFont, &QPushButton::released, this, &SettingUI::onFontChanged);
    connect(uiSubGeneral->cbAutostart, &QCheckBox::clicked, this, &SettingUI::onAutostart);
}

void SettingUI::closeEvent(QCloseEvent *e)
{
    CJ.onSyncToFile();
    QWidget::closeEvent(e);
}

void SettingUI::onLanguageChanged(const QString &arg1)
{
    COMM.loadTranslation(arg1);
    ui->retranslateUi(this);

    uiSubGeneral->btnFont->setText(CJ_GET_QSTR("general.font")); // fix: 切换语言后会被刷新掉
    CJ_SET("general.language", arg1.toStdString());
}

void SettingUI::onFontChanged()
{
    bool ok = false;
    const auto fontFamily = uiSubGeneral->btnFont->text();
    QFont font = QFontDialog::getFont(&ok, QFont(fontFamily.split(',').at(0)), this, tr("Select Font"));
    if (!ok) return;

    QString text = QString("%1,%2").arg(font.family()).arg(font.pointSize());
    COMM.setAppFont(text);
    uiSubGeneral->btnFont->setText(text);

    CJ_SET("general.font", text.toStdString());
    qDebug("当前选择的字体是[%s]-是否加粗[%d]-是否倾斜[%d]-字号[%d]", font.family().toUtf8().data(), font.bold(), font.italic(), font.pointSize());
}

void SettingUI::onAutostart(bool checked)
{
#if defined(Q_OS_WIN)
    QSettings reg("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
    if (checked) {
        QString strAppPath = QDir::toNativeSeparators(qApp->applicationFilePath());
        strAppPath.replace(QChar('/'), QChar('\\'), Qt::CaseInsensitive);
        reg.setValue(XPROJECT_NAME, strAppPath);
    } else {
        reg.setValue(XPROJECT_NAME, "");
    }
#elif defined (Q_OS_LINUX)
    // *.desktop 复制到 "~/.config/autostart/" 目录下实现开机自启动
    const QString desktopDir = "/usr/share/applications/tech.xmuli.sunny.desktop";
    const QString targetDir = QDir::homePath() + "/.config/autostart/";
    const QString targetFile = targetDir + "tech.xmuli.sunny.desktop";

    if (checked) {
        QDir().mkpath(targetDir);    // 创建目标目录（如果不存在）
        if (QFile::copy(desktopDir, targetFile)) qDebug() << "File tech.xmuli.sunny.desktop copied successfully to" << targetFile;
        else qDebug() << "Failed to copy file";
    } else {
        if (QFile::remove(targetFile)) qDebug() << "File removed successfully from" << targetFile;
        else qDebug() << "Failed tech.xmuli.sunny.desktop to remove file";
    }
#else
#endif

    CJ_SET("general.autostart", checked);
}


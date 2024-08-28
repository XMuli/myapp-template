#include "settingui.h"
#include "ui_settingui.h"
#include "ui_sub_general.h"
#include "ui_sub_update.h"
#include "ui_sub_about.h"
#include <QDir>
#include <QFontDialog>
#include <QSettings>
#include <QVBoxLayout>
#include <QPointer>
#include <QDateTime>
#include <QMessageBox>
#include "licenseui.h"
#include "communication.h"
#include "configjson.h"

SettingUI::SettingUI(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SettingUI)
    , uiSubGeneral(new Ui::SubGeneral)
    , uiSubUpdate(new Ui::SubUpdate)
    , uiSubAbout(new Ui::SubAbout)
    , m_verUpdate(new VersionUpdater(QString(XPROJECT_VERSION), this))
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

    if (m_verUpdate) m_verUpdate->deleteLater();
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

    // sub_general.ui
    onLanguageChanged("");
    connect(uiSubGeneral->cbbLanguage, &QComboBox::currentTextChanged, this, &SettingUI::onLanguageChanged);
    connect(uiSubGeneral->btnFont, &QPushButton::released, this, &SettingUI::onFontChanged);
    connect(uiSubGeneral->cbAutostart, &QCheckBox::clicked, this, &SettingUI::onAutostart);
    // sub_update.ui
    connect(uiSubUpdate->cbAutoCheckUpdate, &QCheckBox::clicked, this, &SettingUI::onAutoCheckUpdateClicked);
    connect(uiSubUpdate->sbDay, QOverload<int>::of(&QSpinBox::valueChanged), this, &SettingUI::onDayValueChanged);
    connect(uiSubUpdate->cbJoinInsiderProgram, &QCheckBox::clicked, this, &SettingUI::onJoinInsiderProgramToggled);

    connect(uiSubUpdate->cbbProxyType, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &SettingUI::onProxyTypeCurrentIndexChanged);
    connect(uiSubUpdate->leIP, &QLineEdit::editingFinished, this, &SettingUI::onIPEditingFinished);
    connect(uiSubUpdate->sbPort, QOverload<int>::of(&QSpinBox::valueChanged), this, &SettingUI::onPortEditingFinished);
    connect(uiSubUpdate->btnTest, &QPushButton::released, this, &SettingUI::onTestReleased);
    connect(uiSubUpdate->btnCheckUpdate, &QPushButton::released, this, &SettingUI::onCheckUpdateReleased);
    // sub_about.ui
    connect(uiSubAbout->btnLicenses, &QPushButton::released, this, &SettingUI::onLicensesRelease);

}

void SettingUI::closeEvent(QCloseEvent *e)
{
    CJ.onSyncToFile();
    QWidget::closeEvent(e);
}

void SettingUI::onLanguageChanged(const QString &arg1)
{
    COMM.loadTranslation(arg1);
    uiSubGeneral->retranslateUi(this);
    uiSubUpdate->retranslateUi(this);
    uiSubAbout->retranslateUi(this);
    ui->retranslateUi(this);  // fix: 在 uiSubXxx 后面，i18n 被刷新为 Form

    uiSubGeneral->btnFont->setText(CJ_GET_QSTR("general.font")); // fix: 切换语言后会被刷新掉
    uiSubAbout->labProject->setText(QString("%1").arg(XPROJECT_NAME));
    uiSubAbout->labBit->setText(tr("%1-bit").arg(XARCH_BIT));
    uiSubAbout->labVersion->setText(tr("Version %1 (%2) - %3 %4").arg(XPROJECT_VERSION).arg(XBUILD_TIME).arg(XCOMPILER).arg(XCOMPILER_ID));

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

void SettingUI::onAutoCheckUpdateClicked(bool checked)
{
    CJ_SET("update.enable_auto_check", checked);
}

void SettingUI::onDayValueChanged(int arg1)
{
    CJ_SET("update.day", arg1);
}

void SettingUI::onJoinInsiderProgramToggled(bool checked)
{
    CJ_SET("update.join_inside", checked);
}

// 0-NoProxy  1-SystemProxy,  2-HTTPS  3-SOCKET5
void SettingUI::onProxyTypeCurrentIndexChanged(int index)
{
    CJ_SET("update.porxy.type", index);
}


void SettingUI::onIPEditingFinished()
{
    CJ_SET("update.porxy.server", uiSubUpdate->leIP->text().toStdString());
}


void SettingUI::onPortEditingFinished()
{
    CJ_SET("update.porxy.port", uiSubUpdate->sbPort->value());
}


void SettingUI::onTestReleased()
{
    // 访问验证测试网络

    QStringList urlsToTest = {"https://www.baidu.com", "https://www.google.com", "https://www.github.com"};
    m_verUpdate->testConnectivity(urlsToTest);
}


void SettingUI::onCheckUpdateReleased()
{
    // 尝试验证和下载网络
    CJ_SET("update.last_check_time", QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss:zzz").toStdString());
    // m_verUpdate->checkForUpdate();
    m_verUpdate->downLatestVersion();
    // VersionUpdater
}

void SettingUI::onLicensesRelease()
{
    static QPointer<LicenseUI> licenseUI = nullptr;
    if (!licenseUI) {
        licenseUI = new LicenseUI(nullptr);
        if (!licenseUI->isVisible()) licenseUI->show();
    }
}




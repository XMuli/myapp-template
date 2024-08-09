// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2024-2024 XMuli
// SPDX-GitHub: https://github.com/XMuli
// SPDX-Author: XMuli <xmulitech@gmail.com>

#include "communication.h"
#include <cstdlib> // 包含 exit 函数的头文件
#include <QLocale>
#include <QTranslator>
#include <QGuiApplication>
#include <QDebug>
#include <QMessageBox>
#include <QDate>

#include <QString>

Communication &Communication::instance()
{
    static Communication ins;
    return ins;
}

void Communication::loadTranslation(const QString &language)
{
    QString temp = /*language.isEmpty() ? CJ_GET_QSTR("general.language") : language;*/ language.isEmpty() ? QLocale::system().name() : language;
    temp = temp.toLower();

    // 创建 QTranslator 对象
    static QTranslator* translator = nullptr;
    if (!translator) translator = new QTranslator(this);
    QString qmDir = qGuiApp->applicationDirPath() + "/translations/";
#if defined(Q_OS_LINUX)
    qmDir = QString("/usr/local/%1/translations/").arg(XPROJECT_NAME);
#endif

    const QString& qmName = QString("%1.qm").arg(toLocaleName(temp));
    const QString& qmPath = qmDir + qmName;
    qDebug() << "loadTranslation qmPath:" << qmPath;
    if (translator->load(qmPath)) {  // 加载翻译文件
        qApp->installTranslator(translator);
    }
}

QString Communication::toLocaleName(const QString &language)
{
    const auto& map = languageMap();
    auto it = map.find(language);
    return it != map.cend() ? it->second : "";
}

void Communication::trialVersion()
{
    // 设定检查的日期
    QDate checkDate(2024, 9, 30);
    QDate currentDate = QDate::currentDate();

    if (currentDate >= checkDate) {
        QString message = QObject::tr(
            "Your current version is outdated. Please download the latest version from "
            "<a href='http://ecgview.xmuli.tech'>ecgview.xmuli.tech</a> or contact developer at "
            "<a href='mailto:xmulitech@gmail.com'>xmulitech@gmail.com</a> for the download link."
            );

        QMessageBox msgBox;
        msgBox.setWindowTitle(QObject::tr("Version Update"));
        msgBox.setText(message);
        msgBox.setTextFormat(Qt::RichText);  // 使用富文本格式
        msgBox.setTextInteractionFlags(Qt::TextBrowserInteraction);  // 允许交互
        msgBox.setStandardButtons(QMessageBox::Ok);

        // 执行消息框，并在用户点击 OK 后退出程序
        if (msgBox.exec() == QMessageBox::Ok) {
            std::exit(0);
        }
    }
}

Communication::Communication(QObject *parent)
    : QObject(parent)
{

}


std::map<QString, QString> languageMap()
{
    static  std::map<QString, QString> map = {  {"English", "en_us"}
        // , {"简体中文", "zh_cn"}
        // , {"繁体中文", "zh_tw"}
    };
    return map;
}

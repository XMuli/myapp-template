// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2024-2024 XMuli
// SPDX-GitHub: https://github.com/XMuli
// SPDX-Author: XMuli <xmulitech@gmail.com>

#ifndef COMMUNICATION_H
#define COMMUNICATION_H
// #include <map>
#include <QObject>
#include <QString>
#include <QPointer>
#include "../network/version_updater/versionupdater.h"

#define COMM Communication::instance()

class Communication : public QObject
{
    Q_OBJECT
public:
    void loadTranslation(const QString& language);
    QString toLocaleName(const QString& language);
    void trialVersion();
    void setAppFont(const QString& font);
    static Communication &instance(); // signle

    QPointer<VersionUpdater> verUpdate() const;

signals:
    void sigLanguageChange(const QString qmFile);
    void sigShowSystemMessagebox(const QString &title, const QString &msg, const int& msecs); // 系统消息通知

private:
    explicit Communication(QObject *parent = nullptr);
    virtual ~Communication() = default;
    Communication(const Communication&) = delete;
    Communication& operator=(const Communication&) = delete;
    Communication(Communication&&) = delete;
    Communication& operator=(Communication&&) = delete;

    QPointer<VersionUpdater>  m_verUpdate;
};

std::map<QString, QString> languageMap();

#endif // COMMUNICATION_H

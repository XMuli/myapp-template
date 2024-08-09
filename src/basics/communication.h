// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2024-2024 XMuli
// SPDX-GitHub: https://github.com/XMuli
// SPDX-Author: XMuli <xmulitech@gmail.com>

#ifndef COMMUNICATION_H
#define COMMUNICATION_H
// #include <map>
#include <QObject>
#include <QString>

#define COMM Communication::instance()

class Communication : public QObject
{
    Q_OBJECT
public:
    static Communication &instance(); // signle
    void loadTranslation(const QString& language);
    QString toLocaleName(const QString& language);
    void trialVersion();

private:
    explicit Communication(QObject *parent = nullptr);
    virtual ~Communication() = default;
    Communication(const Communication&) = delete;
    Communication& operator=(const Communication&) = delete;
    Communication(Communication&&) = delete;
    Communication& operator=(Communication&&) = delete;
};

std::map<QString, QString> languageMap();

#endif // COMMUNICATION_H

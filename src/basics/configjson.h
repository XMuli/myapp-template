// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2024-2024 XMuli
// SPDX-GitHub: https://github.com/XMuli
// SPDX-Author: XMuli <xmulitech@gmail.com>

#ifndef CONFIGJSON_H
#define CONFIGJSON_H

#include <QObject>
#include "isingleton.h"
#include "json/json.hpp"
using ordered_json = nlohmann::ordered_json;

class ConfigJson : public QObject, public ISingleton<ConfigJson>
{
    Q_OBJECT
    FRIEND_CREAT_SINGLETION(ConfigJson);

private:
    ordered_json m_j;
    explicit ConfigJson(QObject *parent = nullptr);
    virtual ~ConfigJson() = default;

private:
    // QString m_jFilePath;
    QString m_key;
    QString m_iv;
};

#endif // CONFIGJSON_H

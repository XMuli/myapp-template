#ifndef CONFIGJSON_H
#define CONFIGJSON_H

#include <QObject>
#include <QPen>
#include <QChar>
#include <QBrush>
#include <QDebug>
#include <QString>
#include <QApplication>
#include <QStandardPaths>
#include <QFont>
#include "isingleton.h"
#include "json.hpp"
using ordered_json = nlohmann::ordered_json;

#define CJ ConfigJson::instance()
#define CJ_SET(name, val) CJ.setKeyValue(QStringLiteral(name), val)

// GET 多层时候采用 . 句号分割
#define CJ_GET(name) CJ.getKeyValue(QStringLiteral(name))                                        // #define CJ_GET(name) CJ.getKeyValue(#name)   1.不要用 #name; 2.不要使用 dump() 会造成引号有两次
#define CJ_GET_STR(name) CJ.getKeyValue(QStringLiteral(name)).get<std::string>()
#define CJ_GET_QSTR(name) QString::fromStdString(CJ.getKeyValue(QStringLiteral(name)).get<std::string>())    // 直接获取单例里面的键值对
//-------------------------------------------------------------
#define CJ_CD CJ.m_cd  // 获取 ContextData m_cd

/***************************************************************************************************************

*.ui  ⇆  ConfigJson::instance().ordered_json m_j  ⇆  xconfig.json

***************************************************************************************************************/

class ConfigJson : public QObject, public ISingleton<ConfigJson>
{
    Q_OBJECT
    FRIEND_CREAT_SINGLETION(ConfigJson);
public:
    void initPrepare();             // 准备工作.比如创建文件夹之类

    ordered_json defaultConfigJson();   // 一个原始的 json 初始化字符串
    void readFromFile();
    void writeToFile();

    void initOutputDefaulValue();
    void onSyncToFile();
    void setKeyValue(const QString& key, const ordered_json& val); // 设置多层键值对
    ordered_json getKeyValue(const QString& key);    // 获取多层键值对

private:
    void createDefaultConfigInLinux();
    explicit ConfigJson(QObject *parent = nullptr);
    virtual ~ConfigJson() = default;

private:
    ordered_json m_j;
    QString      m_key;
    QString      m_iv;
    QString      m_jFilePath;
};

#endif // CONFIGJSON_H

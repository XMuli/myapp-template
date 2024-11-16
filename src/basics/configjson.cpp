#include "configjson.h"
#include <Qt>
#include <QFile>
#include <QPen>
#include <QBrush>
#include <QString>
#include <QByteArray>
#include <QMutex>
#include <QMetaEnum>
#include <QApplication>
#include <QCryptographicHash>
#include <QDir>
#include <QVersionNumber>

// first time init  xconfig.json write to linux "~/.config/Sunny"
void ConfigJson::initPrepare()
{
    bool needDefaultConfig = false; // 是否需要创建默认配置

    if (QFile::exists(m_jFilePath)) {   // 有旧的配置文件

        readFromFile();
        auto j = CJ_GET("advanced.info.version");
        if (j.empty()) {
            needDefaultConfig = true; // 配置文件为空，需要创建默认配置
        } else {
            QVersionNumber vJson = QVersionNumber::fromString(QString::fromStdString(j.get<std::string>()));
            QVersionNumber vSoft = QVersionNumber::fromString(QString(XPROJECT_VERSION));
            if (vJson < vSoft) {
                needDefaultConfig = true; // 配置文件版本低于当前软件版本，需要创建默认配置
            }
        }

        m_j.clear();
    } else {
        needDefaultConfig = true; // 没有配置文件，需要创建默认配置
    }

    if (needDefaultConfig)
        createDefaultConfigInLinux();
}

ordered_json ConfigJson::defaultConfigJson()
{
    // 打开 JSON 文件
    QFile file(":/config/xconfig.json");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open config file:" << file.errorString();
        return ordered_json(); // 返回一个空的 JSON 对象
    }

    // 读取文件内容
    QTextStream in(&file);
    QString fileContent = in.readAll();
    file.close();

    // 解析 JSON 内容
    ordered_json json;
    try {
        json = ordered_json::parse(fileContent.toStdString());
    } catch (const nlohmann::json::parse_error& e) {
        qDebug() << "JSON parse error:" << e.what();
        return ordered_json(); // 返回一个空的 JSON 对象
    }

    return json;
}


void ConfigJson::readFromFile()
{
    QFile file(m_jFilePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open file for reading. m_jFilePath:" << m_jFilePath << "error:" << file.errorString();
        return;
    }

    QByteArray data = file.readAll();
    if (data.isEmpty()) return;

    m_j = ordered_json::parse(data.toStdString()); // Convert JSON data to ordered_json
    file.close();

    QString jsonString = QString::fromStdString(m_j.dump());
    // qDebug().noquote() << "m_j content:" << jsonString;
}

void ConfigJson::writeToFile()
{
    QFile file(m_jFilePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        qDebug() << "Failed to open file for writing: m_jFilePath:" << m_jFilePath << "error:" << file.errorString();
        return;
    }

    file.write(QString::fromStdString(m_j.dump(4)).toUtf8()); // 4 个空格缩进
    file.close();
}

void ConfigJson::initOutputDefaulValue()
{
    // fix: 初次读配置文件, Output path 为空; 此时单例还没有构造完成，不能用宏
    // setKeyValue("output.flie_name", QString("%1_$yyyyMMdd_hhmmss$.png").arg(XPROJECT_NAME).toStdString());
    setKeyValue("advanced.info.exe_path", qApp->applicationDirPath().toStdString());
    setKeyValue("advanced.info.output_path", QStandardPaths::standardLocations(QStandardPaths::DesktopLocation).first().toStdString());

    // qDebug().noquote() << "Parsed JSON content:" << QString::fromStdString(m_j.dump(4)); // Pretty print with indentation
}

void ConfigJson::onSyncToFile()
{
    writeToFile();
}

void ConfigJson::setKeyValue(const QString &key, const ordered_json &val)
{
    QStringList keys = key.split('.'); // 使用点号分隔键
    ordered_json* currentObj = &m_j;
    for (const QString& k : keys) {
        currentObj = &((*currentObj)[k.toUtf8().constData()]);
    }
    *currentObj = val;
}

ordered_json ConfigJson::getKeyValue(const QString &key)
{
    QStringList keys = key.split('.'); // 使用点号分隔键
    ordered_json* currentObj = &m_j;
    for (const QString& k : keys) {
        auto it = currentObj->find(k.toStdString());
        if (it == currentObj->end())
            return ordered_json(); // 如果键不存在，返回默认值

        currentObj = &it.value();
    }

    return *currentObj;
}

void ConfigJson::createDefaultConfigInLinux()
{
    const QString& str = QFileInfo(m_jFilePath).dir().path();
    QDir dir(str);
    dir.removeRecursively();  // 删除目录及其所有内容
    dir.mkpath(str);          // 仅新创建文件夹

    m_j = defaultConfigJson();
    writeToFile();
    m_j.clear();
}

ConfigJson::ConfigJson(QObject *parent)
    : QObject(parent)
    , m_j()
    , m_key("key-s%Prup7BWa6Wn%pB")
    , m_iv("IV-Q#3VZjPbmS!wZfZL")
    , m_jFilePath("")
{
    QString xconfigDir = qApp->applicationDirPath();
#if defined(Q_OS_LINUX)
    xconfigDir = QDir::homePath() + QString("/.config/%1").arg(XPROJECT_NAME);
#endif

    m_jFilePath = xconfigDir + "/xconfig.json";
}

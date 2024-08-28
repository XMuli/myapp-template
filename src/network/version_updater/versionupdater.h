#ifndef VERSIONUPDATER_H
#define VERSIONUPDATER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QVersionNumber>
#include <QNetworkProxy>
#include <QTemporaryFile>
#include "json.hpp"
using json = nlohmann::json;




class VersionUpdater : public QObject
{
    Q_OBJECT

public:
    explicit VersionUpdater(const QString &localVersion, QObject *parent = nullptr);

    enum RESP_TYPE {
        RT_empty,                      // 忘记头文件加此，则解析默认为这个
        RT_check_update,               // 检查版本更新
        RT_download_latest,            // 仅下载支持 release
        RT_download_insider            // 下载包括内测版本
    };
    Q_ENUM(RESP_TYPE)

    enum ProxyType {
        NoProxy,
        SystemProxy,
        HttpProxy,
        Socks5Proxy
    };

public:
    void checkForUpdate();
    void downLatestVersion();
    void testConnectivity(const QStringList &urls);

private:
    void setProxy(ProxyType proxyType, const QString &host = QString(), int port = 0);

    void dealCheckForUpdate(QNetworkReply *reply);
    void dealDownLatestVersion(QNetworkReply *reply);

    void handleRedirect(QNetworkReply *reply);

signals:
    void updateAvailable(const QString &latestVersion, const QString &downloadUrl);

public slots:
    void onFinished(QNetworkReply *reply);
    void onConnectivityTestFinished(QNetworkReply *reply);


private:
    QString m_localVersion;
    QNetworkAccessManager *m_manager;
    QString m_latestVersion;
    QString m_downloadUrl;
    QTemporaryFile m_tempFile;
};

#endif // VERSIONUPDATER_H

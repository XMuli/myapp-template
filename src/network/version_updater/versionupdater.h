#ifndef VERSIONUPDATER_H
#define VERSIONUPDATER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QVersionNumber>
#include <QNetworkProxy>
#include "json.hpp"
using json = nlohmann::json;

class VersionUpdater : public QObject
{
    Q_OBJECT

public:
    explicit VersionUpdater(const QString &localVersion, QObject *parent = nullptr);

    enum ProxyType {
        NoProxy,
        SystemProxy,
        HttpProxy,
        Socks5Proxy
    };

    void checkForUpdate();
    void setProxy(ProxyType proxyType, const QString &host = QString(), int port = 0);
    void testConnectivity(const QStringList &urls);

signals:
    void updateAvailable(const QString &latestVersion, const QString &downloadUrl);
    void noUpdateAvailable();
    void errorOccurred(const QString &errorMessage);
    void connectivityTestResult(const QString &url, bool success);

public slots:
    void onFinished(QNetworkReply *reply);
    void onConnectivityTestFinished(QNetworkReply *reply);

private:
    QString m_localVersion;
    QNetworkAccessManager *m_manager;
    QString m_latestVersion;
    QString m_downloadUrl;
};

#endif // VERSIONUPDATER_H

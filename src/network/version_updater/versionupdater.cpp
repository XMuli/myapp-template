#include "versionupdater.h"

#include <QMessageBox>
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>

VersionUpdater::VersionUpdater(const QString &localVersion, QObject *parent)
    : QObject(parent),
    m_localVersion(localVersion),
    m_manager(new QNetworkAccessManager(this))
{
    connect(m_manager, &QNetworkAccessManager::finished, this, &VersionUpdater::onFinished);
}

void VersionUpdater::checkForUpdate()
{
    QUrl url("https://api.github.com/repos/XMuli/SunnyPages/releases/latest");
    QNetworkRequest request(url);
    m_manager->get(request);
}

void VersionUpdater::setProxy(ProxyType proxyType, const QString &host, int port)
{
    QNetworkProxy proxy;
    switch (proxyType) {
    case NoProxy:
        proxy.setType(QNetworkProxy::NoProxy);
        break;
    case SystemProxy:
        proxy.setType(QNetworkProxy::DefaultProxy);
        break;
    case HttpProxy:
        proxy.setType(QNetworkProxy::HttpProxy);
        proxy.setHostName(host);
        proxy.setPort(port);
        break;
    case Socks5Proxy:
        proxy.setType(QNetworkProxy::Socks5Proxy);
        proxy.setHostName(host);
        proxy.setPort(port);
        break;
    }

    m_manager->setProxy(proxy);
}

void VersionUpdater::testConnectivity(const QStringList &urls)
{
    for (const QString &urlString : urls) {
        QUrl url(urlString);
        QNetworkRequest request(url);
        QNetworkReply *reply = m_manager->head(request);
        connect(reply, &QNetworkReply::finished, this, [this, reply, urlString]() {
            onConnectivityTestFinished(reply);
            emit connectivityTestResult(urlString, reply->error() == QNetworkReply::NoError);
        });
    }
}

void VersionUpdater::onFinished(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError) {
        emit errorOccurred(reply->errorString());
        reply->deleteLater();
        return;
    }

    const QByteArray response = reply->readAll();
    json jsonDoc = json::parse(response.toStdString(), nullptr, false);

    if (jsonDoc.is_discarded()) {
        emit errorOccurred("Failed to parse JSON.");
        reply->deleteLater();
        return;
    }

    m_latestVersion = QString::fromStdString(jsonDoc["tag_name"].get<std::string>()).trimmed();
    if (m_latestVersion.startsWith("v")) m_latestVersion.remove(0, 1);  // 去掉版本号前的 'v' 字符
    m_downloadUrl = QString::fromStdString(jsonDoc["assets"][0]["browser_download_url"].get<std::string>());

    if (QVersionNumber::fromString(m_latestVersion) > QVersionNumber::fromString(m_localVersion)) {
        emit updateAvailable(m_latestVersion, m_downloadUrl);
    } else {
        emit noUpdateAvailable();
    }

    reply->deleteLater();
}

void VersionUpdater::onConnectivityTestFinished(QNetworkReply *reply)
{
    reply->deleteLater();
}

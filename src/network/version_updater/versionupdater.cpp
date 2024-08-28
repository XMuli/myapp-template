#include "versionupdater.h"
#include <QNetworkRequest>
#include <QMessageBox>
#include <QUrl>
#include <QFileInfo>
#include <qdir.h>

VersionUpdater::VersionUpdater(const QString &localVersion, QObject *parent)
    : QObject(parent), m_localVersion(localVersion), m_manager(new QNetworkAccessManager(this))
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

void VersionUpdater::onFinished(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError) {
        emit errorOccurred(reply->errorString());
        reply->deleteLater();
        return;
    }

    handleRedirect(reply);
    if (!reply->isFinished()) {
        reply->deleteLater();
        return;
    }

    auto jsonData = json::parse(reply->readAll().toStdString());
    m_latestVersion = QString::fromStdString(jsonData["tag_name"].get<std::string>());
    if (m_latestVersion.startsWith('v')) m_latestVersion.remove(0, 1);
    m_downloadUrl = QString::fromStdString(jsonData["assets"][0]["browser_download_url"].get<std::string>());

    if (QVersionNumber::fromString(m_localVersion) < QVersionNumber::fromString(m_latestVersion)) {
        emit updateAvailable(m_latestVersion, m_downloadUrl);
        downloadLatestVersion();
    } else {
        emit noUpdateAvailable();
    }

    reply->deleteLater();
}

void VersionUpdater::handleRedirect(QNetworkReply *reply)
{
    QUrl redirectUrl = reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();
    if (!redirectUrl.isEmpty()) {
        QNetworkRequest newRequest(redirectUrl);
        m_manager->get(newRequest);
    }
}

void VersionUpdater::testConnectivity(const QStringList &urls)
{
    foreach (const QString &url, urls) {
        QNetworkRequest request((QUrl(url)));
        QNetworkReply *reply = m_manager->get(request);
        connect(reply, &QNetworkReply::finished, this, [this, reply]() { onConnectivityTestFinished(reply); });
    }
}

void VersionUpdater::onConnectivityTestFinished(QNetworkReply *reply)
{
    bool success = (reply->error() == QNetworkReply::NoError);
    emit connectivityTestResult(reply->url().toString(), success);
    reply->deleteLater();
}

void VersionUpdater::downloadLatestVersion()
{
    if (m_downloadUrl.isEmpty()) {
        emit errorOccurred("Download URL is empty.");
        return;
    }

    QUrl url(m_downloadUrl);
    QNetworkRequest request(url);
    QNetworkReply *reply = m_manager->get(request);
    connect(reply, &QNetworkReply::readyRead, this, &VersionUpdater::onDownloadReadyRead);
    connect(reply, &QNetworkReply::finished, this, &VersionUpdater::onDownloadFinished);
}

void VersionUpdater::startDownload()
{
    if (!m_tempFile.open()) {
        emit errorOccurred("Failed to create temporary file.");
        return;
    }
}

void VersionUpdater::onDownloadReadyRead()
{
    if (!m_tempFile.isOpen()) {
        startDownload();
    }
    m_tempFile.write(qobject_cast<QNetworkReply *>(sender())->readAll());
}

void VersionUpdater::onDownloadFinished()
{
    m_tempFile.close();

    QString fileName = QFileInfo(QUrl(m_downloadUrl).path()).fileName();
    QString tempFilePath = QDir::tempPath() + "/" + fileName;
    if (QFile::rename(m_tempFile.fileName(), tempFilePath)) {
        QMessageBox::information(nullptr, "Download Complete", "The update has been downloaded to " + tempFilePath);
    } else {
        emit errorOccurred("Failed to rename the downloaded file.");
    }

    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    reply->deleteLater();
}

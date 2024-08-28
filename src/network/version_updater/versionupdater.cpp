#include "versionupdater.h"
#include <QNetworkRequest>
#include <QMessageBox>
#include <QUrl>
#include <QFileInfo>
#include <QMetaEnum>
#include <QDir>

VersionUpdater::VersionUpdater(const QString &localVersion, QObject *parent)
    : QObject(parent), m_localVersion(localVersion), m_manager(new QNetworkAccessManager(this))
{
    connect(m_manager, &QNetworkAccessManager::finished, this, &VersionUpdater::onFinished);
}

void VersionUpdater::checkForUpdate()
{
    QUrl url("https://api.github.com/repos/XMuli/SunnyPages/releases/latest");
    QNetworkRequest request(url);
    request.setAttribute(QNetworkRequest::User, int(RESP_TYPE::RT_check_update));
    m_manager->get(request);
}

void VersionUpdater::testUrlConnectivity(const QStringList &urls)
{
    foreach (const QString &url, urls) {
        QNetworkRequest request((QUrl(url)));
        request.setAttribute(QNetworkRequest::User, int(RESP_TYPE::RT_test_url));
        m_manager->get(request);
    }
}

void VersionUpdater::downLatestVersion(QString url)
{
    url = "https://picgo-release.molunerfinn.com/2.4.0-beta.8/PicGo-Setup-2.4.0-beta.8-ia32.exe";
    // m_downloadUrl = "https://picgo-release.molunerfinn.com/2.4.0-beta.8/PicGo-Setup-2.4.0-beta.8.exe";
    if (url.isEmpty()) {
        qDebug() << "Download URL is empty.";
        return;
    }

    QNetworkRequest request((QUrl(url)));
    request.setAttribute(QNetworkRequest::User, int(RESP_TYPE::RT_download_latest));
    QNetworkReply * reply = m_manager->get(request);
    m_tempFile.open();

    connect(reply, &QNetworkReply::downloadProgress, this, [](qint64 bytesReceived, qint64 bytesTotal) {
        if (bytesTotal > 0) {
            int progress = static_cast<int>((bytesReceived * 100) / bytesTotal);
            qDebug() << "Download progress:" << bytesReceived << "/" << bytesTotal << progress << "%";
        } else {
            qDebug() << "Download progress:" << bytesReceived << "/Unknown" << "0%";
        }
    });


    // 逐块读取数据并写入临时文件
    connect(reply, &QNetworkReply::readyRead, this, [this, reply]() {
        if (m_tempFile.isOpen()) m_tempFile.write(reply->readAll());
    });
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


void VersionUpdater::dealCheckForUpdate(QNetworkReply *reply)
{
    json j;
    try {
        j = json::parse(reply->readAll().toStdString());
    } catch (json::parse_error& e) {
        qDebug() << "JSON Parsing failure: " << e.what();
        return;
    }
    if (j.empty()) return;

    QString latestVersion = QString::fromStdString(j["tag_name"].get<std::string>());
    if (latestVersion.startsWith('v')) latestVersion.remove(0, 1);
    QString url = QString::fromStdString(j["assets"][0]["browser_download_url"].get<std::string>());

    if (QVersionNumber::fromString(m_localVersion) < QVersionNumber::fromString(latestVersion)) {
        const auto& ret = QMessageBox::information(nullptr, tr("Update Available"), tr("A new version %1 is available. Do you want to download it?\ndownload link: %2").arg(latestVersion, url), QMessageBox::Ok | QMessageBox::Cancel);
        if (ret == QMessageBox::Ok)
            downLatestVersion(url);
    } else {
        QMessageBox::information(nullptr, tr("No Update"), tr("You are already on the latest version."));
    }
}

void VersionUpdater::dealDownLatestVersion(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << "Download error:" << reply->errorString();
        m_tempFile.remove(); // 删除错误的文件
        m_tempFile.close(); // 关闭文件
        return;
    } else {
        m_tempFile.flush(); // 确保所有数据都写入文件            m_tempFile.close(); // 关闭文件
    }

    QString tempFilePath = m_tempFile.fileName(); // 获取临时文件路径
    qDebug() << "Download completed. Temporary file saved to:" << tempFilePath;

    // // 从下载链接中提取文件名
    // QUrl url(m_downloadUrl);
    // QString fileName = QFileInfo(url.path()).fileName();
    // // QString finalFilePath = QDir::homePath() + QDir::separator() + fileName;
    // QString finalFilePath = QString("D:") + QDir::separator() + fileName;

    // qDebug() << "Temporary file path:" << tempFilePath;
    // qDebug() << "Final file path:" << finalFilePath;

    // // 重命名临时文件
    // if (QFile::rename(tempFilePath, finalFilePath)) {
    //     qDebug() << "File rename saved to:" << finalFilePath;
    // } else {
    //     qDebug() << "Failed to rename file.";
    // }
}

void VersionUpdater::dealTestUrlConnectivity(QNetworkReply *reply)
{
    bool success = (reply->error() == QNetworkReply::NoError);
    qDebug() << QString("Test URL: %1 is %2.").arg(reply->url().toString()).arg(success ? "true" : "false");
}

void VersionUpdater::handleRedirect(QNetworkReply *reply)
{
    QUrl redirectUrl = reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();
    qDebug() << "---------------->redirectUrl:" << redirectUrl;
    if (!redirectUrl.isEmpty()) {
        QNetworkRequest newRequest(redirectUrl);
        newRequest.setAttribute(QNetworkRequest::User, int(RESP_TYPE::RT_download_latest));
        m_manager->get(newRequest);
    }
}

void VersionUpdater::onFinished(QNetworkReply *reply)
{
    const RESP_TYPE& type = static_cast<RESP_TYPE>(reply->request().attribute(QNetworkRequest::User, int(RESP_TYPE::RT_empty)).toInt());
    const bool nonTest = static_cast<RESP_TYPE>(type) != RESP_TYPE::RT_test_url;
    if (reply->error() != QNetworkReply::NoError && !nonTest) return;

    QMetaEnum metaEnum = QMetaEnum::fromType<VersionUpdater::RESP_TYPE>();
    const auto& szType = metaEnum.valueToKey(int(type));            //! enum转string
    qDebug() << "onFinished() type:" << szType << (int)type;

    if (static_cast<RESP_TYPE>(type) == RESP_TYPE::RT_test_url) {
        dealTestUrlConnectivity(reply);
    } else if (static_cast<RESP_TYPE>(type) == RESP_TYPE::RT_check_update) {
        dealCheckForUpdate(reply);
    } else if (static_cast<RESP_TYPE>(type) == RESP_TYPE::RT_download_latest || static_cast<RESP_TYPE>(type) == RESP_TYPE::RT_download_insider) {

        // handleRedirect(reply);
         // if (!reply->isFinished()) {
         //     reply->deleteLater();
         //     return;
         // }
        dealDownLatestVersion(reply);
    }


    qDebug() << "reply:" << reply;
    if (reply) reply->deleteLater();
}




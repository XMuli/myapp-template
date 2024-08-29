#include "versionupdater.h"
#include <QNetworkRequest>
#include <QMessageBox>
#include <QUrl>
#include <QFileInfo>
#include <QMetaEnum>
#include <QDir>
#include "../basics/configjson.h"

VersionUpdater::VersionUpdater(const QString &localVersion, QObject *parent)
    : QObject(parent)
    , m_localVersion(localVersion)
    , m_manager(new QNetworkAccessManager(this))
    , m_redirect(true)
{
    connect(m_manager, &QNetworkAccessManager::finished, this, &VersionUpdater::onFinished);
}

void VersionUpdater::checkForUpdate()
{
    QUrl url("https://api.github.com/repos/XMuli/SunnyPages/releases/latest");
    QNetworkRequest request(url);
    request.setAttribute(QNetworkRequest::User, int(RESP_TYPE::RT_check_update));
    m_manager->get(request);


    QDateTime currentTime = QDateTime::currentDateTime();
    QString currentTimeStr = currentTime.toString(Qt::ISODate); // 格式化为 ISO 日期字符串
    CJ_SET("update.last_check_time", currentTimeStr.toStdString()); // 保存当前时间为上次检查时间
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
    // url = "https://picgo-release.molunerfinn.com/2.4.0-beta.8/PicGo-Setup-2.4.0-beta.8-ia32.exe";
    // m_downloadUrl = "https://picgo-release.molunerfinn.com/2.4.0-beta.8/PicGo-Setup-2.4.0-beta.8.exe";
    if (url.isEmpty()) {
        qDebug() << "Download URL is empty.";
        return;
    }

    QUrl qurl(url);
    QNetworkRequest request(qurl);
    request.setAttribute(QNetworkRequest::User, int(RESP_TYPE::RT_download_latest));
    setProperty("down_file_name", QFileInfo(qurl.path()).fileName());

    if (m_redirect) { // 是 GitHub 会重定向的链接

        QNetworkReply* reply = m_manager->get(request);
        connect(reply, &QNetworkReply::finished, this, [this, reply](){
            QUrl redirectUrl = reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();

            if (!redirectUrl.isEmpty()) {
                m_redirect = false;
                QNetworkRequest newRequest(redirectUrl);
                newRequest.setAttribute(QNetworkRequest::User, int(RESP_TYPE::RT_download_latest));
                QNetworkReply* newReply = m_manager->get(newRequest);

                m_tempFile.open();
                connect(newReply, &QNetworkReply::downloadProgress, this, &VersionUpdater::sigDownloadProgress);
                connect(newReply, &QNetworkReply::readyRead, this, [this, newReply]() {
                    if (m_tempFile.isOpen()) m_tempFile.write(newReply->readAll());});   // 逐块读取数据并写入临时文件
            }
        });

    }
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
    m_redirect = true;
    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << "Download error:" << reply->errorString();
        m_tempFile.remove(); // 删除错误的文件
        m_tempFile.close(); // 关闭文件
        return;
    } else {
        m_tempFile.flush(); // 确保所有数据都写入文件
        m_tempFile.close(); // 关闭文件
    }

    QString tempFilePath = m_tempFile.fileName();  // 获取临时文件路径
    QFileInfo fileInfo(tempFilePath);
    QString directoryPath = fileInfo.path();  // 获取文件所在目录路径
    const QString name = property("down_file_name").toString();
    // QString finalFilePath = QDir::homePath() + QDir::separator() + name;
    QString finalFilePath = directoryPath + QDir::separator() + name;
    qDebug() << "Download completed. Temporary file saved to:" << tempFilePath;
    qDebug() << "Final file path:" << finalFilePath;

    QFile::remove(finalFilePath);  // 删除目标文件（如果存在）
    if (QFile::copy(tempFilePath, finalFilePath)) {
        qDebug() << "File copied successfully to:" << finalFilePath;
        m_tempFile.remove();  // 删除原来的临时文件
    } else {
        qDebug() << "Failed to copy file to:" << finalFilePath;
    }
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

bool VersionUpdater::userAllowCheckUpdate()
{
    bool bAutoCheck = CJ_GET("update.enable_auto_check").get<bool>();
    if (!bAutoCheck) return false;

    QString lastCheckTimeStr = CJ_GET_QSTR("update.last_check_time");
    int lastDay = CJ_GET("update.day").get<int>();
    if (lastCheckTimeStr.isEmpty()) return true;

    // 将 lastCheckTime 转换为 QDateTime 对象
    QDateTime lastCheckTime = QDateTime::fromString(lastCheckTimeStr, Qt::ISODate);
    if (!lastCheckTime.isValid()) return false;  // 确保上次检查时间是有效的
    QDateTime currentTime = QDateTime::currentDateTime();

    // 计算当前时间与上次检查时间的差值，判断是否大于等于设置的天数
    if (lastCheckTime.daysTo(currentTime) >= lastDay)
        return true;

    return false;
    // bool bJoinInside = CJ_GET("update.join_inside").get<bool>();
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

        if (!m_redirect)
            dealDownLatestVersion(reply);
    }

    static int i = 1;
    qDebug() << "deleteLater reply:" << reply << i++;
    if (reply) reply->deleteLater();
}




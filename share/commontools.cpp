#include "commontools.h"
#include "gui/forms/mainwindow.h"
#include "ui_mainwindow.h"
#include "share/defs.h"
#include "settings/settingsmanager.h"
#include <QMessageBox>
#include <QList>
#include <QFile>
#include <QFileInfo>
#include "share/debug.h"

void CommonTools::setHeader(const QString &accessToken, QNetworkRequest &request)
{
    request.setRawHeader("User-Agent", SettingsManager().applicationName().toAscii());
    request.setRawHeader("GData-Version", "3.0");
    request.setRawHeader("Authorization", QString("Bearer %1").arg(accessToken).toLatin1());
}

void CommonTools::msg(const QString &text, QWidget *parent)
{
    QMessageBox msgBox(parent);

    msgBox.setText(text);
    msgBox.exec();
}

int CommonTools::errorMsg(const QString &caption, const QString &text, QWidget *parent)
{
    return QMessageBox::critical(parent, caption, text, QMessageBox::Ok);
}

//QString CommonTools::convertDate(const QString &dateStr)
//{
//    QDateTime fileDateTime = QDateTime::fromString(dateStr, Qt::ISODate);
//    return fileDateTime.toLocalTime().toString("ddd, MMM d yyyy, h:mm");
//}

//QString CommonTools::getFormattedFileSize(const QString &sizeStr)
//{
//    return QLocale().toString(sizeStr.toLongLong());
//}

void CommonTools::logToFile(const QString &fileName, const QByteArray &bytes)
{
    QFile file(fileName);

    file.open(QIODevice::WriteOnly);
    file.write(bytes);
    file.close();
}

QByteArray CommonTools::loadFromFile(const QString &fileName)
{
    QFile file(fileName);

    file.open(QIODevice::ReadOnly);
    QByteArray arr = file.readAll();
    file.close();

    return arr;
}

bool CommonTools::fileFromURLExists(const QString &url)
{
    QFileInfo fi(url);
    QFile file(fi.fileName());

    return file.exists();
}

QString CommonTools::getFileNameFromURL(const QString &url)
{
  QFileInfo fi(url);
  return fi.fileName();
}

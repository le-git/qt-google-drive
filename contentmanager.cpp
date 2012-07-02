#include "contentmanager.h"
#include <QApplication>
#include <QDebug>

ContentManager::ContentManager(int handleType, QObject *parent):
    NetworkManager(parent),
    operationsManager(new OperationsManager(parent)),
    type(handleType)
{
}

ContentManager::~ContentManager()
{
}

void ContentManager::get(const QString& url)
{
    QApplication::setOverrideCursor(Qt::ArrowCursor);

    CommonTools::setHeader(request);
    getRequest(url);

    QApplication::setOverrideCursor(Qt::WaitCursor);
}

void ContentManager::slotReplyFinished(QNetworkReply* reply)
{
    CommonTools::logToFile(QString::number(type) + ".txt", replyStr.toAscii());

    if(parseReply(replyStr)) qDebug() << "parse OK";
    else qDebug() << "parse not OK";

    replyStr.clear();
    if(!parser->getXMLHandler()->resDownloadingNow()) show();

    QApplication::restoreOverrideCursor();
}

bool ContentManager::parseReply(const QString& str)
{
    QXmlSimpleReader reader;
    QXmlInputSource source;

    parser.reset(new XMLParser(type));

    connect(parser->getXMLHandler(), SIGNAL(signalAllResDownloaded(int)),this, SLOT(slotResDownloaded(int)));

    source.setData(str.toAscii());

    reader.setContentHandler(parser.data());
    reader.setErrorHandler(parser.data());

    return reader.parse(&source);
}

void ContentManager::slotResDownloaded(int queryType)
{
    if(queryType == type) show();
}

XMLParser* ContentManager::getParser(void) const
{
    return parser.data();
}

void ContentManager::clear(void)
{
    items.clear();
    if(items.empty()) return;

    for(int i = 0; i < items.count(); ++i) delete items[i];

    QApplication::setOverrideCursor(Qt::ArrowCursor);
}

void ContentManager::del(const QString &url)
{
  operationsManager->del(url);
}

void ContentManager::createFolder(const QString& folderUrl, const QString& name)
{
  operationsManager->createFolder(folderUrl, name);
}

OperationsManager* ContentManager::getOperationsManager(void) const
{
  return operationsManager.data();
}



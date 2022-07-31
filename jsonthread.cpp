#include "jsonthread.h"

JsonThread::JsonThread(QObject *parent)
    : QThread{parent}
{

}

JsonThread::~JsonThread()
{
    qDebug() << "JsonThread: CurrentThread has been released.";
}

void JsonThread::setDataModel(QJsonModel *model)
{
    dataModel = model;
}

void JsonThread::setDataObject(const std::string& jsonStr)
{
    QJsonDocument doc = QJsonDocument::fromJson(QString::fromStdString(jsonStr).toUtf8());
    dataObject = doc.object();
}

void JsonThread::run()
{
    if (dataModel == nullptr)
    {
        qDebug() << "JsonThread : dataModel is nullptr";
        return;
    }

    if (dataObject.isEmpty())
    {
        qDebug() << "JsonThread : dataObject is empty";
        return;
    }

    dataModel->loadJsonObject(dataObject);
}

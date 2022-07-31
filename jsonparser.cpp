#include "jsonparser.h"
#include <QThread>

JsonParser::JsonParser(QObject *parent)
    : QObject{parent}
{

}

JsonParser::~JsonParser()
{
    qDebug() << QThread::currentThread() << " QJsonparser has been deleted.";
}

void JsonParser::setModel(QJsonModel *model)
{
    jModel = model;
}

void JsonParser::parse(const QJsonObject &jsonObj)
{
    jModel->loadJsonObject(jsonObj);

    emit parseFinished();
}

void JsonParser::parseStr(const std::string &jsonStr)
{
    jModel->loadJson(QByteArray::fromStdString(jsonStr));
    emit parseFinished();
}

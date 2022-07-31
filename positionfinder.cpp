#include "positionfinder.h"

PositionFinder::PositionFinder(QObject *parent)
    : QObject{parent}
{

}

PositionFinder::~PositionFinder()
{
    qDebug() << QThread::currentThread() << " QPositionFinder has been deleted.";
}

void PositionFinder::getPositionJson(const QJsonObject &pObj)
{
    pObject = pObj;
}

void PositionFinder::getPositionJsonStr(const std::string &jsonStr)
{
    QJsonDocument doc = QJsonDocument::fromJson(QByteArray::fromStdString(jsonStr));
    pObject = doc.object();
}

void PositionFinder::find(QList<QString> keys)
{
    if (pObject.isEmpty())
    {
        emit positionFound(0, 0);
        return;
    }

    QJsonObject object = pObject;
    QJsonValue value;
    QString startValue;
    QString endValue;

    int startIndex = 0;
    int endIndex = 0;

    while (!keys.isEmpty())
    {
        QString keyword = keys.takeFirst(); // 按顺序获取节点名称
        if (object.contains(keyword))
        {
            if (keys.isEmpty()) // 当前为目标节点
            {
                if (object.contains(keyword + "位置")) // 当前节点还包含子节点
                    object = object.value(keyword + "位置").toObject();
                else // 当前节点不包含子节点
                    object = object.value(keyword).toObject();

                startValue = object.value("start index").toString();
                endValue = object.value("end index").toString();

                startIndex = startValue.toInt();
                endIndex = endValue.toInt();
            }
            else { // 当前为目标节点的祖先节点
                value = object.value(keyword);
                object = value.toObject();
            } // if (keys.isEmpty())
        } // if (object.contains(keyword))
        else
        { // 找不到节点，跳出循环
            startIndex = 0;
            endIndex = 0;
            break;
        } // if (object.contains(keyword))
    } // while

    emit positionFound(startIndex, endIndex);
}

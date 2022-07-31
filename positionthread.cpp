#include "positionthread.h"

#include <utility>

PositionThread::PositionThread(QObject *parent)
    : QThread{parent}
{

}

PositionThread::~PositionThread()
{
    qDebug() << "PositionThread: CurrentThread has been released.";
}

void PositionThread::setPosition(const std::string& jsonStr)
{
    QJsonDocument doc = QJsonDocument::fromJson(QString::fromStdString(jsonStr).toUtf8());
    position = doc.object();
}

void PositionThread::setKeys(const QStringList& k)
{
    keys = k;
}

int PositionThread::getStartIndex() const
{
    return startIndex;
}

int PositionThread::getEndIndex() const
{
    return endIndex;
}

void PositionThread::run()
{
    if (position.isEmpty())
    {
        startIndex = 0;
        endIndex = 0;
        return;
    }

    QJsonObject object = position;
    QJsonValue value;
    QJsonValue startValue;
    QJsonValue endValue;

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

                startValue = object.value("start index");
                endValue = object.value("end index");

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
}

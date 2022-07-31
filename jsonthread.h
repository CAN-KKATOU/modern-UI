#ifndef JSONTHREAD_H
#define JSONTHREAD_H

#include <QThread>
#include <QModelIndex>
#include <QJsonObject>
#include <QJsonDocument>
#include "qjsonmodel.h"

class JsonThread : public QThread
{
    Q_OBJECT

public:
    explicit JsonThread(QObject *parent = nullptr);

    ~JsonThread() override;

    void setDataModel(QJsonModel*);

    void setDataObject(const std::string&);

protected:

    void run() override;

private:

    QJsonModel *dataModel;

    QJsonObject dataObject;
};

#endif // JSONTHREAD_H

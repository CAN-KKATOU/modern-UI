#ifndef QJSONPARSER_H
#define QJSONPARSER_H

#include <QObject>
#include "qjsonmodel.h"

class JsonParser : public QObject
{
    Q_OBJECT

public:
    explicit JsonParser(QObject *parent = nullptr);

    ~JsonParser() override;

    void setModel(QJsonModel *model);

signals:

    void parseFinished();

public slots:

    void parse(const QJsonObject&);

    void parseStr(const std::string&);

private:

    QJsonModel *jModel;

};

#endif // QJSONPARSER_H

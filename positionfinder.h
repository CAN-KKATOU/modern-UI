#ifndef QPOSITIONFINDER_H
#define QPOSITIONFINDER_H

#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>
#include <QThread>

class PositionFinder : public QObject
{
    Q_OBJECT

public:

    explicit PositionFinder(QObject *parent = nullptr);

    ~PositionFinder() override;

signals:

    void positionFound(int, int);

public slots:

    void getPositionJson(const QJsonObject&);

    void getPositionJsonStr(const std::string&);

    void find(QList<QString>);

private:

    QJsonObject pObject;

};

#endif // QPOSITIONFINDER_H

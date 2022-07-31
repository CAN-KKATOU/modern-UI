#ifndef POSITIONTHREAD_H
#define POSITIONTHREAD_H

#include <QThread>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

class PositionThread : public QThread
{
    Q_OBJECT

public:
    explicit PositionThread(QObject *parent = nullptr);

    ~PositionThread() override;

    void setPosition(const std::string&);

    void setKeys(const QStringList&);

    [[nodiscard]] int getStartIndex() const;

    [[nodiscard]] int getEndIndex() const;

protected:

    void run() override;

private:

    QJsonObject position;

    QStringList keys;

    int startIndex = 0;

    int endIndex = 0;
};

#endif // POSITIONTHREAD_H

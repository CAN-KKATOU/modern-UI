#ifndef OPENTHREAD_H
#define OPENTHREAD_H

#include <QThread>
#include <QFile>
#include <QIODevice>
#include <QApplication>
#include <QReadWriteLock>

class OpenThread : public QThread
{
    Q_OBJECT

public:
    explicit OpenThread(QObject *parent = nullptr);

    ~OpenThread() override;

    bool setFileName(const QString&);

    void setBuffer(QList<QString>*);

protected:

    void run() override;

private:

    QFile *file = nullptr;

    QList<QString> *buffer = nullptr;
};

#endif // OPENTHREAD_H

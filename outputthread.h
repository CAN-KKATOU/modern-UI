#ifndef OUTPUTTHREAD_H
#define OUTPUTTHREAD_H

#include <QThread>
#include <QFile>
#include <QStandardItemModel>
#include <QTextStream>
#include <QReadWriteLock>

class outputThread : public QThread
{
public:
    explicit outputThread(QObject *parent = nullptr);

    ~outputThread() override;

    void setFileName(const QString& file);

    void setBuffer(std::shared_ptr<QStringList>);

protected:

    void run() override;

private:

    std::shared_ptr<QStringList> buffer;

    QString file_name;
};

#endif // OUTPUTTHREAD_H

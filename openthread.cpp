#include "openthread.h"

QReadWriteLock rwLock;

OpenThread::OpenThread(QObject *parent)
    : QThread{parent}
{

}

OpenThread::~OpenThread()
{
    qDebug() << "OpenThead: " + tr("File has been read, this thread will be released.");
}

bool OpenThread::setFileName(const QString& fileName)
{
    file = new QFile(fileName, this);
    if (!file->open(QIODevice::ReadOnly | QIODevice::Text))
        return false;
    else
        return true;
}

void OpenThread::setBuffer(QList<QString> *b)
{
    buffer = b;
}

void OpenThread::run()
{
    if (buffer == nullptr)
    {
        qDebug() << "OpenThread: " + tr("Buffer not set!");
        return;
    }

    if (file == nullptr)
    {
        qDebug() << "OpenThread: " + tr("File not set!");
        return;
    }

    QTextStream in(file);

    while(!in.atEnd())
    {
        QWriteLocker lock(&rwLock);
        QString line = in.readLine();
        QString endLine = "---------------------------------------------------";
        if (!line.isEmpty() and line.compare(endLine) != 0)
        {
            buffer->append(line);
        }
    }

    file->close();
    delete file;

    qDebug() << " buffer size: " << buffer->size();
}

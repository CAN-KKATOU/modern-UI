#include "outputthread.h"

outputThread::outputThread(QObject *parent)
    : QThread{parent}
{

}

outputThread::~outputThread()
{
    buffer->clear();
    qDebug() << "outputThread: " + tr("This thread will be released.");
}

void outputThread::setFileName(const QString &file)
{
    this->file_name = file;
}

void outputThread::setBuffer(std::shared_ptr<QStringList> b)
{
    this->buffer = std::move(b);

    qDebug() << "Optput buffer set!";
}

void outputThread::run()
{
    auto file = new QFile(file_name);
    if (!file->open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(file);

    while (!buffer->isEmpty())
    {
        out << buffer->takeFirst();
    }
    out.flush();
    file->close();
}

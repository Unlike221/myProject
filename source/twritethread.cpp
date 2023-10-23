#include "twritethread.h"
#include <QTextStream>
#include <QFile>
#include <QDebug>

TWriteThread::TWriteThread(QObject *parent)
    : QThread{parent}
{

}

void TWriteThread::run()
{
    QFile file(m_fileName);
    if(file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&file);
        for(int i=0; i<100; ++i)
        {
            emit writing(i);
            out << "你好，“文件”\n";
            msleep(500);
        }
    }else
    {
        qDebug()<< "file open fail";
    }
    file.close();
}

#ifndef TWRITETHREAD_H
#define TWRITETHREAD_H

#include <QThread>
#include <QObject>

class TWriteThread : public QThread
{
    Q_OBJECT
public:
    explicit TWriteThread(QObject *parent = nullptr);

    // QThread interface
protected:
    virtual void run();

public:
    QString m_fileName;

signals:
    void writing(int schedule);
};



#endif // TWRITETHREAD_H

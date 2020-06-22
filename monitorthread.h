#ifndef MONITORTHREAD_H
#define MONITORTHREAD_H

#include <QObject>
#include <QThread>
#include <QMutex>
#include <stdio.h>
#include <Windows.h>
#include <OpenNR-IF.h>


class monitorThread : public QThread
{
    Q_OBJECT
private:
    bool m_isRun;
    int openID;
    float nr_JAngle[6]={0};

signals:
   void sendJAngle(float *);
   void idReceived();

protected:
    void run();//不能直接调用，调用start()方法间接调用

public:
    monitorThread(QObject* parent);
    void stop();
    void receiveID(int);
};

#endif // MONITORTHREAD_H

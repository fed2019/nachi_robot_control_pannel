#ifndef RECORDTHREAD_H
#define RECORDTHREAD_H

#include <QObject>
#include <QThread>
#include <stdio.h>
#include <Windows.h>
#include <OpenNR-IF.h>
#include <QDebug>

class recordThread : public QThread
{
    Q_OBJECT
public:
    recordThread(QObject *parent);
    void recordByTime();
    void recordByDistance();
    void recordByPoint();
    void getStatus(int);
    void getMode(int,unsigned long);

protected:
    void run();//不能直接调用，调用start()方法间接调用

signals:
    void canIStart();
    void sendJAngle(float *);

private:
   bool m_isRun;
   int recordStatus;//0-停止 1-记录 2-暂停
   int mode;//1-等时 2-等距 3-点
   int openID;
   unsigned long gap; //the gap is time or distance
   float JAngle[6]={0};



};

#endif // RECORDTHREAD_H

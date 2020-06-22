#ifndef ROBOTOPERATETHREAD_H
#define ROBOTOPERATETHREAD_H

#include <QObject>
#include <stdio.h>
#include <Windows.h>
#include <stdlib.h>
#include <OpenNR-IF.h>
#include <QDebug>
#include <QThread>

class RobotOperateThread : public QThread
{

    Q_OBJECT
private:
    int speedOverRide;
    int whichButton;
    int id;
    bool operateMode;
public:
    RobotOperateThread(QObject* parent);
    void onXMinus(int);
    void onXPlue(int);
    void onYMinus(int);
    void onYPlus(int);
    void onZMinus(int);
    void onZPlue(int);
    void onRXMinus(int);
    void onRXPlue(int);
    void onRYMinus(int);
    void onRYPlus(int);
    void onRZMinus(int);
    void onRZPlue(int);
protected:
    void run();//不能直接调用，调用start()方法间接调用
};

#endif // ROBOTOPERATETHREAD_H

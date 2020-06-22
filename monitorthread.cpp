#include "monitorthread.h"
#include <QDebug>

monitorThread::monitorThread(QObject *parent):QThread(parent)
{
    m_isRun = true;
}

void monitorThread::run()
{
    while (m_isRun)
    {
        int result=NR_AcsAxisTheta (openID, nr_JAngle, 1, 6, 3, 0.1f);
        if (result==NR_E_NORMAL)
        {
            emit sendJAngle(nr_JAngle);
        }
        else
        {continue;}
        msleep(200);
    }
    return;

}

void monitorThread::stop()
{

    m_isRun = false;
}

void monitorThread::receiveID(int id)
{
    openID=id;
    emit idReceived();
}



#include "readcamera.h"

readCamera::readCamera(QObject *parent) : QTcpSocket(parent)
{
//    connect(subThreadTimer,&QTimer::timeout,this,&readCamera::dataRequest);
}

void readCamera::connectWithCamera()
{
    this->connectToHost(QString("127.0.0.1"),8888);
}

void readCamera::disConnectWithCamera()
{
    this->disconnectFromHost();
}

void readCamera::dataRequest()
{
    QString msg="please";
    this->write(msg.toUtf8().data());
    qDebug()<<"data please";
}

void readCamera::recvData()
{
    QByteArray array=this->readAll();
    emit sendCamData(QString(array));
}




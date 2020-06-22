#ifndef READCAMERA_H
#define READCAMERA_H

#include <QObject>
#include <QTcpSocket>
#include <QDebug>
#include <QTimer>

class readCamera : public QTcpSocket
{
    Q_OBJECT
public:
    explicit readCamera(QObject *parent = nullptr);
    void connectWithCamera();
    void disConnectWithCamera();
    void dataRequest();
    void recvData();

signals:
    void sendCamData(QString);
};

#endif // READCAMERA_H

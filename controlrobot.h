#ifndef CONTROLROBOT_H
#define CONTROLROBOT_H

#include <QObject>
#include <stdio.h>
#include <Windows.h>
#include <stdlib.h>
#include <string.h>
#include <OpenNR-IF.h>
#include <QDebug>

class controlRobot : public QObject
{
    Q_OBJECT
public:
    explicit controlRobot(QObject *parent = nullptr);
    int startOpennrReal(const QString);
    void endOpennrReal(int nXmlOpenId);
    void stopExternalTracking(int nXmlOpenId, NR_GET_REAL_DATA_ALL *p_get, NR_SET_REAL_DATA_ALL *p_set);
    void setMoveMode(float y, float z, NR_GET_REAL_DATA_ALL *p_get, NR_SET_REAL_DATA_ALL *p_set);

signals:

public slots:
};

#endif // CONTROLROBOT_H

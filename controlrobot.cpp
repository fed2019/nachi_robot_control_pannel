#include "controlrobot.h"

controlRobot::controlRobot(QObject *parent) : QObject(parent)
{

}




int controlRobot::startOpennrReal(const QString id)
{
//    const QString id="127.0.0.1";
    QByteArray ba = id.toLatin1();
    char *ipAdress = ba.data();

    NACHI_COMMIF_INFO Info;
    ZeroMemory(&Info, sizeof(Info));
    Info.pcAddrs = ipAdress;
    Info.lCommSide = NR_OBJECT_INTERNAL;
    Info.lMode = 0;
    Info.lKind = NR_DATA_XML;
    int nXmlOpenId = NR_Open(&Info);
    return nXmlOpenId;
}

void controlRobot::setMoveMode(float y, float z, NR_GET_REAL_DATA_ALL *p_get, NR_SET_REAL_DATA_ALL *p_set)
{
    p_set->stCtrl.ushEstopBit = 0;
    p_set->stCtrl.ushFinishBit = 0;
    p_set->stCtrl.ushOrderBit = 0; //按TCP坐标运动
    p_set->ustData.stStd.fComTcpPos[0] = p_get->ustData.stStd.fCurTcpPos[0];
    p_set->ustData.stStd.fComTcpPos[1] = p_get->ustData.stStd.fCurTcpPos[1] + y;
    p_set->ustData.stStd.fComTcpPos[2] = p_get->ustData.stStd.fCurTcpPos[2] + z;
    p_set->ustData.stStd.fComTcpPos[3] = p_get->ustData.stStd.fCurTcpPos[3];
    p_set->ustData.stStd.fComTcpPos[4] = p_get->ustData.stStd.fCurTcpPos[4];
    p_set->ustData.stStd.fComTcpPos[5] = p_get->ustData.stStd.fCurTcpPos[5];
    qDebug()<< "模式设置成功";
}

void controlRobot::stopExternalTracking(int nXmlOpenId, NR_GET_REAL_DATA_ALL *p_get, NR_SET_REAL_DATA_ALL *p_set)
{
    p_set->stCtrl.ushFinishBit = 1;
    NR_GetAll(nXmlOpenId, p_get, NR_ACCESS_NO_WAIT);
    for (int i = 0; i < 8; i++)
    {
        p_set->ustData.stStd.fComTcpPos[i] = p_get->ustData.stStd.fCurTcpPos[i];
    }
    p_set->stCtrl.ushOrderBit = 0; //按TCP坐标运动
    NR_SetAll(nXmlOpenId, p_set, NR_ACCESS_NO_WAIT);
}

void controlRobot::endOpennrReal(int nXmlOpenId)
{
    NR_Close(nXmlOpenId);
    return;
}


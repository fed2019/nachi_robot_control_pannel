#ifndef NRMETHOD_H
#define NRMETHOD_H
#include <iostream>
#include <stdio.h>
#include <Windows.h>
#include <stdlib.h>
#include <string.h>
#include <OpenNR-IF.h>
#include <QDebug>


#define JOINT_SIZE (6)


void startOpennrReal(QString id);
void endOpennrReal(int nXmlOpenId);
void stopExternalTracking(int nXmlOpenId, NR_GET_REAL_DATA_ALL *p_get, NR_SET_REAL_DATA_ALL *p_set);
void setMoveMode(float y, float z, NR_GET_REAL_DATA_ALL *p_get, NR_SET_REAL_DATA_ALL *p_set);


#endif // NRMETHOD_H

#include "recordthread.h"

recordThread::recordThread(QObject *parent) :QThread(parent)
{
    recordStatus=1;
    gap=2000;

}

void recordThread::recordByTime()
{
    int count_T=0;
    int result;
    float JAngle[6]={0};
//    ui->table1->clearContents();//首先清除表格内容
    while(recordStatus>0)
    {

        while(recordStatus==1)
        {
            result=NR_AcsAxisTheta (openID, JAngle, 1, 6, 3, 0.1f);
            qDebug()<<"result:"<<result;
            if (result==NR_E_NORMAL)
            {
//                qDebug()<<"setpos";
//                pos[count_T][0]=JAngle[0];
//                pos[count_T][1]=JAngle[1];
//                pos[count_T][2]=JAngle[2];
//                pos[count_T][3]=JAngle[3];
//                pos[count_T][4]=JAngle[4];
//                pos[count_T][5]=JAngle[5];

////                ui->table1->insertRow(1);
//                ui->table1->setItem(count_T,0,new QTableWidgetItem(QString::number(count_T)));
//                ui->table1->setItem(count_T,1,new QTableWidgetItem(QString::number((JAngle[0],'f',2))));
//                ui->table1->setItem(count_T,2,new QTableWidgetItem(QString::number(JAngle[1],'f',2)));
//                ui->table1->setItem(count_T,3,new QTableWidgetItem(QString::number(JAngle[2],'f',2)));
//                ui->table1->setItem(count_T,4,new QTableWidgetItem(QString::number(JAngle[3],'f',2)));
//                ui->table1->setItem(count_T,5,new QTableWidgetItem(QString::number(JAngle[4],'f',2)));
//                ui->table1->setItem(count_T,6,new QTableWidgetItem(QString::number(JAngle[5],'f',2)));

            }
            else
            {continue;}
            count_T++;
            qDebug()<<"count_T:"<<count_T;
//            ui->teacPointNum->setText(QString::number(count_T));
//            msleep(time_gap);
        }
        while(recordStatus==2)
        {
            QThread::msleep(100);
        }

    }
}

void recordThread::getStatus(int st)
{
    recordStatus=st;
}

void recordThread::getMode(int m, unsigned long g)
{
    mode=m;
    gap=g;
}


#include "controlpannel.h"
#include "ui_controlpannel.h"

controlPannel::controlPannel(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::controlPannel)
{

    ui->setupUi(this);
    //全局变量的初始化
    open_ID=0;
    robot =new controlRobot(this);
    count_time=0;
    count_distance=0;
    count_point=0;
    timer1 =new QTimer(this);
    timer2=new QTimer(this);
    timer3 =new QTimer(this);
    timer4=new QTimer(this);
    timer5=new QTimer(this);
    timer6=new QTimer(this);
    subwin=new subwindow();
    readCam=new readCamera();

    //窗口的初始化
    resize(1066,600);
    ui->ip->setText(readInit("IP"));
    ui->table1->setColumnCount(6);//列数
    ui->table1->setHorizontalHeaderLabels(QStringList()<<"J1"<<"J2"<<"J3"<<"J4"<<"J5"<<"J6");
    ui->table1->setStyleSheet("QTableWidget::item{border:1px solid ;}");
    ui->table1->verticalHeader()->setDefaultSectionSize(10);
    ui->table1->verticalHeader()->setHighlightSections(true);
    ui->rdb_time->setChecked(true);
    ui->lineEdit_time->setText("2");
    ui->lineEdit_distance->setText("10");
    ui->teacPointNum->setText(QString("0"));
    ui->rdb_singleMove->setChecked(true);

    //可视化区初始化
    connect(ui->PB_link,&QPushButton::clicked,this,&controlPannel::scatterLink);
    connect(subwin,&subwindow::subWindowClosed,[=](){
        swich=false;
        ui->PB_link->setText("可视化开");
    });

    connect(this,&controlPannel::sendCurrentPos,subwin,&subwindow::addData);

    connect(ui->table1,&QTableWidget::cellClicked,this,[=](int x,int y){
    subwin->setCurrentItem(x);
    });

    connect(subwin,&subwindow::currentItemChanged,ui->table1,[=](int index){
        ui->table1->setCurrentCell(index,0);
        });

    connect(this,&controlPannel::deletedPos,subwin,&subwindow::deleteData);
    connect(this,&controlPannel::updatedPos,subwin,&subwindow::modifyData);

    //按钮逻辑实现 连接区
    connect(ui->NRConnectOrDis,&QPushButton::clicked,this,&controlPannel::connectOrDisConnect);
    connect(timer1, &QTimer::timeout, this, &controlPannel::angleMonitor);
    connect(ui->PB_Stop,&QPushButton::clicked,this,&controlPannel::stopRecord);

    //按钮逻辑实现 记录区
    connect(ui->PB_Start,&QPushButton::clicked,this,&controlPannel::startRecord);
    connect(timer2, &QTimer::timeout, this, &controlPannel::recordByTime);
    connect(timer3, &QTimer::timeout, this, &controlPannel::recordByDistance);
    connect(timer4, &QTimer::timeout, this, &controlPannel::recordByPoint);

    //按钮逻辑实现 控制区
    connect(ui->PB_ON,&QPushButton::clicked,this,&controlPannel::servoOn);
    connect(ui->PB_OFF,&QPushButton::clicked,this,&controlPannel::servoOff);
    connect(ui->PB_XDown,&QPushButton::pressed,this,&controlPannel::onXMinus);
    connect(ui->PB_XUp,&QPushButton::pressed,this,&controlPannel::onXPlus);
    connect(ui->PB_YDown,&QPushButton::pressed,this,&controlPannel::onYMinus);
    connect(ui->PB_YUp,&QPushButton::pressed,this,&controlPannel::onYPlus);
    connect(ui->PB_ZDown,&QPushButton::pressed,this,&controlPannel::onZMinus);
    connect(ui->PB_ZUp,&QPushButton::pressed,this,&controlPannel::onZPlus);
    connect(ui->PB_RXDown,&QPushButton::pressed,this,&controlPannel::onRXMinus);
    connect(ui->PB_RXUp,&QPushButton::pressed,this,&controlPannel::onRXPlus);
    connect(ui->PB_RYDown,&QPushButton::pressed,this,&controlPannel::onRYMinus);
    connect(ui->PB_RYUp,&QPushButton::pressed,this,&controlPannel::onRYPlus);
    connect(ui->PB_RZDown,&QPushButton::pressed,this,&controlPannel::onRZMinus);
    connect(ui->PB_RZUp,&QPushButton::pressed,this,&controlPannel::onRZPlus);
    connect(timer6,&QTimer::timeout,this,&controlPannel::onTimerGo);
    connect(ui->PB_XDown,&QPushButton::released,timer6,&QTimer::stop);
    connect(ui->PB_XUp,&QPushButton::released,timer6,&QTimer::stop);
    connect(ui->PB_YDown,&QPushButton::released,timer6,&QTimer::stop);
    connect(ui->PB_YUp,&QPushButton::released,timer6,&QTimer::stop);
    connect(ui->PB_ZDown,&QPushButton::released,timer6,&QTimer::stop);
    connect(ui->PB_ZUp,&QPushButton::released,timer6,&QTimer::stop);
    connect(ui->PB_RXDown,&QPushButton::released,timer6,&QTimer::stop);
    connect(ui->PB_RXUp,&QPushButton::released,timer6,&QTimer::stop);
    connect(ui->PB_RYDown,&QPushButton::released,timer6,&QTimer::stop);
    connect(ui->PB_RYUp,&QPushButton::released,timer6,&QTimer::stop);
    connect(ui->PB_RZDown,&QPushButton::released,timer6,&QTimer::stop);
    connect(ui->PB_RZUp,&QPushButton::released,timer6,&QTimer::stop);

    //按钮逻辑实现 数据处理区
    connect(ui->PB_add,&QPushButton::clicked,this,&controlPannel::addPos);
    connect(ui->PB_insert,&QPushButton::clicked,this,&controlPannel::insertPos);
    connect(ui->PB_delete,&QPushButton::clicked,this,&controlPannel::deletePos);
    connect(ui->PB_modify,&QPushButton::clicked,this,&controlPannel::modifyPos);
    connect(ui->PB_save,&QPushButton::clicked,this,&controlPannel::savePoseData);
    connect(ui->PB_send,&QPushButton::clicked,this,&controlPannel::sendRobotProgram);

    //人脸控制区
    faceControlThread=new QThread();
    readCam->moveToThread(faceControlThread);
    faceControlThread->start();
    connect(ui->PB_faceON,&QPushButton::clicked,readCam,&readCamera::connectWithCamera);
    connect(ui->PB_faceOFF,&QPushButton::clicked,readCam,&readCamera::disConnectWithCamera);
    connect(readCam,&readCamera::connected,this,[=](){
        int coordType=1;//1-Machine
        int manualSpeed=5;
        NR_AcsManualCoordinateType(open_ID,&coordType,1);
        NR_AcsManualSpeed(open_ID,&manualSpeed,1);
        ui->sysMessage->setText("message:与相机连接成功!");
        ui->PB_faceON->setEnabled(false);
        ui->PB_faceOFF->setEnabled(true);
        timer5->start(40);});
    connect(readCam,&readCamera::disconnected,this,[=](){
        ui->sysMessage->setText("message:断开与相机的连接!");
        ui->PB_faceON->setEnabled(true);
        ui->PB_faceOFF->setEnabled(false);
        timer5->stop();});
    connect(timer5,&QTimer::timeout,readCam,&readCamera::dataRequest);
    connect(readCam, &readCamera::readyRead,readCam,&readCamera::recvData);
    connect(readCam, &readCamera::sendCamData,this,&controlPannel::handleCamData);
    connect(this,&controlPannel::faceMoved,this,&controlPannel::onTimerGo);
}

controlPannel::~controlPannel()
{
    delete ui;
}

//关闭主窗口时要做的操作
void controlPannel::closeEvent(QCloseEvent *event)
 {
    BOOL bValue=FALSE;
    NR_AcsFixedIOServoOn(open_ID,&bValue);
    if(bValue == TRUE)
    {
    NR_CtrlMotor(open_ID, 0); //如果伺服ON,则将其断掉
    }

    robot->endOpennrReal(open_ID);//中断与机器人的连接
    delete subwin;//释放子窗口对象
    if(faceControlThread->isRunning())
    {
        faceControlThread->QThread::exit();//如果人脸控制线程正在运行，则将其关闭
    }

    writeInit("IP", ui->ip->text());//将最新Ip保存
 }

void controlPannel::connectOrDisConnect()
{
    static int openId=0;
    static int NRConnectOrDis_switch=1;//连接按钮的功能旋转开关
    if(NRConnectOrDis_switch==1)
    {
        openId=robot->startOpennrReal(ui->ip->text());
        if (openId>0)
        {
            ui->sysMessage->setText("system status message:与机器人连接成功!");
            ui->NRConnectOrDis->setText("断开");
            NRConnectOrDis_switch=0;
            open_ID=openId;
            ui->PB_Start->setEnabled(true);
            ui->PB_ON->setEnabled(true);
            ui->PB_send->setEnabled(true);

            //连接成功，启动定时器1，开启监视器
//           LPSTR value;
//           int nrr=NR_AcsStrInputSignalNameA(open_ID,&value,1000,2,1);
//           if(NR_E_NORMAL==nrr)
//           {
//               qDebug()<<"ok";
//               QString content=QString(QLatin1String(value));
//               int l=content.length();
//               qDebug()<<QString::number(l);
//           }

           timer1->start(200);
        }
        else
        {
            ui->sysMessage->setText("system error message:与机器人连接失败!");
            open_ID=openId;
        }
    }
    else
    {
        //中断连接 先停止轴监视器
        timer1->stop();
        ui->NRConnectOrDis->setText("连接");
        ui->sysMessage->setText("system error message:断开与机器人的连接！");
        NRConnectOrDis_switch=1;
        //中断连接
        robot->endOpennrReal(openId);
        ui->PB_Start->setEnabled(false);
        ui->PB_ON->setEnabled(false);
        ui->PB_add->setEnabled(false);
        ui->PB_insert->setEnabled(false);
        ui->PB_delete->setEnabled(false);
        ui->PB_modify->setEnabled(false);
        ui->PB_save->setEnabled(false);
        ui->PB_send->setEnabled(false);
        ui->PB_faceON->setEnabled(false);
        ui->PB_faceOFF->setEnabled(false);
        ui->PB_XUp->setEnabled(false);
        ui->PB_XDown->setEnabled(false);
        ui->PB_YUp->setEnabled(false);
        ui->PB_YDown->setEnabled(false);
        ui->PB_ZUp->setEnabled(false);
        ui->PB_ZDown->setEnabled(false);
        ui->PB_RXUp->setEnabled(false);
        ui->PB_RXDown->setEnabled(false);
        ui->PB_RYUp->setEnabled(false);
        ui->PB_RYDown->setEnabled(false);
        ui->PB_RZUp->setEnabled(false);
        ui->PB_RZDown->setEnabled(false);
    }

}


void controlPannel::angleMonitor()
{
    static float angle[6];
    int result=NR_AcsAxisTheta (open_ID, angle, 1, 6, 3, 0.1f);
    if (result==NR_E_NORMAL)
    {
        ui->textBrowser_J1->setText(QString::number(angle[0],'f',2));
        ui->textBrowser_J2->setText(QString::number(angle[1],'f',2));
        ui->textBrowser_J3->setText(QString::number(angle[2],'f',2));
        ui->textBrowser_J4->setText(QString::number(angle[3],'f',2));
        ui->textBrowser_J5->setText(QString::number(angle[4],'f',2));
        ui->textBrowser_J6->setText(QString::number(angle[5],'f',2));
    }
    else
    {return;}
}

void controlPannel::startRecord()
{

    if(ui->rdb_time->isChecked())
    {
        if(QMessageBox::question(this,"Info",QString("record by time gapped with %1s?").arg(ui->lineEdit_time->text()),\
                              QMessageBox::Yes|QMessageBox::No)==QMessageBox::No)
        {
            return;
        }
        else
        {
            ui->PB_add->setEnabled(false);
            ui->PB_insert->setEnabled(false);
            ui->PB_delete->setEnabled(false);
            ui->PB_modify->setEnabled(false);
            ui->PB_save->setEnabled(false);
            if(count_distance!=0||count_point!=0)
            {
                ui->table1->clearContents();
                ui->teacPointNum->setText("0");
                subwin->deleteAllData();
                count_distance=0;
                count_point=0;
            }
            int gap=ui->lineEdit_time->text().toFloat()*1000;
            timer2->start(gap);
            ui->PB_Stop->setEnabled(true);
        }


    }
    else if(ui->rdb_distance->isChecked())
    {
        if(QMessageBox::question(this,"Info",QString("record by distance gapped with %1mm?").arg(ui->lineEdit_distance->text()),QMessageBox::Yes|QMessageBox::No)==QMessageBox::No)
        {
            return;
        }
        else
        {
            ui->PB_add->setEnabled(false);
            ui->PB_insert->setEnabled(false);
            ui->PB_delete->setEnabled(false);
            ui->PB_modify->setEnabled(false);
            ui->PB_save->setEnabled(false);
            if(count_time!=0||count_point!=0)
            {
                ui->table1->clearContents();
                ui->teacPointNum->setText("0");
                subwin->deleteAllData();
                count_time=0;
                count_point=0;
            }
            if(count_distance==0)  //如果是第一个点，则记录起始点
            {
                float angle_curr[6]={0};
                float pos_curr[6]={0};
                int result1=NR_AcsAxisTheta (open_ID, angle_curr, 1, 6, 3, 0.1f);
                int result2=NR_AcsToolTipPos(open_ID, pos_curr, 1, 6, 3, 0.1f);
                if (result1==NR_E_NORMAL&&result2==NR_E_NORMAL)
                {
                    posDistanceList.append(posFormat(angle_curr));
                    for(int j=0;j<6;j++){pastPos[j]=pos_curr[j];}
                    addToTable(count_distance,angle_curr);
                    count_distance++;
                    ui->teacPointNum->setText(QString::number(count_distance));
                    emit sendCurrentPos(pos_curr,-1);
                    timer3->start(500);
                    ui->PB_Stop->setEnabled(true);
                }
                else
                {return;}
            }
            else
            {
                timer3->start(50);
                ui->PB_Stop->setEnabled(true);
            }
        }
    }
    else
    {
        if(QMessageBox::question(this,"Info",\
                              QString("record by point?"),\
                              QMessageBox::Yes|QMessageBox::No)==QMessageBox::No)
        {
            return;
        }
        else
        {
            ui->PB_add->setEnabled(false);
            ui->PB_insert->setEnabled(false);
            ui->PB_delete->setEnabled(false);
            ui->PB_modify->setEnabled(false);
            ui->PB_save->setEnabled(false);
            if(count_distance!=0||count_time!=0)
            {
                ui->table1->clearContents();
                ui->teacPointNum->setText("0");
                subwin->deleteAllData();
                count_distance=0;
                count_time=0;
            }
            if(count_point==0)  //如果是第一个点，则记录起始点
            {
                float angle_curr[6]={0};
                int result1=NR_AcsAxisTheta (open_ID, angle_curr, 1, 6, 3, 0.1f);
                int result2=NR_AcsToolTipPos(open_ID, pastPos, 1, 6, 3, 0.1f);
                pastTime=QDateTime::currentMSecsSinceEpoch();
                if (result1==NR_E_NORMAL&&result2==NR_E_NORMAL)
                {
                    timer4->start(50);
                    ui->PB_Stop->setEnabled(true);
                }
                else
                {return;}
            }
            else
            {
                timer4->start(50);
                ui->PB_Stop->setEnabled(true);
            }
        }
    }
}

void controlPannel::recordByTime()
{

    float angle_curr[6]={0};
    float pos_curr[6]={0};
    int result1=NR_AcsAxisTheta (open_ID, angle_curr, 1, 6, 3, 0.1f);
    int result2=NR_AcsToolTipPos(open_ID, pos_curr, 1, 6, 3, 0.1f);
    if (result1==NR_E_NORMAL&&result2==NR_E_NORMAL)
    {
        posTimeList.append(posFormat(angle_curr));
        addToTable(count_time,angle_curr);
        count_time++;
        ui->teacPointNum->setText(QString::number(count_time));
        emit sendCurrentPos(pos_curr,-1);
        if (count_distance>=9999)
        {
            ui->sysMessage->setText("Error:记录点数超上限10000!");
            stopRecord();
        }
    }
    else
    {return;}
}

void controlPannel::recordByDistance()
{
    float angle_curr[6]={0};
    float pos_curr[6]={0};
    int result1=NR_AcsAxisTheta (open_ID, angle_curr, 1, 6, 3, 0.1f);
    int result2=NR_AcsToolTipPos(open_ID, pos_curr, 1, 6, 3, 0.1f);
    if (result1==NR_E_NORMAL&&result2==NR_E_NORMAL)
    {
        double distance=qSqrt((pos_curr[0]-pastPos[0])*(pos_curr[0]-pastPos[0])
                +(pos_curr[1]-pastPos[1])*(pos_curr[1]-pastPos[1])
                +(pos_curr[2]-pastPos[2])*(pos_curr[2]-pastPos[2]));
        if(distance<ui->lineEdit_distance->text().toFloat())
        {return;}
        else
        {
            for(int i=0;i<6;i++){pastPos[i]=pos_curr[i];}
            posDistanceList.append(posFormat(angle_curr));
            addToTable(count_distance,angle_curr);
            count_distance++;
            ui->teacPointNum->setText(QString::number(count_distance));
            emit sendCurrentPos(pos_curr,-1);
            if (count_distance>=9999)
            {
                ui->sysMessage->setText("Error:记录点数超上限10000!");
                stopRecord();
            }
        }
    }
    else
    {return;}
}

void controlPannel::recordByPoint()
{
    float angle_curr[6]={0};
    float pos_curr[6]={0};
    int result1=NR_AcsAxisTheta (open_ID, angle_curr, 1, 6, 3, 0.1f);
    int result2=NR_AcsToolTipPos(open_ID, pos_curr, 1, 6, 3, 0.1f);
    qint64 currentTime=QDateTime::currentMSecsSinceEpoch();
    if (result1==NR_E_NORMAL&&result2==NR_E_NORMAL)
    {
        double distance=qSqrt((pos_curr[0]-pastPos[0])*(pos_curr[0]-pastPos[0])
                +(pos_curr[1]-pastPos[1])*(pos_curr[1]-pastPos[1])
                +(pos_curr[2]-pastPos[2])*(pos_curr[2]-pastPos[2]));
        if(distance<0.2) //前一点跟后一点直线距离小于0.2则认为机器人没有动
        {
            if((currentTime-pastTime)>=4000)//判断机器人是否停留4000毫秒以上
            {
                pastTime=currentTime;
                for(int i=0;i<6;i++){pastPos[i]=pos_curr[i];}
                posPointList.append(posFormat(angle_curr));
                addToTable(count_point,angle_curr);
                count_point++;
                ui->teacPointNum->setText(QString::number(count_point));
                emit sendCurrentPos(pos_curr,-1);
                if (count_point>=9999)
                {
                    ui->sysMessage->setText("Error:记录点数超上限10000!");
                    stopRecord();
                }
            }
            else  //没动，但是未停留4000毫秒
            {
                return;
            }
        }
        else //反之，机器人动了
        {
            pastTime=currentTime;
            for(int i=0;i<6;i++){pastPos[i]=pos_curr[i];}
        }
        timer4->start(50);
        ui->PB_Stop->setEnabled(true);
    }

}

void controlPannel::stopRecord()
{

    if(timer2->isActive())
    {
        timer2->stop();
    }
    if(timer3->isActive())
    {
        timer3->stop();
    }
    if(timer4->isActive())
    {
        timer4->stop();
    }

    ui->PB_Stop->setEnabled(false);
    ui->PB_add->setEnabled(true);
    ui->PB_insert->setEnabled(true);
    ui->PB_delete->setEnabled(true);
    ui->PB_modify->setEnabled(true);
    ui->PB_save->setEnabled(true);
    ui->PB_send->setEnabled(true);
}

void controlPannel::servoOn()
{
    int result=NR_CtrlMotor(open_ID, 1);
    if(NR_E_NORMAL != result)
    {
    ui->sysMessage->setText("Error:启动伺服失败!");
    return;
    }
    else
    {
        ui->sysMessage->setText("Message:启动伺服成功!");
        //设置运动参数
        int speed[1]={50};
        int accuracy[1]={8};
        int interPolationKind[1]={1};
        NR_AcsRecordSpeed(open_ID, speed, true, 1);
        NR_AcsRecordAccuracyNo(open_ID, accuracy, true, 1);
        NR_AcsInterpolationKind(open_ID, interPolationKind, true, 1);

        ui->PB_ON->setEnabled(false);
        ui->PB_OFF->setEnabled(true);
        ui->PB_faceON->setEnabled(true);
        ui->PB_XUp->setEnabled(true);
        ui->PB_XDown->setEnabled(true);
        ui->PB_YUp->setEnabled(true);
        ui->PB_YDown->setEnabled(true);
        ui->PB_ZUp->setEnabled(true);
        ui->PB_ZDown->setEnabled(true);
        ui->PB_RXUp->setEnabled(true);
        ui->PB_RXDown->setEnabled(true);
        ui->PB_RYUp->setEnabled(true);
        ui->PB_RYDown->setEnabled(true);
        ui->PB_RZUp->setEnabled(true);
        ui->PB_RZDown->setEnabled(true);
    }

}

void controlPannel::servoOff()
{
    int result=NR_CtrlMotor(open_ID, 0);
    if(NR_E_NORMAL != result)
    {
    ui->sysMessage->setText("Error:关闭伺服失败!");
    }
    else
    {
        ui->PB_ON->setEnabled(true);
        ui->PB_faceON->setEnabled(false);
        ui->PB_faceOFF->setEnabled(false);
        ui->PB_OFF->setEnabled(false);
        ui->PB_XUp->setEnabled(false);
        ui->PB_XDown->setEnabled(false);
        ui->PB_YUp->setEnabled(false);
        ui->PB_YDown->setEnabled(false);
        ui->PB_ZUp->setEnabled(false);
        ui->PB_ZDown->setEnabled(false);
        ui->PB_RXUp->setEnabled(false);
        ui->PB_RXDown->setEnabled(false);
        ui->PB_RYUp->setEnabled(false);
        ui->PB_RYDown->setEnabled(false);
        ui->PB_RZUp->setEnabled(false);
        ui->PB_RZDown->setEnabled(false);
    }
}

void controlPannel::onXMinus()
{
    int coordType=0;//0-joint
    int speedLevel=ui->slider_speed->value();
    if (ui->rdb_unionMove->isChecked())
    {
        coordType=1;//1-Machine      
    }
    NR_AcsManualCoordinateType(open_ID,&coordType,1);
    NR_AcsManualSpeed(open_ID,&speedLevel,1);
    m_Pose=returnNrPose(1,speedLevel);
    NR_CtrlJog(open_ID, &m_Pose,1);
    timer6->start(TIME_GAP);
//    if(NR_E_NORMAL != nErr)
//    {
//        ui->sysMessage->setText(QString("Error:%1").arg(nErr));
//    }
}

void controlPannel::onXPlus()
{
    int coordType=0;//0-joint
    int speedLevel=ui->slider_speed->value();
    if (ui->rdb_unionMove->isChecked())
    {
        coordType=1;//1-Machine
    }
    NR_AcsManualCoordinateType(open_ID,&coordType,1);
    NR_AcsManualSpeed(open_ID,&speedLevel,1);
    m_Pose=returnNrPose(2,speedLevel);
    NR_CtrlJog(open_ID, &m_Pose,1);
    timer6->start(TIME_GAP);
}

void controlPannel::onYMinus()
{
    int coordType=0;//0-joint
    int speedLevel=ui->slider_speed->value();
    if (ui->rdb_unionMove->isChecked())
    {
        coordType=1;//1-Machine
    }
    NR_AcsManualCoordinateType(open_ID,&coordType,1);
    NR_AcsManualSpeed(open_ID,&speedLevel,1);
    m_Pose=returnNrPose(3,speedLevel);
    NR_CtrlJog(open_ID, &m_Pose,1);
    timer6->start(TIME_GAP);
}

void controlPannel::onYPlus()
{
    int coordType=0;//0-joint
    int speedLevel=ui->slider_speed->value();
    if (ui->rdb_unionMove->isChecked())
    {
        coordType=1;//1-Machine
    }
    NR_AcsManualCoordinateType(open_ID,&coordType,1);
    NR_AcsManualSpeed(open_ID,&speedLevel,1);
    m_Pose=returnNrPose(4,speedLevel);
    NR_CtrlJog(open_ID, &m_Pose,1);
    timer6->start(TIME_GAP);
}

void controlPannel::onZMinus()
{

    int coordType=0;//0-joint
    int speedLevel=ui->slider_speed->value();
    if (ui->rdb_unionMove->isChecked())
    {
        coordType=1;//1-Machine
    }
    NR_AcsManualCoordinateType(open_ID,&coordType,1);
    NR_AcsManualSpeed(open_ID,&speedLevel,1);
    m_Pose=returnNrPose(5,speedLevel);
    NR_CtrlJog(open_ID, &m_Pose,1);
    timer6->start(TIME_GAP);
}

void controlPannel::onZPlus()
{

    int coordType=0;//0-joint
    int speedLevel=ui->slider_speed->value();
    if (ui->rdb_unionMove->isChecked())
    {
        coordType=1;//1-Machine
    }
    NR_AcsManualCoordinateType(open_ID,&coordType,1);
    NR_AcsManualSpeed(open_ID,&speedLevel,1);
    m_Pose=returnNrPose(6,speedLevel);
    NR_CtrlJog(open_ID, &m_Pose,1);
    timer6->start(TIME_GAP);
}

void controlPannel::onRXMinus()
{
    int coordType=0;//0-joint
    int speedLevel=ui->slider_speed->value();
    if (ui->rdb_unionMove->isChecked())
    {
        coordType=1;//1-Machine
    }
    NR_AcsManualCoordinateType(open_ID,&coordType,1);
    NR_AcsManualSpeed(open_ID,&speedLevel,1);
    m_Pose=returnNrPose(7,speedLevel);
    NR_CtrlJog(open_ID, &m_Pose,1);
    timer6->start(TIME_GAP);
}

void controlPannel::onRXPlus()
{
    int coordType=0;//0-joint
    int speedLevel=ui->slider_speed->value();
    if (ui->rdb_unionMove->isChecked())
    {
        coordType=1;//1-Machine
    }
    NR_AcsManualCoordinateType(open_ID,&coordType,1);
    NR_AcsManualSpeed(open_ID,&speedLevel,1);
    m_Pose=returnNrPose(8,speedLevel);
    NR_CtrlJog(open_ID, &m_Pose,1);
    timer6->start(TIME_GAP);
}
void controlPannel::onRYMinus()
{
    int coordType=0;//0-joint
    int speedLevel=ui->slider_speed->value();
    if (ui->rdb_unionMove->isChecked())
    {
        coordType=1;//1-Machine
    }
    NR_AcsManualCoordinateType(open_ID,&coordType,1);
    NR_AcsManualSpeed(open_ID,&speedLevel,1);
    m_Pose=returnNrPose(9,speedLevel);
    NR_CtrlJog(open_ID, &m_Pose,1);
    timer6->start(TIME_GAP);
}

void controlPannel::onRYPlus()
{
    int coordType=0;//0-joint
    int speedLevel=ui->slider_speed->value();
    if (ui->rdb_unionMove->isChecked())
    {
        coordType=1;//1-Machine
    }
    NR_AcsManualCoordinateType(open_ID,&coordType,1);
    NR_AcsManualSpeed(open_ID,&speedLevel,1);
    m_Pose=returnNrPose(10,speedLevel);
    NR_CtrlJog(open_ID, &m_Pose,1);
    timer6->start(TIME_GAP);
}
void controlPannel::onRZMinus()
{
    int coordType=0;//0-joint
    int speedLevel=ui->slider_speed->value();
    if (ui->rdb_unionMove->isChecked())
    {
        coordType=1;//1-Machine
    }
    NR_AcsManualCoordinateType(open_ID,&coordType,1);
    NR_AcsManualSpeed(open_ID,&speedLevel,1);
    m_Pose=returnNrPose(11,speedLevel);
    NR_CtrlJog(open_ID, &m_Pose,1);
    timer6->start(TIME_GAP);
}

void controlPannel::onRZPlus()
{
    int coordType=0;//0-joint
    int speedLevel=ui->slider_speed->value();
    if (ui->rdb_unionMove->isChecked())
    {
        coordType=1;//1-Machine
    }
    NR_AcsManualCoordinateType(open_ID,&coordType,1);
    NR_AcsManualSpeed(open_ID,&speedLevel,1);
    m_Pose=returnNrPose(12,speedLevel);
    NR_CtrlJog(open_ID, &m_Pose,1);
    timer6->start(TIME_GAP);
}

void controlPannel::onTimerGo()
{
    NR_CtrlJog(open_ID, &m_Pose,1);
}

void controlPannel::addPos()
{
    float angle_curr[6]={0};
    float pos_curr[6]={0};
    int result1=NR_AcsAxisTheta (open_ID, angle_curr, 1, 6, 3, 0.1f);
    int result2=NR_AcsToolTipPos(open_ID, pos_curr, 1, 6, 3, 0.1f);
    if (result1==NR_E_NORMAL&&result2==NR_E_NORMAL)
    {
        if(ui->rdb_time->isChecked())
        {
            posTimeList.append(posFormat(angle_curr));
            addToTable(count_time,angle_curr);
            count_time++;
            ui->teacPointNum->setText(QString::number(count_time));
            emit sendCurrentPos(pos_curr,-1); //发送点位坐标及索引
            if (count_time>=9999)
            {
                ui->sysMessage->setText("Error:记录点数超上限10000!");
            }
        }
        else if(ui->rdb_distance->isChecked())
        {
            posDistanceList.append(posFormat(angle_curr));
            addToTable(count_distance,angle_curr);
            count_distance++;
            ui->teacPointNum->setText(QString::number(count_distance));
            emit sendCurrentPos(pos_curr,-1);
            if (count_distance>=9999)
            {
                ui->sysMessage->setText("Error:记录点数超上限10000!");
            }
         }
        else
        {
            posPointList.append(posFormat(angle_curr));
            addToTable(count_point,angle_curr);
            count_point++;
            ui->teacPointNum->setText(QString::number(count_point));
            emit sendCurrentPos(pos_curr,-1);
            if (count_point>=9999)
            {
                ui->sysMessage->setText("Error:记录点数超上限10000!");
            }
        }
    }
    else {return;}
}

void controlPannel::insertPos()
{
    int currentRow=ui->table1->currentRow();
    float angle_curr[6]={0};
    float pos_curr[6]={0};
    int result1=NR_AcsAxisTheta (open_ID, angle_curr, 1, 6, 3, 0.1f);
    int result2=NR_AcsToolTipPos(open_ID, pos_curr, 1, 6, 3, 0.1f);
    if (result1==NR_E_NORMAL&&result2==NR_E_NORMAL)
    {
        if(ui->rdb_time->isChecked())
        {
            posTimeList.insert(currentRow,posFormat(angle_curr));
            addToTable(currentRow,angle_curr);
            count_time++;
            ui->teacPointNum->setText(QString::number(count_time));
            emit sendCurrentPos(pos_curr,currentRow);//发送点坐标及索引
            if (count_time>=9999)
            {
                ui->sysMessage->setText("Error:记录点数超上限10000!");
            }
        }
        else if(ui->rdb_distance->isChecked())
        {
            posDistanceList.insert(currentRow,posFormat(angle_curr));
            addToTable(currentRow,angle_curr);
            count_distance++;
            ui->teacPointNum->setText(QString::number(count_distance));
            emit sendCurrentPos(pos_curr,currentRow);
            if (count_distance>=9999)
            {
                ui->sysMessage->setText("Error:记录点数超上限10000!");
            }
         }
        else
        {
            posPointList.insert(currentRow,posFormat(angle_curr));
            addToTable(currentRow,angle_curr);
            count_point++;
            ui->teacPointNum->setText(QString::number(count_point));
            emit sendCurrentPos(pos_curr,currentRow);
            if (count_point>=9999)
            {
                ui->sysMessage->setText("Error:记录点数超上限10000!");
            }
        }
    }
    else {return;}
}

void controlPannel::deletePos()
{
   int rowCount=ui->table1->rowCount();
   if (rowCount==0)
   {
       ui->sysMessage->setText("Error:当前示教点数量为0!");
       return;
   }
   else
   {
       int currentRow=ui->table1->currentRow();
       int index;
       if(ui->rdb_time->isChecked())
       {
           if(currentRow==-1)
           {index=rowCount-1;}
           else
           {index=currentRow;}
           posTimeList.removeAt(index);
            ui->table1->removeRow(index);
            count_time--;
            ui->teacPointNum->setText(QString::number(count_time));
            emit deletedPos(currentRow);//发送要删除的点的索引
        }
        else if(ui->rdb_distance->isChecked())
        {
           if(currentRow==-1)
           {index=rowCount-1;}
           else
           {index=currentRow;}
           posDistanceList.removeAt(index);
           ui->table1->removeRow(index);
           count_distance--;
           ui->teacPointNum->setText(QString::number(count_distance));
           emit deletedPos(currentRow);//发送要删除的点的索引
        }
        else
        {
           if(currentRow==-1)
           {index=rowCount-1;}
           else
           {index=currentRow;}
           posPointList.removeAt(index);
           ui->table1->removeRow(index);
           count_point--;
           ui->teacPointNum->setText(QString::number(count_point));
           emit deletedPos(currentRow);//发送要删除的点的索引
        }
   }

}

void controlPannel::modifyPos()
{
    int currentRow=ui->table1->currentRow();
    int count=ui->table1->columnCount();
    int index;
    if(count==0){return;}
    else
    {
        if (currentRow==-1)
        {index=count-1;}
        else
        {index=currentRow;}
    }
//    qDebug()<<"index is:"<<index;

    float angle_curr[6]={0};
    float pos_curr[6]={0};
    int result1=NR_AcsAxisTheta (open_ID, angle_curr, 1, 6, 3, 0.1f);
    int result2=NR_AcsToolTipPos(open_ID, pos_curr, 1, 6, 3, 0.1f);
    if (result1==NR_E_NORMAL&&result2==NR_E_NORMAL)
    {
        if(ui->rdb_time->isChecked())
        {
            posTimeList.replace(index,posFormat(angle_curr));
        }
        else if(ui->rdb_distance->isChecked())
        {
            posDistanceList.replace(index,posFormat(angle_curr));
         }
        else
        {
            posPointList.replace(index,posFormat(angle_curr));
        }
        updateTable(index,angle_curr);
        emit updatedPos(pos_curr,currentRow);//发送点坐标及索引
    }
    else {return;}
}

QString controlPannel::posFormat(float pos[])
{
    QString l;
    for(int i=0;i<5;i++)
    {
        l+=QString::number(pos[i],'f',2);
        l+=",";
    }
    l+=QString::number(pos[5],'f',2);
    return l;
}

void controlPannel::addToTable(int row,float angle[])
{
    ui->table1->insertRow(row);
    ui->table1->setItem(row,0,new QTableWidgetItem(QString::number(angle[0],'f',2)));
    ui->table1->setItem(row,1,new QTableWidgetItem(QString::number(angle[1],'f',2)));
    ui->table1->setItem(row,2,new QTableWidgetItem(QString::number(angle[2],'f',2)));
    ui->table1->setItem(row,3,new QTableWidgetItem(QString::number(angle[3],'f',2)));
    ui->table1->setItem(row,4,new QTableWidgetItem(QString::number(angle[4],'f',2)));
    ui->table1->setItem(row,5,new QTableWidgetItem(QString::number(angle[5],'f',2)));
}

void controlPannel::updateTable(int row,float angle[])
{
    ui->table1->setItem(row,0,new QTableWidgetItem(QString::number(angle[0],'f',2)));
    ui->table1->setItem(row,1,new QTableWidgetItem(QString::number(angle[1],'f',2)));
    ui->table1->setItem(row,2,new QTableWidgetItem(QString::number(angle[2],'f',2)));
    ui->table1->setItem(row,3,new QTableWidgetItem(QString::number(angle[3],'f',2)));
    ui->table1->setItem(row,4,new QTableWidgetItem(QString::number(angle[4],'f',2)));
    ui->table1->setItem(row,5,new QTableWidgetItem(QString::number(angle[5],'f',2)));
}

void controlPannel::savePoseData()
{

    if(ui->rdb_time->isChecked())
    {
        if(posTimeList.length()==0)
        {
            ui->sysMessage->setText("Error:pos number is 0!");
            return;
        }
        else
        {
           QString path=QFileDialog::getSaveFileName(this,"保存路径","G:\\QTCodes\\poseFile\\MZ07L-01-A.001");
           QFile file(path);
           if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
           {
               ui->sysMessage->setText("Message:文件打开失败");
           }
           QTextStream out(&file);
           QTextCodec *codec = QTextCodec::codecForName("GBK");
           for (int i=0;i<posTimeList.length();i++)
           {
               if(i==0)
               {
                   QString l="MOVEX A=4,AC=0,SM=0,M1J,P,("+posTimeList[i] +"),R=80,H=1,MS"+"\n";
                   codec->toUnicode(l.toLocal8Bit());
                   out << l;
               }
               else
               {
                   QString l="MOVEX A=4,AC=0,SM=0,M1J,L,("+posTimeList[i] +"),R=80,H=1,MS"+"\n";
                   codec->toUnicode(l.toLocal8Bit());
                   out << l;
               }

           }
           QString endLine="END";
           codec->toUnicode(endLine.toLocal8Bit());
           out << endLine;
           file.close();
           ui->sysMessage->setText("Message:文件保存成功");
        }

    }

    else if(ui->rdb_distance->isChecked())
    {
        if(posDistanceList.length()==0)
        {
            ui->sysMessage->setText("Error:pos number is 0!");
            return;
        }
        else
        {
           QString path=QFileDialog::getSaveFileName(this,"保存路径","G:\\QTCodes\\poseFile\\MZ07L-01-A.001");
           QFile file(path);
           if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
           {
               ui->sysMessage->setText("Message:文件打开失败");
           }
           QTextStream out(&file);
           QTextCodec *codec = QTextCodec::codecForName("GBK");
           for (int i=0;i<posDistanceList.length();i++)
           {
               if(i==0)
               {
                   QString l="MOVEX A=4,AC=0,SM=0,M1J,P,("+posDistanceList[i] +"),R=80,H=1,MS"+"\n";
                   codec->toUnicode(l.toLocal8Bit());
                   out << l;
               }
               else
               {
                   QString l="MOVEX A=4,AC=0,SM=0,M1J,L,("+posDistanceList[i] +"),R=80,H=1,MS"+"\n";
                   codec->toUnicode(l.toLocal8Bit());
                   out << l;
               }

           }
           QString endLine="END";
           codec->toUnicode(endLine.toLocal8Bit());
           out << endLine;
           file.close();
           ui->sysMessage->setText("Message:文件保存成功");
        }
     }
    else
    {
        if(posPointList.length()==0)
        {
            ui->sysMessage->setText("Error:pos number is 0!");
            return;
        }
        else
        {
           QString path=QFileDialog::getSaveFileName(this,"保存路径","G:\\QTCodes\\poseFile\\MZ07L-01-A.001");
           QFile file(path);
           if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
           {
               ui->sysMessage->setText("Message:文件打开失败");
           }
           QTextStream out(&file);
           QTextCodec *codec = QTextCodec::codecForName("GBK");
           for (int i=0;i<posPointList.length();i++)
           {
               if(i==0)
               {
                   QString l="MOVEX A=4,AC=0,SM=0,M1J,P,("+posPointList[i] +"),R=80,H=1,MS"+"\n";
                   codec->toUnicode(l.toLocal8Bit());
                   out << l;
               }
               else
               {
                   QString l="MOVEX A=4,AC=0,SM=0,M1J,L,("+posPointList[i] +"),R=80,H=1,MS"+"\n";
                   codec->toUnicode(l.toLocal8Bit());
                   out << l;
               }
           }
           QString endLine="END";
           codec->toUnicode(endLine.toLocal8Bit());
           out << endLine;
           file.close();
           ui->sysMessage->setText("Message:文件保存成功");
        }
    }
}

void controlPannel::sendRobotProgram()
{
    QString path=QFileDialog::getOpenFileName(this,"send file dialog", "G:\\QTCodes\\poseFile");
    if (path.isEmpty()) return;
    QString fileName="PROGRAM\\"+path.split("/",QString::SkipEmptyParts).last();
    char * localFile=path.toLatin1().data();
    char * remoteFile=fileName.toLatin1().data();
    char *ipAdress = ui->ip->text().toLatin1().data();

     int result=NR_UpLoadA(ipAdress,remoteFile,localFile);
     if (result!=NR_E_NORMAL)
     {
         ui->sysMessage->setText(QString("Error:文件传输失败,错误代码:%1").arg(result));
     }
     else
     {
         ui->sysMessage->setText("Message:文件传输成功");
     }

}

void controlPannel::scatterLink()
{
    if(swich==false)
    {
        subwin->show();
        ui->PB_link->setText("可视化关");
        swich=true;
    }
    else
    {
        subwin->hide();
        ui->PB_link->setText("可视化开");
        swich=false;
    }
}

void controlPannel::handleCamData(QString str)
{
    if(str=="noData")
    {return;}
    float data[6]={0};
    data[0]=str.split(",",QString::SkipEmptyParts).value(0).toFloat();
    data[1]=str.split(",",QString::SkipEmptyParts).value(1).toFloat();
    data[2]=str.split(",",QString::SkipEmptyParts).value(2).toFloat();
    if (dataList.length()<6)
    {
        dataList.enqueue(data[0]);
        dataList.enqueue(data[1]);
        dataList.enqueue(data[2]);
    }
    else if(dataList.length()==6)
    {
        dataList.dequeue();
        dataList.dequeue();
        dataList.dequeue();
        dataList.enqueue(data[0]);
        dataList.enqueue(data[1]);
        dataList.enqueue(data[2]);
    }
    if(dataList.length()==6)
    {

        float delta_x=dataList[3]-dataList[0];
        float delta_y=dataList[4]-dataList[1];
        float delta_z=dataList[5]-dataList[2];
        if(delta_x==0.0f&&delta_y==0.0f&&delta_z==0.0f)
        {
            return;
        }
        m_Pose = {delta_x, delta_y, delta_z, 0.0f, 0.0f, 0.0f};
        emit faceMoved();
    }

}

NR_POSE controlPannel::returnNrPose(int i,int speedLevel)
{
    NR_POSE PoseRet = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
    switch(i)
            {
            case 1:
                PoseRet.fX = -operateSpeed[speedLevel-1];
                break;
            case 2:
                PoseRet.fX = operateSpeed[speedLevel-1];
                break;
            case 3:
                PoseRet.fY = -operateSpeed[speedLevel-1];
                break;
            case 4:
                PoseRet.fY = operateSpeed[speedLevel-1];
                break;
            case 5:
                PoseRet.fZ = -operateSpeed[speedLevel-1];
                break;
            case 6:
                PoseRet.fZ = operateSpeed[speedLevel-1];
                break;
            case 7:
                PoseRet.fYaw = -operateSpeed[speedLevel-1];
                break;
            case 8:
                PoseRet.fYaw = operateSpeed[speedLevel-1];
                break;
            case 9:
                PoseRet.fPitch = -operateSpeed[speedLevel-1];
                break;
            case 10:
                PoseRet.fPitch = operateSpeed[speedLevel-1];
                break;
            case 11:
                PoseRet.fRoll = -operateSpeed[speedLevel-1];
                break;
            case 12:
                PoseRet.fRoll = operateSpeed[speedLevel-1];
                break;
            default:
                break;
            }

        return PoseRet;
}

void controlPannel::writeInit(QString key, QString value)
{
    QString path = "ip.ini";

    //创建配置文件操作对象
    QSettings *config = new QSettings(path, QSettings::IniFormat);

    //将信息写入配置文件
    config->beginGroup("config");
    config->setValue(key, value);
    config->endGroup();
    delete config;
}

QString controlPannel::readInit(QString key)
{
    QString value = QString("");
    QString path = "ip.ini";

    //创建配置文件操作对象
    QSettings *config = new QSettings(path, QSettings::IniFormat);

    //读取配置信息
    value = config->value(QString("config/") + key).toString();
    delete config;
    return value;
}




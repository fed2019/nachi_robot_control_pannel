#ifndef CONTROLPANNEL_H
#define CONTROLPANNEL_H

#include <QMainWindow>
#include <QPushButton>
#include <QRadioButton>
#include <QString>
#include <QLocale>
#include <QTimer>
#include <QDebug>
#include <QThread>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QComboBox>
#include <QFontComboBox>
#include <QLabel>
#include <QMessageBox>
#include <QFileDialog>
#include <QScreen>
#include <QFontDatabase>
#include <QFile>
#include "controlrobot.h"
#include "subwindow.h"
#include "readcamera.h"
#include <QSettings>

QT_BEGIN_NAMESPACE
namespace Ui { class controlPannel; }
QT_END_NAMESPACE


class controlPannel : public QMainWindow
{
    Q_OBJECT

private:
    Ui::controlPannel *ui;
    controlRobot *robot;
    int open_ID;//全局变量，连接ID
    float jAngle[6]={0};//全局变量，各轴角度
    int recordStatus=1;//1-记录 2-暂停 0-停止
    bool swich=false;//子窗口开启状态
    QList<QString> posTimeList;
    QList<QString> posDistanceList;
    QList<QString> posPointList;
    float operateSpeed[5]={0.5,5,50,100,250};
    QQueue<float> dataList;
    float pastPos[6]={0};
    int count_time;
    int count_distance;
    int count_point;
    const int TIME_GAP=15;
    qint64 pastTime=0;
    QTimer *timer1;//轴监视器的定时器
    QTimer *timer2; //记录模式的定时器
    QTimer *timer3; //等距记录模式定时器
    QTimer *timer4; //点记录模式定时器
    QTimer *timer5; //请求相机定时器
    QTimer *timer6; //触发运动定时器
    subwindow *subwin;
    readCamera *readCam;//本身就是一个套接字
    QThread *faceControlThread;
    NR_POSE m_Pose;

signals:
    void disconnected();
    void sendRecordMode(int,unsigned long);
    void sendCurrentPos(float *,int num);
    void deletedPos(int);
    void updatedPos(float *,int);
    void faceMoved();

protected:
    void closeEvent(QCloseEvent *);

public:
    controlPannel(QWidget *parent = nullptr);
    ~controlPannel();
    void connectOrDisConnect();
    void angleMonitor();
    void startRecord();
    void recordByTime();
    void recordByDistance();
    void recordByPoint();
    void stopRecord();
    void servoOn();
    void servoOff();
    void onXMinus();
    void onXPlus();
    void onYMinus();
    void onYPlus();
    void onZMinus();
    void onZPlus();
    void onRXMinus();
    void onRXPlus();
    void onRYMinus();
    void onRYPlus();
    void onRZMinus();
    void onRZPlus();
    void onTimerGo();
    void addPos();
    void insertPos();
    void deletePos();
    void modifyPos();
    QString posFormat(float *);
    void addToTable(int,float *);
    void updateTable(int,float *);
    void savePoseData();
    void sendRobotProgram();
    void scatterLink();
    void handleCamData(QString);
    NR_POSE returnNrPose(int,int);
    void writeInit(QString, QString);
    QString readInit(QString);
};
#endif // CONTROLPANNEL_H

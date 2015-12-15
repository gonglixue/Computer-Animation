#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QPoint>
#include <QPainter>
#include <QDebug>
#include <QPixmap>
#include <QPropertyAnimation>
#include <QLabel>
#include <QString>
#include <QDir>
#include <math.h>
#include <QMessageBox>
#include <QTimer>
#include <QRadioButton>
#include <QButtonGroup>
#include <QGroupBox>

class CSpline
{
public:
    float m[16];
    QPoint s[1000];
    int allCount;   //所有点个数
    float lengthList[800];  //弧长列表
    QPoint dividPoint[800];
    int dividCount;
public:
    void CSplineInit(int x[],int y[],int n,int grain,float tension);
    void CubicSpline(int n,QPoint* knots,int grain,float tension);
    void GetCardinalMatrix(float a1);
    float Matrix(float a,float b,float c,float d,float u);
    void clearSpline();
    float Length();
    //void animation(int duration);
    int SearchLength(float length);
    void Divid(float length);
};

class PaintedWidget:public QWidget
{
    Q_OBJECT
private:
    CSpline spline;
    QPixmap carimg;
    QLabel *car;
    int duration;

    QPoint pp[100];
    int x[100];
    int y[100];
    int count;  //鼠标输入的控制点个数
    bool animatStart;   //是否开始运动
    int animationPos;  //小车运动到第几个点
    bool uniform;       //匀速为true
public:
    PaintedWidget(QWidget *parent = 0);
    void clear();
    void sliderupdate(int){
        update();
    }
    void clickupdate(bool){
        update();
    }
    void animation();
    void animation2();
    void setDuration(int speed)
    {
        duration = -195*speed + 4000;
    }
    float Widget_Length();
    int SearchLengthPosition(float length);
    void DividSpline(float length);
    void animationControl();
    void animationStart();
    void motion_choose1();  //选择匀速
    void motion_choose0();   //选择变速

protected:
    void paintEvent(QPaintEvent *);  //重写这个函数
    void drawSpline(QPaintEvent *);

protected:
    void mousePressEvent(QMouseEvent *event);
};

class Set:public QWidget
{
    Q_OBJECT
public:
    void setTension(int tension);
    void setGrain(int grain);
    void setDurationValue(int speed);
};

class MainWindow : public QWidget
{
    Q_OBJECT
private:
    //PaintedWidget *paintArea;
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
};

#endif // MAINWINDOW_H

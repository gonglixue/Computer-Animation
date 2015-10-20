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


class CSpline
{
public:
    float m[16];
    QPoint s[800];
    int allCount;
public:
    void CSplineInit(int x[],int y[],int n,int grain,float tension);
    void CubicSpline(int n,QPoint* knots,int grain,float tension);
    void GetCardinalMatrix(float a1);
    float Matrix(float a,float b,float c,float d,float u);
    void clearSpline();
    //void animation(int duration);
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
    int count;
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
    void setDuration(int speed)
    {
        duration = -195*speed + 4000;
    }

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

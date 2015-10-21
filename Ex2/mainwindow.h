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
#include <QHBoxLayout>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include <QMessageBox>
#include <qmath.h>
#include <QtMath>
#include <math.h>

#define PI 3.141592



class Shape
{
private:
    QPoint point[30];      // 该形状图形的控制点
    int count;             // 记录该图形控制点个数
public:
    Shape();               // constructor
    int getCount();        // interface to get the number of key points
    void addPoint(QPoint in, int i);              // set the shape's key point int position i
    QPoint getPoint(int i);                       // get point[i]
    void drawShape();
    void clear();

};

class Vector
{
private:
    float r;
    float theta;
public:
    void set_v(float a_r, float a_theta)
    {
        r = a_r; theta = a_theta;
    }
    float getR(){return r;}
    float getTheta(){return theta;}
    void setR(float ar){r = ar;}
    void setTheta(float aTheta){theta = aTheta;}
};

class PaintedWidget:public QWidget
{
    Q_OBJECT
private:
    Shape shape1;          // initial graph
    Shape shape2;          // ultimate graph
    Shape inter[50];       // the intermediate graphs in the linear interpolation process
    Vector inter_v[50][30];     // 50 nbframes, and in each nbframe there are at most 29 vectors.
    int state;
    int finish;
    int frame;
protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);
public:
    PaintedWidget(QWidget *parent = 0);
    void changeState();
    void calculateInter();
    void calculateInter_v();
    void changeFinish();
    void increaseFrame();
    void clear();
};

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    //QComboBox *shape_select;
//    QPushButton *nextState;
//    PaintedWidget view;

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
};

#endif // MAINWINDOW_H

#include "mainwindow.h"
#include <QApplication>
#include <QPushButton>
#include <QSlider>
#include <QHBoxLayout>

#include <QSpinBox>

extern int grainValue;
extern float tensionValue;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    PaintedWidget view;
    //view.setMouseTracking(false);


    QPushButton clear_btn(&view);
    clear_btn.setText("Clear");
    //clear_btn.show();
    QObject::connect(&clear_btn,&QPushButton::clicked,&view,&PaintedWidget::clear);
    QObject::connect(&clear_btn,&QPushButton::clicked,&view,&PaintedWidget::clickupdate);

    QPushButton car_btn(&view);
    car_btn.setText("Start");
    QObject::connect(&car_btn,&QPushButton::clicked,&view,&PaintedWidget::animation);

    QSlider *sliderTension = new QSlider(Qt::Horizontal,&view);  // slider of tension
    sliderTension->setRange(0,10);
    sliderTension->setValue(5);
    Set o_set;
    QObject::connect(sliderTension,&QSlider::valueChanged,&o_set,&Set::setTension);
    QObject::connect(sliderTension,&QSlider::valueChanged,&view,&PaintedWidget::sliderupdate);

    QSlider *sliderGrain = new QSlider(Qt::Horizontal,&view);
    sliderGrain->setRange(1,20);
    sliderGrain->setValue(10);
    QObject::connect(sliderGrain,&QSlider::valueChanged,&o_set,&Set::setGrain);
    QObject::connect(sliderGrain,&QSlider::valueChanged,&view,&PaintedWidget::sliderupdate);

    // tension & grain
    QLabel *labelTension = new QLabel("Tension");
    QLabel *labelGrain = new QLabel("Grain");
    QSpinBox *boxTension = new QSpinBox(&view);
    QSpinBox *boxGrain = new QSpinBox(&view);
    boxTension->setRange(0,10);
    boxTension->setValue(5);
    boxGrain->setRange(1,20);
    boxGrain->setValue(10);

    // speed 和duration成反比。。。
    QLabel *labelSpeed = new QLabel("Speed");
    QSlider *sliderSpeed = new QSlider(Qt::Horizontal,&view);
    sliderSpeed->setRange(0,20);
    sliderSpeed->setValue(9);
    QObject::connect(sliderSpeed,&QSlider::valueChanged,&view,&PaintedWidget::setDuration);

    QObject::connect(sliderTension,&QSlider::valueChanged,boxTension,&QSpinBox::setValue);
    void (QSpinBox::*spinBoxSignal)(int) = &QSpinBox::valueChanged;
    QObject::connect(boxTension,spinBoxSignal,sliderTension,&QSlider::setValue);

    QObject::connect(sliderGrain,&QSlider::valueChanged,boxGrain,&QSpinBox::setValue);
    QObject::connect(boxGrain,spinBoxSignal,sliderGrain,&QSlider::setValue);
    QGridLayout* rightlayout = new QGridLayout;
    rightlayout->addWidget(labelTension,0,0);
    rightlayout->addWidget(sliderTension,0,1);
    rightlayout->addWidget(boxTension,0,2);
    rightlayout->addWidget(labelGrain,1,0);
    rightlayout->addWidget(sliderGrain,1,1);
    rightlayout->addWidget(boxGrain,1,2);
    rightlayout->addWidget(labelSpeed,2,0);
    rightlayout->addWidget(sliderSpeed,2,1);
    rightlayout->addWidget(&car_btn,3,0);
    rightlayout->addWidget(&clear_btn,3,2);
    //w.setLayout(rightlayout);

    QHBoxLayout *mainlayout = new QHBoxLayout;
    mainlayout->addWidget(&view);
    mainlayout->addLayout(rightlayout);
    mainlayout->setStretchFactor(&view,1);
    mainlayout->setStretchFactor(rightlayout,0);
    w.setLayout(mainlayout);
//    QHBoxLayout* mainlayout = new QHBoxLayout(&view);
//    mainlayout->addWidget();

   // view.show();
    w.show();
    return a.exec();
}

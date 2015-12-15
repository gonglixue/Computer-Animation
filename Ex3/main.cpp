#include "mainwindow.h"
#include <QApplication>
#include <QPushButton>
#include <QSlider>
#include <QHBoxLayout>

#include <QSpinBox>

extern int grainValue;
extern float tensionValue;
extern int durationValue;
extern QTimer *timer;

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
    //QObject::connect(&car_btn,&QPushButton::clicked,&view,&PaintedWidget::animation);
    QObject::connect(&car_btn,&QPushButton::clicked,&view,&PaintedWidget::animationStart);

    //计算总长的按钮
    QPushButton length_btn(&view);
    length_btn.setText("Length");
    QObject::connect(&length_btn,&QPushButton::clicked,&view,&PaintedWidget::Widget_Length);

    QSlider *sliderTension = new QSlider(Qt::Horizontal,&view);  // slider of tension
    sliderTension->setRange(0,10);
    sliderTension->setValue(5);
    Set o_set;
    QObject::connect(sliderTension,&QSlider::valueChanged,&o_set,&Set::setTension);
    QObject::connect(sliderTension,&QSlider::valueChanged,&view,&PaintedWidget::sliderupdate);

    QSlider *sliderGrain = new QSlider(Qt::Horizontal,&view);
    sliderGrain->setRange(1,50);
    sliderGrain->setValue(20);
    QObject::connect(sliderGrain,&QSlider::valueChanged,&o_set,&Set::setGrain);
    QObject::connect(sliderGrain,&QSlider::valueChanged,&view,&PaintedWidget::sliderupdate);


    // tension & grain
    QLabel *labelTension = new QLabel("Tension");
    QLabel *labelGrain = new QLabel("Grain");
    QSpinBox *boxTension = new QSpinBox(&view);
    QSpinBox *boxGrain = new QSpinBox(&view);
    boxTension->setRange(0,10);
    boxTension->setValue(5);
    boxGrain->setRange(1,50);
    boxGrain->setValue(20);

    // speed 和duration成反比。。。
    QLabel *labelSpeed = new QLabel("Speed");
    QSlider *sliderSpeed = new QSlider(Qt::Horizontal,&view);
    sliderSpeed->setRange(0,20);
    sliderSpeed->setValue(9);
    //QObject::connect(sliderSpeed,&QSlider::valueChanged,&view,&PaintedWidget::setDuration);
    QObject::connect(sliderSpeed,&QSlider::valueChanged,&o_set,&Set::setDurationValue);

    QObject::connect(sliderTension,&QSlider::valueChanged,boxTension,&QSpinBox::setValue);
    void (QSpinBox::*spinBoxSignal)(int) = &QSpinBox::valueChanged;
    QObject::connect(boxTension,spinBoxSignal,sliderTension,&QSlider::setValue);

    QObject::connect(sliderGrain,&QSlider::valueChanged,boxGrain,&QSpinBox::setValue);
    QObject::connect(sliderGrain,&QSlider::valueChanged,&view,&PaintedWidget::Widget_Length);
    QObject::connect(boxGrain,spinBoxSignal,sliderGrain,&QSlider::setValue);
    QObject::connect(boxGrain,spinBoxSignal,&view,&PaintedWidget::DividSpline);
    QObject::connect(boxGrain,spinBoxSignal,&view,&PaintedWidget::Widget_Length);

    //输入弧长的box
    QSpinBox *boxLength = new QSpinBox(&view);
    boxLength->setRange(10,800);
    boxLength->setValue(50);
    QObject::connect(boxLength,spinBoxSignal,&view,&PaintedWidget::DividSpline);
    QObject::connect(boxLength,spinBoxSignal,&view,&PaintedWidget::sliderupdate);
    QLabel *labelLength = new QLabel("length of each unit");

    //匀速 变速单选按钮
    QRadioButton *uniform = new QRadioButton(&view);
    uniform->setText("uniform");
    uniform->setChecked(true);
    QRadioButton *variable = new QRadioButton(&view);
    variable->setText("variable");

    //QGroupBox *buttonGroup = new QGroupBox(&view);
    //QGridLayout *radiolayout = new QGridLayout;
    //radiolayout->addWidget(uniform,0,0,1,2);
    //radiolayout->addWidget(variable,0,3,1,2);
    //buttonGroup->setLayout(radiolayout);
//    QButtonGroup* buttonGroup = new QButtonGroup(&view);
//    buttonGroup->addButton(uniform,0);
//    buttonGroup->addButton(variable,1);
    QObject::connect(uniform,&QRadioButton::toggled,&view,&PaintedWidget::motion_choose1);
    QObject::connect(variable,&QRadioButton::toggled,&view,&PaintedWidget::motion_choose0);



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
    rightlayout->addWidget(&length_btn,3,1);
    rightlayout->addWidget(&clear_btn,3,2);
    rightlayout->addWidget(labelLength,4,0,1,2);
    rightlayout->addWidget(boxLength,4,2);  //单位弧长输入
    //rightlayout->addWidget(buttonGroup,5,0,2,5);
    rightlayout->addWidget(uniform,5,0);
    rightlayout->addWidget(variable,5,1);

    //w.setLayout(rightlayout);

    QHBoxLayout *mainlayout = new QHBoxLayout;
    mainlayout->addWidget(&view);
    mainlayout->addLayout(rightlayout);
    mainlayout->setStretchFactor(&view,1);
    mainlayout->setStretchFactor(rightlayout,0);
    w.setLayout(mainlayout);
//    QHBoxLayout* mainlayout = new QHBoxLayout(&view);
//    mainlayout->addWidget();

    //test

    timer = new QTimer(&view);
    QObject::connect(timer,&QTimer::timeout,&view,&PaintedWidget::animationControl);
    timer->start(durationValue);



   // view.show();
    w.show();
    return a.exec();
}

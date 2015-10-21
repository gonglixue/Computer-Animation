#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //MainWindow w;
    //MainWindow main;
    QWidget main;
    main.resize(800,600);
    main.setWindowTitle("interpolitation demo");

    PaintedWidget view;

    QLabel *labelState = new QLabel("next state");
    QPushButton next_state(&view);
    next_state.setText("next state");
    QObject::connect(&next_state,&QPushButton::clicked,&view,&PaintedWidget::changeState);

    QLabel *labelFinish = new QLabel("finish");
    QPushButton finish(&view);
    finish.setText("finish");
    QObject::connect(&finish,&QPushButton::clicked,&view,&PaintedWidget::changeFinish);

    QLabel *labelClear = new QLabel("clear");
    QPushButton clear(&view);
    clear.setText("clear");
    QObject::connect(&clear,&QPushButton::clicked,&view,&PaintedWidget::clear);

    QGridLayout *rightlayout = new QGridLayout;
    rightlayout->addWidget(&next_state,0,1);
    rightlayout->addWidget(labelState,0,0);
    rightlayout->addWidget(labelFinish,1,0);
    rightlayout->addWidget(&finish,1,1);
    rightlayout->addWidget(labelClear,2,0);
    rightlayout->addWidget(&clear,2,1);

    QHBoxLayout *mainlayout = new QHBoxLayout;
    mainlayout->addWidget(&view);
    mainlayout->addLayout(rightlayout);
    mainlayout->setStretchFactor(&view,1);
    mainlayout->setStretchFactor(rightlayout,0);

    main.setLayout(mainlayout);
    //view.setLayout(rightlayout);

    QTimer *timer = new QTimer(&view);
    QObject::connect(timer,&QTimer::timeout,&view,&PaintedWidget::increaseFrame);
    timer->start(30);


    main.show();
    //view.show();

    return a.exec();
}

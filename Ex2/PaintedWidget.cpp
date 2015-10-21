#include "mainwindow.h"
PaintedWidget::PaintedWidget(QWidget *parent):QWidget(parent),shape1(),shape2()
{
    resize(800,600);
    setWindowTitle(tr("interpolation"));

    this->setMouseTracking(true);
    state = 0;
    finish = 0;
    frame = 0;
}

void PaintedWidget::mousePressEvent(QMouseEvent *e)
{
    setCursor(Qt::PointingHandCursor);
    QPoint current = e->pos();
    int i = shape1.getCount();
    int j = shape2.getCount();
    if(state == 0)
    {
        shape1.addPoint(current,i);
        i++;
        qDebug() << "click: shape 1 add point";
    }
    else
    {
        shape2.addPoint(current,j);
        j++;
        qDebug() << "click: shape 2 add point";
    }

    //qDebug()<<current <<" click";

    update();
}

void PaintedWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    // draw shape1
    //qDebug()<<"PaintedWidget::painEvent";

    int numShape1 = shape1.getCount();
    for(int i=0;i<numShape1-1;i++)
    {
        if(numShape1>1)
            painter.drawLine(shape1.getPoint(i),shape1.getPoint(i+1));
    }
    painter.setBrush(Qt::blue);
    for(int i=0;i<numShape1;i++)
        painter.drawEllipse(shape1.getPoint(i),3,3);

    // draw shape2
    painter.setBrush(Qt::black);
    int numShape2 = shape2.getCount();
    for(int i=0;i<numShape2-1;i++)
    {
        if(numShape2>1)
            painter.drawLine(shape2.getPoint(i),shape2.getPoint(i+1));
    }
    painter.setBrush(Qt::blue);
    for(int i=0;i<numShape1;i++)
        painter.drawEllipse(shape2.getPoint(i),3,3);

    // draw intermediate image
    painter.setBrush(Qt::blue);
    if(finish == 1)
    {
        calculateInter_v();
        int numShape = shape1.getCount();
        //for(int i=0;i<5;i++)  // 50 nbframes
        //{
            //qDebug()<<"draw inter "<<frame<<":";
            for(int j=0;j<numShape-1;j++)
            {
                if(numShape>1)
                    painter.drawLine(inter[frame].getPoint(j),inter[frame].getPoint(j+1));
                    //qDebug()<<"draw inter line "<<inter[frame].getPoint(j)<<"to"<<inter[frame].getPoint(j+1);
            }
        //}


    }



}

void PaintedWidget::changeState()
{
    state = 1;
    update();
}

void PaintedWidget::calculateInter()
{
    int count = shape1.getCount();  // the number of key points in the graph
    //假设生成50帧中间画面
    for(int t=0;t<50;t++)
    {
        for(int i=0;i<count;i++)
        {
            float u = float(t)/50.0;
            QPoint in_i;    // the i_th point in the intermediate image
            QPoint p0_i = shape1.getPoint(i);   // the i_th point in the initial graph
            QPoint p1_i = shape2.getPoint(i);   // the i_th point in the ultimate graph
            int x_i = (1-u)*(p0_i.x()) + u*(p1_i.x());
            int y_i = (1-u)*(p0_i.y()) + u*(p1_i.y());
            in_i.setX(x_i);
            in_i.setY(y_i);
            inter[t].addPoint(in_i,i);
            //qDebug() << "interShape add point" << count << ":" << inter[t];
            //qDebug()<<"interShape add point "<<count;
        }
    }
    qDebug()<<"finish calculation-----------------";
}

void PaintedWidget::calculateInter_v()
{
    Vector shape1_v[30],shape2_v[30];
    int count = shape1.getCount();
    float x0_first = shape1.getPoint(0).x();
    float y0_first = shape1.getPoint(0).y();
    float r0_first = sqrt(x0_first*x0_first + y0_first*y0_first);
    float theta0_first = (x0_first-0)/r0_first;
    //qDebug()<<"initial vector cos:"<<theta0_first;
    theta0_first = acos(theta0_first);
    if(y0_first-0<0)
        theta0_first = 2*PI - theta0_first;
    shape1_v[0].set_v(r0_first,theta0_first);

    float x1_first = shape2.getPoint(0).x();
    float y1_first = shape2.getPoint(0).y();
    float r1_first = sqrt(x1_first*x1_first + y1_first*y1_first);
    float theta1_first = (x1_first-0)/r1_first;
    //qDebug()<<"ultimate vector cos:"<<theta1_first;
    theta1_first = acos(theta1_first);
    if(y1_first-0<0)
        theta1_first = 2*PI - theta1_first;
    shape2_v[0].set_v(r1_first,theta1_first);


    for(int i=0;i<count-1;i++)
    {
        float x01 = shape1.getPoint(i).x();
        float y01 = shape1.getPoint(i).y();
        float x02 = shape1.getPoint(i+1).x();
        float y02 = shape1.getPoint(i+1).y();

        float r0 = sqrt( (x02-x01)*(x02-x01)+(y02-y01)*(y02-y01) );
        float theta0 = (x02-x01)/r0;
//        if(i==0)
//        {
//            qDebug()<<"r"<<r0;
//            qDebug()<<"x02-x01:"<<(x02-x01);
//            qDebug()<<"cos:"<<theta0;
//            qDebug()<<"arccos:"<<acos(theta0);
//            qDebug()<<"y02-y01"<<y02-y01;
//        }
        theta0 = acos(theta0);
        if((y02-y01)<0)
            theta0 = 2*PI - theta0;
        shape1_v[i+1].set_v(r0,theta0);

        float x11 = shape2.getPoint(i).x();
        float y11 = shape2.getPoint(i).y();
        float x12 = shape2.getPoint(i+1).x();
        float y12 = shape2.getPoint(i+1).y();

        float r1 = sqrt( (x12-x11)*(x12-x11)+(y12-y11)*(y12-y11) );
        float theta1 = (x12-x11)/r1;
        theta1 = acos(theta1);
        if((y12-y11)<0)
            theta1 = 2*PI - theta1;
        shape2_v[i+1].set_v(r1,theta1);
    }
    //qDebug()<<"initial vector in 0"<<shape1_v[1].getR()<<","<<shape1_v[1].getTheta();
    //qDebug()<<"ultimate vector in 0"<<shape2_v[1].getR()<<","<<shape2_v[1].getTheta();
    //假设生成50帧中间画面
    for(int t=0;t<50;t++)
    {
        float u = float(t)/50.0;
        for(int i=0;i<count;i++)
        {           
            QPoint in_i;
            float tempR,tempTheta;
            tempR = (1-u)*shape1_v[i].getR() + u*shape2_v[i].getR();

            if(i>0 && (shape1_v[i].getTheta()-shape2_v[i].getTheta())>0)
            {
                tempTheta = (1-u)*shape1_v[i].getTheta() + u*(shape2_v[i].getTheta()+2*PI);
                if(i>0)
                qDebug()<<"big to small";
            }
            else
            {
                tempTheta = (1-u)*shape1_v[i].getTheta() + u*shape2_v[i].getTheta();
                if(i>0)
                qDebug()<<"small to big";
            }

            if(i==0)
            {
                in_i.setX(tempR*cos(tempTheta));
                in_i.setY(tempR*sin(tempTheta));
            }
            else
            {
                in_i.setX(tempR*cos(tempTheta)+inter[t].getPoint(i-1).x());
                in_i.setY(tempR*sin(tempTheta)+inter[t].getPoint(i-1).y());
            }
            inter[t].addPoint(in_i,i);

        }
    }
}

void PaintedWidget::changeFinish()
{
    if(shape1.getCount() != shape2.getCount())
    {
//        QMessageBox::about(this,tr("Error!"),
//                           tr("Sorry! The number of key points in initial state and ultimate state must be equal! You can click 'clear' to redraw."),

//                           );
        if(QMessageBox::warning(this,
                             tr("Error"),
                             tr("Sorry! The number of key points in initial state and ultimate state must be equal! Plase redraw."),
                             QMessageBox::Ok) == QMessageBox::Ok)
        {
            this->clear();
        }

    }
    else if(shape1.getCount() == 0)
        finish = 0;
    else
        finish = 1;
    update();
    qDebug()<<"finish = "<<finish<<"--------------";
}

void PaintedWidget::increaseFrame()
{
    if(finish==1)
    {
        if(frame<50)
            frame++;
        else
            frame = 0;
    }
    update();
}

void PaintedWidget::clear()
{
    shape1.clear();
    shape2.clear();
    for(int i=0;i<50;i++)
    {
        inter[i].clear();
    }
    state = 0;
    finish = 0;
    frame = 0;
}

#include "mainwindow.h"

Shape::Shape()
{
    for(int i=0;i<20;i++)
    {
        point[i].setX(0);
        point[i].setY(0);
    }
    count = 0;
}

int Shape::getCount()
{
    return count;
}

void Shape::addPoint(QPoint in, int i)
{
    point[i].setX(in.x());
    point[i].setY(in.y());

    count++;
    //qDebug()<<"add point "<<in<<". count="<<count;

}

QPoint Shape::getPoint(int i)
{
    return point[i];
}

void Shape::drawShape()
{
    QPainter painter;
    if(count>1)
    {
        for(int i=0;i<count-1;i++)
        {
            painter.drawLine(point[i],point[i+1]);
            qDebug()<<"draw line";
        }
    }
    painter.setBrush(Qt::blue);
    for(int i=0;i<count;i++)
        painter.drawEllipse(point[i],3,3);
}

void Shape::clear()
{
    for(int i=0;i<30;i++)
    {
        point[i].setX(0);
        point[i].setY(0);
    }
    count = 0;
}


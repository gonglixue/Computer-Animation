#include "mainwindow.h"


 int grainValue = 10;
 float tensionValue = 0.5;

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    //paintArea = new PaintedWidget(parent);
    resize(800,600);
}

MainWindow::~MainWindow()
{

}

PaintedWidget::PaintedWidget(QWidget *parent):QWidget(parent)
{
    resize(800,600);
    setWindowTitle(tr("Paint Demo"));
    this->count = 0;
    this->setMouseTracking(true);
    for(int j=0;j<100;j++)
    {
        x[j] = y[j] = 0;
    }

    //carimg = QPixmap("F:/qtLearning/PaintSystem/car2.PNG").scaled(40,40);
    QString path=QDir::currentPath(); //获取程序当前目录
    //path.replace("/","\\"); //将地址中的"/"替换为"\"，因为在Windows下使用的是"\"。
    qDebug()<<path+"/car2.PNG";
    QPixmap pixmap(path+"/car2.PNG");
    carimg = pixmap.scaled(40,40);
    car = new QLabel(this);
    duration = 2000;



}

void PaintedWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);    
    // join these points to draw lines.
    for(int j=0;j<count-1;j++)
    {
        if(count>1)
        {
            painter.drawLine(x[j+1],y[j+1],x[j],y[j]);
            //qDebug()<<pp[j+1]<<"in paint event:"<<pp[j];
        }   
    }
    // join the points in spline.s[] to draw many small lines to represent the curve
    if(count > 1){
        spline.CSplineInit(x,y,count,grainValue,tensionValue);
        //qDebug()<<tensionValue <<"tensiongValue!!";
        for(int j=0;j<spline.allCount-1;j++)
        {
            QPen redpen;
            redpen.setColor(Qt::red);
            painter.setPen(redpen);
            painter.drawLine(spline.s[j],spline.s[j+1]);
            //qDebug()<< spline.s[j+1]<<"insert"<<spline.s[j];
        }
    }
    painter.setBrush(Qt::blue);
    for(int j=0;j<count;j++)
        painter.drawEllipse(pp[j],3,3);
}

/*
void PaintedWidget::drawSpline(QPaintEvent *)
{
    QPainter painter;
    if(count > 1){
        spline.CSplineInit(x,y,count,grainValue,tensionValue);
       // qDebug()<<tensionValue <<"tensiongValue!!";
        for(int j=0;j<spline.allCount-1;j++)
        {
            QPen redpen;
            redpen.setColor(Qt::red);
            painter.setPen(redpen);
            painter.drawLine(spline.s[j],spline.s[j+1]);
            //qDebug()<< spline.s[j+1]<<"insert"<<spline.s[j];
        }
    }
}

*/

void PaintedWidget::mousePressEvent(QMouseEvent *event)
{
    setCursor(Qt::PointingHandCursor);
   // qDebug()<<event->pos()<<"mouse event";
    QPoint current = event->pos();
    // pick up the poings clicked and record them in the array
    pp[count] = event->pos();
    x[count] = pp[count].x();
    y[count] = pp[count].y();

    count++;
    update();
}

void PaintedWidget::clear()
{
    count = 0;
    for(int i=0;i<100;i++)
    {
        x[i] = y[i] = 0;
        pp[i].setX(0);
        pp[i].setY(0);
    }
    spline.clearSpline();
    qDebug()<<pp[20]<<"arbitrary is zero"<<pp[21];
    car->hide();
    //update();
}

void PaintedWidget::animation()
{

    car->setPixmap(carimg);
    car->show();
    QPropertyAnimation *anim = new QPropertyAnimation(car,"pos");
    anim->setDuration(duration);

    QPoint temp=spline.s[0];    // temp是把曲线点移动（-40,-40）的点，原先是小车左上角贴着曲线运动，这样使得右下角贴着曲线运动
    temp.setX(spline.s[0].x()-40);
    temp.setY(spline.s[0].y()-40);

    anim->setStartValue(temp);
    for(int i=1;i<spline.allCount-1;i++)
    {

        temp.setX(spline.s[i].x()-40);
        temp.setY(spline.s[i].y()-40);
        anim->setKeyValueAt(((float)i)/(spline.allCount-1),temp);
    }

    temp.setX(spline.s[spline.allCount-1].x()-40);
    temp.setY(spline.s[spline.allCount-1].y()-40);
    anim->setEndValue(temp);
    //car->move();
    anim->start();

    qDebug()<<spline.s[0]<<" to " << spline.s[spline.allCount-1];
    qDebug()<<"animation exe";

}

void CSpline::CSplineInit(int x[], int y[], int n, int grain, float tension)
{
    int i,np;
    np = n;
    QPoint jd[100];
    QPoint *knots;

    allCount = 0;

    for(i=1;i<=np;i++)
    {
        jd[i].setX(x[i-1]);
        jd[i].setY(y[i-1]);
    }
    jd[0].setX(x[0]);
    jd[0].setY(y[0]);
    jd[np+1].setX(x[np-1]);
    jd[np+1].setY(y[np-1]);
    np=np+2;
//    for(i=0;i<=np;i++)
//    {
//        jd[i].setX(x[i-1]);
//        jd[i].setY(y[i-1]);
//    }

    knots = jd;
    CubicSpline(np,knots,grain,tension);
}

void CSpline::CubicSpline(int n, QPoint* knots, int grain, float tension)
{
    QPoint  *k0, *kml, *k1, *k2;
    int i,j;
    float u[30];
    GetCardinalMatrix(tension);
    for(i=0; i<=grain; i++)
        u[i]=((float)i)/grain;
    kml = knots;
    k0 = kml+1;
    k1 = k0+1;
    k2 = k1+1;
    for(i=1;i<n-2;i++)
    {
        for(j=0;j<=grain;j++)
        {
            s[allCount].setX(Matrix(kml->x(),k0->x(),k1->x(),k2->x(),u[j]));
            s[allCount].setY(Matrix(kml->y(),k0->y(),k1->y(),k2->y(),u[j]));
            //qDebug()<<s[allCount]<<" Cubic inserting";
            allCount++;
        }
        k0++;kml++;k1++;k2++;
    }
}

void CSpline::GetCardinalMatrix(float a1)
{
    m[0]=-a1;  m[1]=2-a1; m[2]=a1-2;   m[3]=a1;
    m[4]=2*a1; m[5]=a1-3; m[6]=3-2*a1; m[7]=-a1;
    m[8]=-a1;  m[9]=0;    m[10]=a1;    m[11]=0;
    m[12]=0;   m[13]=1;   m[14]=0;     m[15]=0;
}

float CSpline::Matrix(float a, float b, float c, float d, float u)
{
    float p0,p1,p2,p3;
    p0=m[0]*a + m[1]*b + m[2]*c + m[3]*d;
    p1=m[4]*a + m[5]*b + m[6]*c + m[7]*d;
    p2=m[8]*a + m[9]*b + m[10]*c + m[11]*d;
    p3=m[12]*a + m[13]*b + m[14]*c + m[15]*d;
    return(p3+u*(p2+u*(p1+u*p0)));
}

void CSpline::clearSpline()
{
    allCount = 0;
    for(int i=0;i<300;i++)
    {
        s[i].setX(0);
        s[i].setY(0);
    }
}


void Set::setTension(int tension)
{
    tensionValue = tension/10.0;
    update();
}

void Set::setGrain(int grain)
{
    grainValue = grain;
}

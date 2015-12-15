#include "mainwindow.h"


 int grainValue = 20;
 float tensionValue = 0.5;
 int durationValue = 500;
 QTimer *timer;

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

    spline.dividCount=0;
    animationPos = 0;
    animatStart = false;
    uniform = true;

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

    if(spline.dividCount>2){
        for(int j=0;j<spline.dividCount;j++)
        {
            painter.setBrush(Qt::red);
            painter.drawEllipse(spline.dividPoint[j],3,3);
            //qDebug()<<"divid point painting"<<j<<spline.dividPoint[j];
        }
    }
    //qDebug()<<"allcount:"<<spline.allCount;
}

void PaintedWidget::mousePressEvent(QMouseEvent *event)
{
    setCursor(Qt::PointingHandCursor);
    //qDebug()<<event->pos()<<"mouse event";
    QPoint current = event->pos();
    // pick up the poings clicked and record them in the array
    pp[count] = event->pos();
    x[count] = pp[count].x();
    y[count] = pp[count].y();

    count++;


    //每次添加点后计算length,更新lengthList
    Widget_Length();
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
    animationPos = 0;
    animatStart = false;
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
    /*
    for(int i=1;i<spline.allCount-1;i++)
    {

        temp.setX(spline.s[i].x()-40);
        temp.setY(spline.s[i].y()-40);
        anim->setKeyValueAt(((float)i)/(spline.allCount-1),temp);
    }
    */

    for(int i=0;i<spline.dividCount-1;i++){
        temp.setX(spline.dividPoint[i].x()-40);
        temp.setY(spline.dividPoint[i].y()-40);
        anim->setKeyValueAt(((float)i)/(spline.dividCount-1),temp);
    }

    /*
    temp.setX(spline.s[spline.allCount-1].x()-40);
    temp.setY(spline.s[spline.allCount-1].y()-40);
    anim->setEndValue(temp);
    //car->move();
    */
    temp.setX(spline.dividPoint[spline.dividCount-1].x()-40);
    temp.setY(spline.dividPoint[spline.dividCount-1].y()-40);
    anim->setEndValue(temp);

    anim->start();


    //qDebug()<<spline.s[0]<<" to " << spline.s[spline.allCount-1];
    qDebug()<<"animation exe";

}

void PaintedWidget::animation2()
{
    car->setPixmap(carimg);
    car->show();
    QPropertyAnimation *anim = new QPropertyAnimation(car,"pos");


    anim->setDuration(durationValue);

    if(!uniform){  //非匀速
        if(animationPos <= spline.dividCount/2){
            durationValue -= 30;
            if (durationValue<30)
                durationValue = 30;
        }
        else
            durationValue += 30;
        timer->start(durationValue);
    }

    QPoint temp1 = spline.dividPoint[animationPos];
    QPoint temp2 = spline.dividPoint[animationPos+1];
    anim->setStartValue(temp1);
    anim->setEndValue(temp2);


    qDebug()<<"start from:"<<temp1<<"to"<<temp2;

    anim->start();
}

void PaintedWidget::animationControl()
{
    if(animatStart){
        animation2();
        animationPos++;
        if(animationPos>spline.dividCount-2){
            animationPos = 0;
            animatStart = false;
        }
        qDebug()<<animationPos;
        qDebug()<<durationValue;
    }
}

float PaintedWidget::Widget_Length()
{
    update();
    return spline.Length();
}

int PaintedWidget::SearchLengthPosition(float length)
{
    int index = spline.SearchLength(length);
    qDebug()<<"position in lengthList:"<<index;
    return index;
}

void PaintedWidget::DividSpline(float length)
{
    if(length<spline.lengthList[0]){
        //qDebug()<<"???????";
        if(QMessageBox::warning(this,
                                tr("Error"),
                                tr("Sorry!the length you input is smaller than the smallest unit."),
                                QMessageBox::Ok)==QMessageBox::Ok)
            length = spline.lengthList[0];
    }
    spline.Divid(length);
}

void PaintedWidget::animationStart(){
    animatStart = true;
}

void PaintedWidget::motion_choose1()
{
    uniform = true;
}

void PaintedWidget::motion_choose0()
{
    uniform = false;
}

void CSpline::CSplineInit(int x[], int y[], int n, int grain, float tension)
{
    int i,np;
    np = n;
    QPoint jd[100];
    QPoint *knots;

    allCount = 0;
    lengthList[0] = 0;

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
    float u[80];  //u的数组长度要和grain的范围对应
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
    for(int i=0;i<dividCount;i++)
    {
        dividPoint[i].setX(0);
        dividPoint[i].setY(0);
    }
    dividCount = 0;
}

float CSpline::Length()
{
    float length=0;
    for(int i=0;i<allCount-1;i++)
    {
        //qDebug()<<i<<":"<<s[i]<<" to "<<i+1<<s[i+1];
        float segment = 0;
        segment = pow(s[i+1].x()-s[i].x(),2)+pow(s[i+1].y()-s[i].y(),2);
        segment = sqrt(segment);  // 两个grain之间的线段长度
        length += segment;
        lengthList[i] = length;

    }
    qDebug()<<"total length:"<<length;
    if( allCount > 2 ){
        qDebug()<<"lengthList[allcount-2]:"<<lengthList[allCount-2];
        for(int i=0;i<allCount-1;i++)
            qDebug()<<"liengthlist["<<i<<"]:"<<lengthList[i];
    }
    return length;
}

int CSpline::SearchLength(float length)
{
    int a = 0;
    int b = (allCount-2);
    int mid = (a+b)/2;
    //qDebug()<<"allCount:"<<allCount;
    qDebug()<<"want to search:"<<length;
    if(allCount<=2){
        //qDebug()<<"allCount<=2";
        return -1;
    }
    if(length<lengthList[0] || length>lengthList[allCount-2]){
        //qDebug()<<"not in 0-allcount-2";
        return -1;
    }

    // binary search
    /*
    while(!(length>=lengthList[mid]&&length<lengthList[mid+1]))
    {
        if(length>=lengthList[mid+1])
        {
            a = mid+1;
            mid = (a+b)/2;
        }
        else if(length<lengthList[mid]){
            b = mid;
            mid = (a+b)/2;
        }
        else{
            break;
        }
        qDebug()<<"a:"<<a<<" b:"<<b<<" mid:"<<mid;

    }
    */


    // linear search

    int mid2 = 0;
    while(!(length>=lengthList[mid2]&&length<lengthList[mid2+1]))
    {
        mid2++;
    }
//    if(mid==mid2)
//        qDebug()<<"binary success !!!!!!!!!!!!!!!!!!!";
//    else
//        qDebug()<<"binary fail !!!!!!!!!!!!!!!"<<mid2;


    return mid2;
}

void CSpline::Divid(float length)
{
    int mid = 0;
    int i=0;
    while(mid!=-1)
    {
        dividPoint[i]=s[mid];
        i++;
        mid = SearchLength(length*i);
        //qDebug()<<"find "<<length*i<<" in "<<mid;
    }
    dividPoint[i] = s[allCount-1];
    i++;
    dividCount = i;
    //test print
//    for(int j=0;j<dividCount;j++)
//    {
//        qDebug()<<"dividPoint["<<j<<"]:"<<dividPoint[j];
//    }
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

void Set::setDurationValue(int speed)
{
    durationValue = -50*speed + 1000;
    //durationValue = 1075;
    timer->start(durationValue);
}



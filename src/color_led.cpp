#include <QPainter>
#include <qmath.h>
#include <QRadialGradient>
#include <QGraphicsDropShadowEffect>
#include "color_led.h"

ColorLED::ColorLED(QWidget *parent) :
    QWidget(parent),available(false),value(LED_CLOSE)
{
    //setup attributes
    attr.insert("OpenColor",QColor::fromHsv(40,255,255));//colors
    attr.insert("OKColor",QColor(Qt::green));
    attr.insert("FaultColor",QColor(Qt::red));
    attr.insert("TextColor",QColor(Qt::white));
    attr.insert("Text",QString(""));//text
    //setup effect
    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect;
    effect->setOffset(1,1); effect->setBlurRadius(5);
    setGraphicsEffect(effect);
}

void ColorLED::build(QVariantHash &a)/**< build UI with attributes */
{
    bool useful = false;
    for(QVariantHash::Iterator i=a.begin();i!=a.end();i++) {
        if(attr.contains(i.key())) {//check key
            if(i->type()==attr[i.key()].type()) {//check type
                attr.insert(i.key(),i.value());//insert attribute
                useful = true;
            }
        }
    }
    if(useful) {
        update();//update widget
    }
}

void ColorLED::disable()/**< set UI unavailable */
{
    if(available) {
        available = false;//set unavailable
        update();
    }
}

void ColorLED::setValue(int v)
{
    if((v>LED_CLOSE) && (v<LED_STATE_NUM)) {
        value = v;
    }
    else {
        value = LED_CLOSE;//close LED if value is illegal
    }
    available = true;
    update();
}

void ColorLED::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    //save initial state
    QPainter painter(this);
    painter.save();

    //set anti-aliasing
    painter.setRenderHint(QPainter::Antialiasing);

    //calculate rect
    QRect ledRect = rect();//get widget rect
    int diff = qAbs(ledRect.width()-ledRect.height());//get diff between width and height
    if(diff) {
        if(ledRect.width()>ledRect.height()) {
            ledRect.setLeft(ledRect.left()+diff/2);//reduce width
            ledRect.setWidth(ledRect.height());
        }
        else {
            ledRect.setTop(ledRect.top()+diff/2);//reduce height
            ledRect.setHeight(ledRect.width());
        }
    }

    //reduce 10%
    int width = ledRect.width(); int offset = width/20;
    ledRect.setLeft(ledRect.left()+offset); ledRect.setTop(ledRect.top()+offset);
    ledRect.setWidth(width*9/10); ledRect.setHeight(width*9/10);

    //get color
    QColor col;
    if((!available) || (value==LED_CLOSE) || (value>=LED_STATE_NUM)) {//unavailable
        col = QColor(Qt::gray);
    }
    else if(value==LED_OPEN) {
        col = attr.value("OpenColor",QColor::fromHsv(40,255,255)).value<QColor>();
    }
    else if(value==LED_OK) {
        col = attr.value("OKColor",QColor(Qt::green)).value<QColor>();
    }
    else {
        col = attr.value("FaultColor",QColor(Qt::red)).value<QColor>();
    }
    QColor colF = QColor::fromHsv(col.hue(),col.saturation(),col.value()*0.75);

    //draw led
    qreal cx = ledRect.x(), cy = ledRect.y(), radius = ledRect.width();
    QRadialGradient g(cx,cy,radius);
    g.setColorAt(0,col); g.setColorAt(0.5,col); g.setColorAt(1,colF);//setup gradiant
    painter.setBrush(QBrush(g)); painter.setPen(Qt::NoPen);
    painter.drawEllipse(ledRect);//draw

    //draw label
    QString text = attr.value("Text",QString("")).toString();//get text
    col = attr.value("TextColor",QColor(Qt::white)).value<QColor>();//get color
    painter.setBrush(Qt::NoBrush); painter.setPen(QPen(col));
    painter.drawText(ledRect,Qt::AlignCenter,text);

    //restore painter
    painter.restore();
}

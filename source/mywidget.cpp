#include "mywidget.h"
#include <QPen>
#include <QPainter>
#include <QPixmap>
#include <QDebug>

MyWidget::MyWidget(QWidget *parent)
    : QWidget{parent}
{

}

//绘图
void MyWidget::paintEvent(QPaintEvent *event)
{
    QPainter *painter = new QPainter(this);
    painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    painter->drawPixmap(rect(), QPixmap("../images/bg.jpg"));
    delete painter;
}

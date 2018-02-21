#include "./include/playbackline.h"

#include <QFrame>
#include <QWidget>
#include <QDialog>
#include <QtCore>
#include <QtGui>
#include <QtWidgets>

PlaybackLine::PlaybackLine(QWidget *parent) : QFrame(parent)
{
}

PlaybackLine::~PlaybackLine()
{

}


void PlaybackLine::move(bool mustMove)
{
    qDebug() << "supposed to move" << "no die";

/*
    QPropertyAnimation *a = new QPropertyAnimation(eff,"opacity");
    a->setDuration(350);
    a->setStartValue(0);
    a->setEndValue(1);
    a->setEasingCurve(QEasingCurve::InBack);
    a->start(QPropertyAnimation::DeleteWhenStopped);
*/
/*
    QPushButton *button = new QPushButton(this);
    button->setGeometry(0, 0, 100, 30);
    button->show();

    QPropertyAnimation *animation = new QPropertyAnimation(button, "geometry");
    animation->setDuration(10000);
    animation->setStartValue(QRect(0, 0, 100, 30));
    animation->setEndValue(QRect(250, 250, 100, 30));

    connect(animation, SIGNAL(finished()), animation, SLOT(deleteLater()));
    animation->start();
*/
/*
    QPropertyAnimation animation(this, "geometry");
    animation.setDuration(10000);
    animation.setStartValue(QRect(this->x(), this->y(), this->width(), this->height()));
    qDebug() << "got past start value";
    animation.setEndValue(QRect(this->x() + 500,this->y(), this->width(), this->height()));

    qDebug() << "got past end value";
    animation.start();
    qDebug() << "got past animation start";
*/
    //while(this->x() < 1000) { // need to delay this somehow because it just insta jumps
        // TODO: this moves the line up and off the grid - fix this
        // use QAnimation(?) or something like that.  seems like shoud work
        //this->QWidget::move(this->x() + 1, this->y());
    //}
    /*
    QPropertyAnimation *mAnimation = new QPropertyAnimation(this, "geometry");
    mAnimation->setDuration(10000);
    mAnimation->setStartValue(QRect(this->x(),this->y(),this->width(),this->height()));
    mAnimation->setEndValue(QRect(this->x(),this->y(),this->width(),this->height()));

    mAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    */
    qDebug() << "got to end";
}

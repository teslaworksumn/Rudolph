#include "./include/playbackline.h"

#include <QFrame>
#include <QWidget>

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
    QPropertyAnimation animation(this, "geometry");
    animation.setDuration(10000);
    animation.setStartValue(QRect(this->x(),this->y(),this->width(),this->height()));
    qDebug() << "got past start value";
    animation.setEndValue(QRect(this->x() + 500,this->y(),this->width(),this->height()));

    qDebug() << "got past end value";
    animation.start();
    qDebug() << "got past animation start";
    */
    //while(this->x() < 1000) { // need to delay this somehow because it just insta jumps
        // TODO: this moves the line up and off the grid - fix this
        // use QAnimation(?) or something like that.  seems like shoud work
        //this->QWidget::move(this->x() + 1, this->y());
    //}
    QPropertyAnimation *mAnimation = new QPropertyAnimation(this, "geometry");
    mAnimation->setDuration(10000);
    mAnimation->setStartValue(QRect(this->x(),this->y(),this->width(),this->height()));
    mAnimation->setEndValue(QRect(this->x(),this->y(),this->width(),this->height()));

    mAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    qDebug() << "got to end";
}

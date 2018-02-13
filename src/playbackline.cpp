#include "./include/playbackline.h"

#include <QFrame>
#include <QWidget>
#include <windows.h>

PlaybackLine::PlaybackLine(QWidget *parent) : QFrame(parent)
{
}

PlaybackLine::~PlaybackLine()
{

}


void PlaybackLine::move(bool mustMove)
{
    qDebug() << "supposed to move";
    while(this->x() < 1000) {
        // need to delay this somehow because it just insta jumps
        // TODO: this moves the line up and off the grid - fix this
        this->QWidget::move(this->x() + 1, 0);
    }
}

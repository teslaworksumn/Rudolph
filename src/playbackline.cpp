#include "./include/playbackline.h"

#include <QFrame>
#include <QWidget>

PlaybackLine::PlaybackLine(QWidget *parent) : QFrame(parent)
{
    m_parent = parent;
}

PlaybackLine::~PlaybackLine()
{

}

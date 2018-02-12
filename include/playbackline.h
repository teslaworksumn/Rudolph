#ifndef PLAYBACKLINE_H
#define PLAYBACKLINE_H

#include <QObject>
#include <QFrame>
#include <QWidget>
#include <QtGui>

class PlaybackLine : public QFrame
{
    Q_OBJECT
public:
    PlaybackLine(QWidget *parent = 0);
    ~PlaybackLine();

};
#endif // PLAYBACKLINE_H


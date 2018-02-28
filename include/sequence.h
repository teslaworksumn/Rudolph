#ifndef SEQUENCE_H
#define SEQUENCE_H

#include <QObject>
#include <string>
#include <vector>
#include "./include/cellrenderer.h"

#include <iostream>
#include <fstream>



class Sequence : public QObject
{
    Q_OBJECT
private:
    QList< QList< uint8_t > > grid;
    QString sequenceName;
    int frame_dur;
    int ROWS;
    int COLS;

public:
    Sequence() ;
    Sequence(int rows, int cols, int dur, QString name);
    int getRows();
    int getCols();
    int getFrameDur();
    QString openFile();
    QString getName();
    QList< QList< uint8_t > > getGrid();

public slots:
    void save();
    void load();
};


#endif // SEQUENCE_H

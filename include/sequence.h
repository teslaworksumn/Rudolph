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
    QVector< QVector< int > > grid;
    int ROWS;
    int COLS;
    std::string sequenceName;
    int frame_dur;

public:
    Sequence() ;
    Sequence(int rows, int cols, int dur, std::string name);
    int getRows();
    int getCols();
    int getFrameDur();
    QString openFile();
    std::string getName();
    QVector< QVector< int > > getGrid();

public slots:
    void save();
    void load();
};


#endif // SEQUENCE_H

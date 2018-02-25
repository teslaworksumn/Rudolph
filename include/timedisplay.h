#ifndef TIMEDISPLAY_H
#define TIMEDISPLAY_H

#include <QAbstractTableModel>

const int columnheader= 21600;
const int rowheader= 1;
const int timeIntervalHeader = 25;


class timeDisplay : public QAbstractTableModel
{
    Q_OBJECT
public:
    timeDisplay(QObject *parent);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, const Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

};

#endif // TIMEDISPLAY_H



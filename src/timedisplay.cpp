#include "./include/timedisplay.h"
#include <QDebug>

timeDisplay::timeDisplay(QObject *parent)
    :QAbstractTableModel(parent)
{

}

int timeDisplay::rowCount(const QModelIndex & /*parent*/) const
{
    return rowheader;
}

int timeDisplay::columnCount(const QModelIndex & /*parent*/) const
{
    return columnheader;
}

QVariant timeDisplay::data(const QModelIndex &index, int role) const
{
    if(index.row() == 0 && role == Qt::DisplayRole) {
        if(index.column() % 40 == 0){
        int second = index.column() / 60;

        QString timeString;
        if  (second > 60){
            if (second % 60 < 10){
            timeString.push_back(QString::number(second / 60));
            timeString.push_back(":");
            timeString.push_back(QString::number(0));

            timeString.push_back(QString::number(second % 60));
        }
            else {
                timeString.push_back(QString::number(second / 60));
                timeString.push_back(":");
                timeString.push_back(QString::number(second % 60));
            }

            }
        else{
            if (second < 10){
                timeString.push_back(":");
                timeString.push_back(QString::number(0));
                timeString.push_back(QString::number(second));
            }
            else{
                timeString.push_back(":");
                timeString.push_back(QString::number(second));

            }
        }

        return timeString;
        }
    }
    return QString();
}
QVariant timeDisplay::headerData(int section, const Qt::Orientation orientation, int role) const
{
    // HORIZONTAL HEADER: (timeline)
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
         return QVariant();
    }

    // VERTICAL HEADER: PUs
    if (orientation == Qt::Vertical && role == Qt::DisplayRole)

    {
        QString sectionString = QVariant(section).toString();
        return QString();

    }
    return QVariant();

}







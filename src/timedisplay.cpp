#include "./include/timedisplay.h"

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
   if(index.isValid() != true || role)
    return QVariant();
   return QVariant();
}
QVariant timeDisplay::headerData(int section, const Qt::Orientation orientation, int role) const
{
    // HORIZONTAL HEADER: (timeline)
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
      int time = section * timeIntervalHeader;
      int minute = time / 60;
      int second = time % 60;
      QString timeString = QString::number(minute);
      timeString.append( ":");
      if(second<=9){
          timeString.append("0");
          timeString.append(second);
      }
      else{
          timeString.append(second);
      }
      return timeString;
    }

    // VERTICAL HEADER: PUs
    if (orientation == Qt::Vertical && role == Qt::DisplayRole)

    {
        QString sectionString = QVariant(section).toString();
        return QString("Channel1024");

    }
    return QVariant();

}







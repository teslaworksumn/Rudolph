#include "./include/cellrenderer.h"
#include <QBrush>
#include <QVariant>
#include <QVector>

QVector< QVector< int > > gridData;


CellRenderer::CellRenderer(QObject *parent)
    :QAbstractTableModel(parent)
{

    for (int i = 0; i < COLS; i++) {
        QVector<int> col;
        for (int j = 0; j < ROWS; j++) {
            col.push_back(0);
        }
        gridData.append(col);
    }

}

int CellRenderer::rowCount(const QModelIndex & /*parent*/) const
{
    return ROWS;
}

int CellRenderer::columnCount(const QModelIndex & /*parent*/) const
{
    return COLS;
}

QVariant CellRenderer::data(const QModelIndex &index, int role) const
{
   if(index.isValid() != true || role)
    return QVariant();
   return QVariant();
}
QVariant CellRenderer::headerData(int section, const Qt::Orientation orientation, int role) const
{
    // HORIZONTAL HEADER: (timeline)
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
      int time = section * timeInterval;
      if(time % 1000 == 0 ){
          return QString ("|\n|");
      }
      else{
          return QString("|");
      }


    }

    // VERTICAL HEADER: PUs
    if (orientation == Qt::Vertical && role == Qt::DisplayRole)

    {
        QString sectionString = QVariant(section).toString();
        return QString("Channel") + sectionString;

            }
    return QVariant();

}

void CellRenderer::editData(const QModelIndex & index)
{
    if(gridData[index.column()][index.row()] == 255){
    gridData[index.column()][index.row()] = 0;
    }
    else {
        gridData[index.column()][index.row()] = 255;
    }
}

void CellRenderer::editDataSpace(const QModelIndexList & indexList)
{
    for(QModelIndex index :indexList){
        if(gridData[index.column()][index.row()] == 255){
        gridData[index.column()][index.row()] = 0;
        }
        else {
            gridData[index.column()][index.row()] = 255;
        }
    }
}







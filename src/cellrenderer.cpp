#include "./include/cellrenderer.h"
#include <QBrush>
#include <QVariant>

int gridData[ROWS][COLS];
CellRenderer::CellRenderer(QObject *parent)
    :QAbstractTableModel(parent)
{

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
    if(gridData[index.row()][index.column()] == 255){
    gridData[index.row()][index.column()] = 0;
    }
    else {
        gridData[index.row()][index.column()] = 255;
    }
}

void CellRenderer::editDataSpace(const QModelIndexList & indexList)
{
    for(QModelIndex index :indexList){
        if(gridData[index.row()][index.column()] == 255){
        gridData[index.row()][index.column()] = 0;
        }
        else {
            gridData[index.row()][index.column()] = 255;
        }
    }
}






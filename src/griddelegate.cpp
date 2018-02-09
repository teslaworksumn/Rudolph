#include "./include/griddelegate.h"
#include "./include/cellrenderer.h"
#include "./include/mainwindow.h"

#include <QtWidgets>
#include <QPainter>


GridDelegate::GridDelegate(QWidget *parent)
    : QAbstractItemDelegate(parent)
{
}

void GridDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                         const QModelIndex &index) const
{
    if ( gridData[index.row()][index.column()] != 0) {
            painter->fillRect(option.rect, Qt::red);

}
        if (option.state & QStyle::State_Selected)
            painter->fillRect(option.rect, option.palette.highlight());


}

QSize GridDelegate::sizeHint(const QStyleOptionViewItem &option,
                             const QModelIndex &index) const
{
    return QSize(1, 1);

}



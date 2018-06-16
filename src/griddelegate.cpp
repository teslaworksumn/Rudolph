#include "./include/griddelegate.h"


GridDelegate::GridDelegate(QWidget *parent)
    : QAbstractItemDelegate(parent)
{
}

void GridDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                         const QModelIndex &index) const
{
    if ( gridData[index.column()][index.row()] != 0)
    {
        painter->fillRect(option.rect, Qt::red);
    }

    if (option.state & QStyle::State_Selected)
        painter->fillRect(option.rect, option.palette.highlight());

}

QSize GridDelegate::sizeHint(const QStyleOptionViewItem &option,
                             const QModelIndex &index) const
{
    QStyleOptionViewItem error = option;
    if(index.isValid())
    {
        return QSize(1, 1);
    }
    return QSize(1, 1);
}




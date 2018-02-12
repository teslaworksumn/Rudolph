#ifndef GRIDDELEGATE_H
#define GRIDDELEGATE_H

#include <QStyledItemDelegate>


class GridDelegate : public QAbstractItemDelegate
{
    Q_OBJECT

public:
    GridDelegate(QWidget *parent = 0);

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
            const QModelIndex &index) const ;
    QSize sizeHint(const QStyleOptionViewItem &option,
            const QModelIndex &index) const override ;
 };



#endif // GRIDDELEGATE_H

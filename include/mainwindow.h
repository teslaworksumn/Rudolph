#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>
#include <QMainWindow>
#include <QTableView>
#include "mainwindow.h"
#include "cellrenderer.h"
#include "ui_mainwindow.h"
#include <QKeyEvent>
#include <QAbstractTableModel>

QT_BEGIN_NAMESPACE // QT_BEGIN_NAMESPACE / QT_END_NAMESPACE are not needed in Qt user code
class QTableView; //forward declaration
QT_END_NAMESPACE

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    Ui::MainWindow *ui;
    QTableView *tableView;

public:
    explicit MainWindow(QWidget *parent = 0);
    bool eventFilter(QObject *object, QEvent *e);
    ~MainWindow();
protected:
    void resizeEvent(QResizeEvent *event) override;

signals:
    void space(const QModelIndexList &);

public slots:
    void updateViewRow(int size);
    void updateViewColumn(int size);
};

#endif // MAINWINDOW_H

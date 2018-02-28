#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>
#include <QMainWindow>
#include <QKeyEvent>
#include <QAbstractTableModel>
#include <QTableView>
#include "./include/cellrenderer.h"
#include "./include/timedisplay.h"

#include "./include/griddelegate.h"
#include "./include/sequence.h"
#include "ui_mainwindow.h"


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
    Ui::MainWindow*ui;
    QTableView *tableView;

public:
    explicit MainWindow(QWidget *parent = 0);
    bool eventFilter(QObject *object, QEvent *e);
    ~MainWindow();
protected:
    void resizeEvent(QResizeEvent *event) override;

signals:
    void space(const QModelIndexList &);
    void save();

public slots:
    void updateViewRow(int size);
    void updateViewColumn(int size);
private slots:
    void on_actionNew_Sequence_triggered();
    void startPlaying();
    void stopPlaying();
    void onTimer();
    void onTimerScroll();
};

#endif // MAINWINDOW_H

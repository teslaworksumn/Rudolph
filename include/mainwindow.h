#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>
#include <QMainWindow>
#include <QKeyEvent>
#include <QMediaPlayer>
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
    void setUpResize();
    void setUpUI();

protected:
    void resizeEvent(QResizeEvent *event) override;

signals:
    void space(const QModelIndexList &);
    void save();
    void stop();

public slots:
    void updateViewRow();
    void updateViewColumn();
    void startMusicPlayer();
    void stopPlaying();
    void onTimer();
    void moveIndex();
    void animateLine();
};

#endif // MAINWINDOW_H

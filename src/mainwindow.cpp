#include <QTableView>
#include "./include/mainwindow.h"
#include "./include/cellrenderer.h"
#include "./include/griddelegate.h"
#include "ui_mainwindow.h"
#include <QKeyEvent>


MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent),
        ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QMainWindow::showMaximized(); // make Rudolph maximized on startup

    ui->tableView->installEventFilter(this);
    QAbstractTableModel *CellRender = new CellRenderer(this);
    ui->tableView->setModel(CellRender);
    QAbstractItemDelegate *GridPainter = new GridDelegate(this);
    ui->tableView->setItemDelegate(GridPainter);
    ui->tableView->setSelectionMode(QAbstractItemView::ExtendedSelection);


    ui->rowSizeBox->addItem(tr("10%"));
    ui->rowSizeBox->addItem(tr("20%"));
    ui->rowSizeBox->addItem(tr("30%"));
    ui->rowSizeBox->addItem(tr("40%"));
    ui->rowSizeBox->addItem(tr("50%"));
    ui->rowSizeBox->addItem(tr("60%"));
    ui->rowSizeBox->addItem(tr("70%"));
    ui->rowSizeBox->addItem(tr("80%"));
    ui->rowSizeBox->addItem(tr("90%"));
    ui->rowSizeBox->addItem(tr("100%"));


    ui->columnSizeBox->addItem(tr("10%"));
    ui->columnSizeBox->addItem(tr("20%"));
    ui->columnSizeBox->addItem(tr("30%"));
    ui->columnSizeBox->addItem(tr("40%"));
    ui->columnSizeBox->addItem(tr("50%"));
    ui->columnSizeBox->addItem(tr("60%"));
    ui->columnSizeBox->addItem(tr("70%"));
    ui->columnSizeBox->addItem(tr("80%"));
    ui->columnSizeBox->addItem(tr("90%"));
    ui->columnSizeBox->addItem(tr("100%"));


    for (int col = 0; col < CellRender->columnCount(); col++) {
        ui->tableView->setColumnWidth(col, 15); // pixel width of cells // needs to be resizeable in future
    }
    for (int row = 0; row < CellRender->rowCount(); row++) {
        ui->tableView->setRowHeight(row, 15); // pixel height of cells // needs to be resizeable in future
    }



    connect(ui->tableView, SIGNAL(doubleClicked(const QModelIndex &)), CellRender, SLOT(editData(const QModelIndex &)));
    connect(this, SIGNAL(space(const QModelIndexList &)), CellRender, SLOT(editDataSpace(const QModelIndexList &)));
    connect(ui->rowSizeBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateViewRow(int)));
    connect(ui->columnSizeBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateViewColumn(int)));

    // types need to be the same
    connect(ui->playSequenceButton, SIGNAL(clicked(bool)), ui->scrollingLine, SLOT(move(bool)));

    // TODO: extract into different function
    qDebug() << "height: " << ui->tableView->height();
    qDebug() << "central widget height: " << ui->centralwidget->height();
    ui->scrollingLine->setGeometry(ui->tableView->x() + ui->tableView->verticalHeader()->width(), // place at beginning of first column
                                   ui->tableView->y(),
                                   1,
                                   ui->tableView->height() - ui->tableView->horizontalScrollBar()->height()); // don't overlap bottom scroll bar
    QPalette pal = palette();
    pal.setColor(QPalette::Background, Qt::red); // TODO: make color a user option(?)
    ui->scrollingLine->setAutoFillBackground(true);
    ui->scrollingLine->setPalette(pal);
    ui->scrollingLine->show();

    //QPropertyAnimation animation(ui->scrollingLine, "geometry");
    //animation.setDuration(1000);
    //animation.setStartValue(QRect(ui->scrollingLine->x(), ui->scrollingLine->y(), ui->scrollingLine->width(), ui->scrollingLine->height()));
    //qDebug() << "got past start value";
    //animation.setEndValue(QRect(ui->scrollingLine->x() + 500,ui->scrollingLine->y(), ui->scrollingLine->width(), ui->scrollingLine->height()));

    QFrame *button = new QFrame(this);
    button->setGeometry(20, 20, 100, 30);
    button->setAutoFillBackground(true);
    button->setPalette(pal);
    button->show();

    QPropertyAnimation *animation = new QPropertyAnimation(button, "geometry");
    animation->setDuration(10000);
    animation->setStartValue(QRect(0, 0, 100, 30));
    animation->setEndValue(QRect(250, 250, 100, 30));

    connect(animation, SIGNAL(finished()), animation, SLOT(deleteLater()));
    animation->start();

/*
    QPushButton *button = new QPushButton(this);
    button->setGeometry(0, 0, 100, 30);
    button->show();

    QPropertyAnimation *animation = new QPropertyAnimation(button, "geometry");
    animation->setDuration(10000);
    animation->setStartValue(QRect(0, 0, 100, 30));
    animation->setEndValue(QRect(250, 250, 100, 30));

    connect(animation, SIGNAL(finished()), animation, SLOT(deleteLater()));
    animation->start();
*/
}

bool MainWindow::eventFilter(QObject *object, QEvent *e)
{
    // Get selected text on keypress event ctrl+e
    if (object == ui->tableView) {
        if (e->type() == QEvent::KeyPress) {
            QKeyEvent *keyEvent = static_cast<QKeyEvent*>(e);

            if (keyEvent->key() == Qt::Key_Space) {
                emit space(ui->tableView->selectionModel()->selectedIndexes());
                return true;
            }
        }
    }

    return false;
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *resizeEvent)
{
    ui->tableView->resize((resizeEvent->size()) - QSize(60,90));
    ui->scrollingLine->setGeometry(ui->tableView->x() + ui->tableView->verticalHeader()->width(), // place at beginning of first column
                                   ui->tableView->y(),
                                   1,
                                   ui->tableView->height() - ui->tableView->horizontalScrollBar()->height()); // don't overlap bottom scroll bar
}

void MainWindow::updateViewRow(int size)
{
    int cellSize = 5;
    switch(size){
    case 0 : cellSize = 5;
        break;
    case 1 : cellSize = 10;
        break;
    case 2 : cellSize = 15;
        break;
    case 3 : cellSize = 25;
        break;
    case 4 : cellSize = 30;
        break;
    case 5 : cellSize = 35;
        break;
    case 6 : cellSize = 40;
        break;
    case 7 : cellSize = 45;
        break;
    case 8 : cellSize = 50;
        break;
    case 9 : cellSize = 55;
        break;
    case 10 : cellSize = 60;
        break;
    }

    for (int row = 0; row < ui->tableView->model()->rowCount(); row++) {
        ui->tableView->setRowHeight(row, cellSize); // pixel height of cells // needs to be resizeable in future
    }
}

void MainWindow::updateViewColumn(int size)
{
    int cellSize = 5;
    switch(size){
    case 0 : cellSize = 5;
        break;
    case 1 : cellSize = 10;
        break;
    case 2 : cellSize = 15;
        break;
    case 3 : cellSize = 25;
        break;
    case 4 : cellSize = 30;
        break;
    case 5 : cellSize = 35;
        break;
    case 6 : cellSize = 40;
        break;
    case 7 : cellSize = 45;
        break;
    case 8 : cellSize = 50;
        break;
    case 9 : cellSize = 55;
        break;
    case 10 : cellSize = 60;
        break;
    }

    for (int col = 0; col < ui->tableView->model()->columnCount(); col++) {
        ui->tableView->setColumnWidth(col, cellSize); // pixel width of cells // needs to be resizeable in future
    }
}

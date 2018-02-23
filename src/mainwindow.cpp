#include "./include/mainwindow.h"
#include <QTest>
#include <QDebug>

bool running = false;

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

    QString sequenceName = "current";
    Sequence *currentSequence = new Sequence(CellRender->rowCount(),CellRender->columnCount(),25,sequenceName);

    for (int i = 0; i< 10; i++){
        QString s = QString::number((i*10) +10);
        s.append( "%");

        ui->rowSizeBox->addItem(s);
        ui->columnSizeBox->addItem(s);

    }

    ui->tableView->setAutoScroll(true);



    QPalette pal = palette();
    pal.setColor(QPalette::Background, Qt::red); // TODO: make color a user option(?)

    ui->scrollingLine->setGeometry(ui->tableView->x() + ui->tableView->verticalHeader()->width(), // place at beginning of first column
                                   ui->tableView->y(),
                                   1,
                                   ui->tableView->height() - ui->tableView->horizontalScrollBar()->height()); // don't overlap bottom scroll bar
    ui->scrollingLine->setAutoFillBackground(true);
    ui->scrollingLine->setPalette(pal);
    ui->scrollingLine->show();


    for (int col = 0; col < CellRender->columnCount(); col++) {
        ui->tableView->setColumnWidth(col, 15); // pixel width of cells // needs to be resizeable in future
    }

    for (int row = 0; row < CellRender->rowCount(); row++) {
        ui->tableView->setRowHeight(row, 15); // pixel height of cells // needs to be resizeable in future
    }

    //transfer changes to the model to the window title
    connect(ui->tableView, SIGNAL(doubleClicked(const QModelIndex &)), CellRender, SLOT(editData(const QModelIndex &)));
    connect(this, SIGNAL(space(const QModelIndexList &)), CellRender, SLOT(editDataSpace(const QModelIndexList &)));
    connect(ui->save, SIGNAL(clicked()), currentSequence, SLOT(save()));
    connect(ui->load, SIGNAL(clicked()), currentSequence, SLOT(load()));
    connect(this, SIGNAL(save()), currentSequence, SLOT(save()));
    connect(ui->rowSizeBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateViewRow(int)));
    connect(ui->columnSizeBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateViewColumn(int)));
    connect(ui->playButton, SIGNAL(clicked()), this, SLOT(startPlaying()));
    connect(ui->playButton, SIGNAL(clicked()), this, SLOT(onTimer()));
    connect(ui->stopButton, SIGNAL(clicked()), this, SLOT(stopPlaying()));

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
        else if (e->type() == QEvent::KeyRelease)
        {
            QKeyEvent *key_event = static_cast<QKeyEvent*>(e);

            if (key_event->matches(QKeySequence::Save))
            {
                emit save();
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
    int cellSize = (5 * size) + 5;

    for (int row = 0; row < ui->tableView->model()->rowCount(); row++) {
        ui->tableView->setRowHeight(row, cellSize); // pixel height of cells // needs to be resizeable in future
    }
}

void MainWindow::updateViewColumn(int size)
{
    int cellSize = (5 * size) + 5;

    for (int col = 0; col < ui->tableView->model()->columnCount(); col++) {
        ui->tableView->setColumnWidth(col, cellSize); // pixel width of cells // needs to be resizeable in future
    }
}

void MainWindow::startPlaying(){
    QPropertyAnimation *animation = new QPropertyAnimation(ui->scrollingLine, "geometry");
    animation->setDuration(10000);
    animation->setStartValue(QRect(ui->tableView->x() + ui->tableView->verticalHeader()->width(), // place at beginning of first column
                                   ui->tableView->y(),
                                   1,
                                   ui->tableView->height() - ui->tableView->horizontalScrollBar()->height())); // don't overlap bottom scroll bar);
    animation->setEndValue(QRect(ui->tableView->x() + ui->tableView->verticalHeader()->width()+ui->tableView->height(), // place at beginning of first column
                                 ui->tableView->y(),
                                 1,
                                 ui->tableView->height() - ui->tableView->horizontalScrollBar()->height())); // don't overlap bottom scroll bar);

    connect(animation, SIGNAL(finished()), animation, SLOT(deleteLater()));
    connect(ui->stopButton, SIGNAL(clicked()), animation, SLOT(stop()));

    animation->start();
    running = true;
}



void MainWindow::stopPlaying(){
    running = false;
}


void MainWindow::onTimer()
{
    QModelIndex nextIndex = ui->tableView->currentIndex().sibling(
                ui->tableView->currentIndex().row(), ui->tableView->currentIndex().column() +1 );
    if(nextIndex.isValid() && running == true){
        ui->tableView->setCurrentIndex(nextIndex);
        QTimer::singleShot(25, this, SLOT(onTimer()));
        QTimer::singleShot(25, this, SLOT(onTimerScroll()));
    }
    else{
        return;
    }
}

void MainWindow::onTimerScroll()
{
        ui->tableView->horizontalScrollBar()->setValue(ui->tableView->horizontalScrollBar()->sliderPosition() + 2);
}





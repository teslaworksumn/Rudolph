#include "./include/mainwindow.h"
#include <QTest>
#include <QDebug>

bool running = false;
int cellSize = 15;
int reSizeHigher = 0;
int reSizeLower = 0;

int spacer = 0;
int rightStart = 0;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow)

{
    ui->setupUi(this);
    QMainWindow::showMaximized(); // make Rudolph maximized on startup


    QAbstractTableModel *CellRender = new CellRenderer(this);
    QAbstractTableModel *header = new timeDisplay(this);


    ui->tableView->installEventFilter(this);
    ui->tableView->setModel(CellRender);
    ui->timeHeader->setModel(header);
    QAbstractItemDelegate *GridPainter = new GridDelegate(this);

    ui->tableView->setItemDelegate(GridPainter);
    ui->tableView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->tableView->horizontalHeader()->setSectionResizeMode (QHeaderView::Fixed);
    ui->tableView->verticalHeader()->setSectionResizeMode (QHeaderView::Fixed);
    ui->timeHeader->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);



    QString sequenceName = "current";
    Sequence *currentSequence = new Sequence(CellRender->rowCount(),CellRender->columnCount(),25,sequenceName);

    QMenu *rowSize = ui->menuView->addMenu("Row Size");
    QMenu *colSize = ui->menuView->addMenu("Column Size");
    QSignalMapper* signalMapperRow = new QSignalMapper (this) ;
    QSignalMapper* signalMapperCol = new QSignalMapper (this) ;

    for (int i = 0; i< 10; i++){
        QString s = QString::number((i*10) +10);
        s.append( "%");
        QAction *resizeRow = new QAction(s);
        QAction *resizeCol = new QAction(s);
        connect(resizeRow, SIGNAL(triggered()), signalMapperRow, SLOT(map()));
        connect(resizeCol, SIGNAL(triggered()), signalMapperCol, SLOT(map()));
        signalMapperCol -> setMapping(resizeCol, i);
        signalMapperRow -> setMapping(resizeRow, i);

        rowSize->addAction(resizeRow);
        colSize->addAction(resizeCol);
    }
    connect ( signalMapperRow, SIGNAL(mapped(int)),this, SLOT(updateViewRow(int)));
    connect ( signalMapperCol, SIGNAL(mapped(int)),this, SLOT(updateViewColumn(int)));

    ui->tableView->setAutoScroll(true);
    ui->tableView->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    ui->timeHeader->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    ui->tableView->horizontalScrollBar()->setRange(0,84600);
    ui->timeHeader->horizontalScrollBar()->setRange(0,86400);
    ui->timeHeader->horizontalHeader()->hide();
    ui->timeHeader->verticalHeader()->setFixedSize(QSize(ui->tableView->verticalHeader()->size()));
    ui->playButton->setText("\u25B6");
    ui->stopButton->setText("\u25FC");


    QPalette pal = palette();
    pal.setColor(QPalette::Background, Qt::red); // TODO: make color a user option(?)
    ui->timeHeader->horizontalScrollBar()->setStyleSheet("QScrollBar {height:0px;}");
    ui->scrollLine->hide();


    //ui->tableView->setSpan(0,0,2,2);

    for (int col = 0; col <= CellRender->columnCount(); col++) {
        ui->tableView->setColumnWidth(col, 15);
        ui->timeHeader->setColumnWidth(col, 15); // pixel width of cells // needs to be resizeable in future
        // pixel width of cells // needs to be resizeable in future
        if( col % 40 == 0){
        ui->timeHeader->setSpan(0, col, 40, 40); // HARD CODE
        }
    }

    for (int row = 0; row <= CellRender->rowCount(); row++) {
        ui->tableView->setRowHeight(row, 15);
        ui->timeHeader->setRowHeight(row, 15);
        // pixel height of cells // needs to be resizeable in future
    }



    //transfer changes to the model to the window title
    connect(ui->tableView, SIGNAL(doubleClicked(const QModelIndex &)), CellRender, SLOT(editData(const QModelIndex &)));
    connect(this, SIGNAL(space(const QModelIndexList &)), CellRender, SLOT(editDataSpace(const QModelIndexList &)));
    connect(ui->actionSave, SIGNAL(triggered(bool)), currentSequence, SLOT(save()));
    connect(ui->actionLoad, SIGNAL(triggered(bool)), currentSequence, SLOT(load()));
    connect(this, SIGNAL(save()), currentSequence, SLOT(save()));
    connect(ui->playButton, SIGNAL(clicked()), this, SLOT(startPlaying()));
    connect(ui->playButton, SIGNAL(clicked()), this, SLOT(onTimer()));
    connect(ui->stopButton, SIGNAL(clicked()), this, SLOT(stopPlaying()));
    connect(this, SIGNAL(timingHigher()), this, SLOT(updateViewColumnTimerHigher()));
    connect(this, SIGNAL(timingLower()), this, SLOT(updateViewColumnTimerLower()));
    connect(ui->tableView->horizontalScrollBar(), SIGNAL(valueChanged(int)),
            ui->timeHeader->horizontalScrollBar(), SLOT(setValue(int)));
    connect(ui->timeHeader->horizontalScrollBar(), SIGNAL(valueChanged(int)),
            ui->tableView->horizontalScrollBar(), SLOT(setValue(int)));






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
    ui->tableView->resize((resizeEvent->size()) - QSize(60,115));
    ui->timeHeader->resize((resizeEvent->size()) - QSize(60,1000));
    ui->timeHeader->move(ui->tableView->x(), (ui->tableView->y()) - (ui->timeHeader->rowHeight(0)));
}


void MainWindow::syncScroll(int value)
{
    //int set = (value * 255) / cellSize;
    int set = (value /15);
    ui->timeHeader->horizontalScrollBar()->setValue(set);
}


void MainWindow::updateViewRow(int size)
{
    cellSize = (5 * size) + 5;

    for (int row = 0; row < ui->tableView->model()->rowCount(); row++) {
        ui->tableView->setRowHeight(row, cellSize); // pixel height of cells // needs to be resizeable in future
    }
}

void MainWindow::updateViewColumn(int size)
{
    reSizeHigher = ui->tableView->indexAt(ui->tableView->rect().topLeft()).column();
    reSizeLower = ui->tableView->indexAt(ui->tableView->rect().topLeft()).column();
    rightStart = ui->tableView->indexAt(ui->tableView->rect().bottomRight()).column();
    int currentSpacer = ui->tableView->columnWidth(1) * (rightStart - ui->tableView->currentIndex().column())  ;


    cellSize = (5 * size) + 5;
    spacer = currentSpacer / cellSize;
    emit timingHigher();
    emit timingLower();
}

void MainWindow::updateViewColumnTimerHigher()
{
    // pixel width of cells // needs to be resizeable in future

    ui->tableView->setColumnWidth(reSizeHigher, cellSize);
    ui->timeHeader->setColumnWidth(reSizeHigher, cellSize);

    reSizeHigher = reSizeHigher + 1;
    if( reSizeHigher != 86399){ // hard code value
        QTimer::singleShot(1, this, SLOT(updateViewColumnTimerHigher()));

    }

    else{
        return;
    }
}
void MainWindow::updateViewColumnTimerLower()
{
    // pixel width of cells // needs to be resizeable in future

    QModelIndex nextIndex = ui->tableView->currentIndex().sibling(
                ui->tableView->currentIndex().row(), ui->tableView->currentIndex().column() + spacer); // weird column matching here


    ui->tableView->setColumnWidth(reSizeLower, cellSize);
    ui->timeHeader->setColumnWidth(reSizeLower, cellSize);

    reSizeLower = reSizeLower -1;

    if (reSizeLower != 0) {
        ui->tableView->scrollTo(nextIndex, QAbstractItemView::	EnsureVisible);

        QTimer::singleShot(1, this, SLOT(updateViewColumnTimerLower()));
}
    else{
        return;
    }
}

void MainWindow::startPlaying(){
    ui->scrollLine->setGeometry(QRect(ui->tableView->x() + ui->tableView->verticalHeader()->width(), // place at beginning of first column
                                  ui->tableView->y(),
                                  1,
                                  ui->tableView->height() - ui->tableView->horizontalScrollBar()->height())); // don't overlap bottom scroll bar););
    QPropertyAnimation *animation = new QPropertyAnimation(ui->scrollLine, "geometry");
    ui->scrollLine->show();
    QModelIndex left = ui->tableView->indexAt(ui->tableView->rect().topLeft());
    QModelIndex right = ui->tableView->indexAt(ui->tableView->rect().bottomRight());
    QModelIndex current = ui->tableView-> currentIndex();
    int start = ( (( (current.column() - left.column()))));
    int stop = ( (( (right.column() - left.column())))/5);

    animation->setStartValue(QRect(ui->tableView->x() + ui->tableView->verticalHeader()->width() + (start *  ui->tableView->columnWidth(1)), // place at beginning of first column
                                   ui->tableView->y(),
                                   1,
                                   ui->tableView->height() - ui->tableView->horizontalScrollBar()->height())); // don't overlap bottom scroll bar);
    animation->setEndValue(QRect(ui->tableView->x() + ui->tableView->verticalHeader()->width() + (stop * ui->tableView->columnWidth(1))-((ui->tableView->columnWidth(1))/2), // place at beginning of first column
                                 ui->tableView->y(),
                                 1,
                                 ui->tableView->height() - ui->tableView->horizontalScrollBar()->height())); // don't overlap bottom scroll bar);

    if( current.column() < stop){
        int spacer = stop - start;
        animation->setDuration( spacer * 25  );

    }
    else{
        int spacer = start - stop ;
        animation->setDuration( spacer * 25  );

    }
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
                ui->tableView->currentIndex().row(), ui->tableView->currentIndex().column() + 1);
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
    QModelIndex left = ui->tableView->indexAt(ui->tableView->rect().topLeft());
    QModelIndex right = ui->tableView->indexAt(ui->tableView->rect().bottomRight());
    int thirds = (3*(right.column() - left.column()));
    int delay = (thirds/4);
    QModelIndex nextIndex = ui->tableView->currentIndex().sibling(
                ui->tableView->currentIndex().row(), ui->tableView->currentIndex().column() + delay +2 ); // weird column matching here
    ui->tableView->scrollTo(nextIndex, QAbstractItemView::EnsureVisible);
}






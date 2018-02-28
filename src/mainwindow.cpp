#include "./include/mainwindow.h"
#include "./include/newsequencedialog.h"

#include <QDebug>
#include <math.h>

bool running = false;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow)

{
    ui->setupUi(this);
    //QMainWindow::showMaximized(); // make Rudolph maximized on startup
    ui->timeHeader->move(ui->tableView->x(), (ui->tableView->y()) - (ui->timeHeader->rowHeight(0)));



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
    ui->timeHeader->move(ui->tableView->x(), (ui->tableView->y()) - (ui->timeHeader->rowHeight(0)));


    QString sequenceName = "current";
    Sequence *currentSequence = new Sequence(CellRender->rowCount(),CellRender->columnCount(),25,sequenceName); // does little

    QMenu *rowSize = ui->menuView->addMenu("Row Size");
    QMenu *colSize = ui->menuView->addMenu("Column Size");
    QSignalMapper* signalMapperRow = new QSignalMapper (this) ; // consider getting rid of this
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
    ui->tableView->setHorizontalScrollMode(QAbstractItemView::ScrollPerItem);
    ui->timeHeader->setHorizontalScrollMode(QAbstractItemView::ScrollPerItem);
    ui->timeHeader->horizontalHeader()->hide();
    ui->timeHeader->verticalHeader()->setFixedSize(QSize(ui->tableView->verticalHeader()->size()));
    ui->playButton->setText("\u25B6");
    ui->stopButton->setText("\u25FC");

    QPalette pal = palette();
    pal.setColor(QPalette::Background, Qt::red); // TODO: make color a user option(?)
    ui->timeHeader->horizontalScrollBar()->setStyleSheet("QScrollBar {height:0px;}");
    ui->scrollLine->hide();
    ui->tableView->setFont(QFont( "Arial", 11, QFont::Bold));



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
    //ui->tableView->horizontalScrollBar()->setSingleStep(ui->tableView->columnWidth(1));



    //transfer changes to the model to the window title
    connect(ui->tableView, SIGNAL(doubleClicked(const QModelIndex &)), CellRender, SLOT(editData(const QModelIndex &)));
    connect(this, SIGNAL(space(const QModelIndexList &)), CellRender, SLOT(editDataSpace(const QModelIndexList &)));
    connect(ui->actionSave, SIGNAL(triggered(bool)), currentSequence, SLOT(save()));
    connect(ui->actionLoad, SIGNAL(triggered(bool)), currentSequence, SLOT(load()));
    connect(this, SIGNAL(save()), currentSequence, SLOT(save()));
    connect(ui->playButton, SIGNAL(clicked()), this, SLOT(startPlaying()));
    connect(ui->playButton, SIGNAL(clicked()), this, SLOT(onTimer()));
    connect(ui->stopButton, SIGNAL(clicked()), this, SLOT(stopPlaying()));

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
    ui->tableView->resize((resizeEvent->size()) - QSize(90,115));
    ui->timeHeader->resize((resizeEvent->size()) - QSize(90,1000));
    ui->timeHeader->move(ui->tableView->x(), (ui->tableView->y()) - (ui->timeHeader->rowHeight(0)));
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
        ui->tableView->horizontalHeader()->resizeSection(col,cellSize);
        ui->timeHeader->horizontalHeader()->resizeSection(col,cellSize);
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
    int start = ((current.column() - left.column()));
    int stop = ((right.column() - left.column())/5);


    int picker[10] = {40,20,20,15,13,11,9,8,7,5};
    int cellSize = ui->tableView->columnWidth(1);
    int select = (cellSize /5)-1;

    animation->setStartValue(QRect(ui->tableView->x() + ui->tableView->verticalHeader()->width() + (start *  ui->tableView->columnWidth(1)), // place at beginning of first column
                                   ui->tableView->y(),
                                   1,
                                   ui->tableView->height() - ui->tableView->horizontalScrollBar()->height())); // don't overlap bottom scroll bar);

    animation->setEndValue(QRect(ui->tableView->x()  + (picker[select] *  ui->tableView->columnWidth(1)), // place at beginning of first column
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
    ui->tableView->setFocus();

    connect(animation, SIGNAL(finished()), animation, SLOT(deleteLater()));
    connect(ui->stopButton, SIGNAL(clicked()), animation, SLOT(stop()));

    animation->start();
    running = true;
}



void MainWindow::stopPlaying(){
    running = false;
    ui->tableView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    setFocus();
}


void MainWindow::onTimer()
{
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(onTimerScroll()));
    timer->setTimerType(Qt::PreciseTimer);
    connect(ui->stopButton, SIGNAL(clicked(bool)), timer, SLOT(stop()));
    timer->start(25);


}

void MainWindow::on_actionNew_Sequence_triggered()
{
    NewSequenceDialog * harry = new NewSequenceDialog();
    harry->show();
}

void MainWindow::onTimerScroll()
{

    if(running == true){
//        QString os = QSysInfo::productType();
//        if (os.compare("ubuntu")==0 ){


         QModelIndex nextIndex1 = ui->tableView->currentIndex().sibling(
                    ui->tableView->currentIndex().row(), ui->tableView->currentIndex().column() + 1);

            ui->tableView->setSelectionMode(QAbstractItemView::ExtendedSelection);
            ui->tableView->setCurrentIndex(nextIndex1);
            //qWarning() <<  nextIndex1.column();
            ui->tableView->setSelectionMode(QAbstractItemView::NoSelection);

            QModelIndex left = ui->tableView->indexAt(ui->tableView->rect().topLeft());
            QModelIndex right = ui->tableView->indexAt(ui->tableView->rect().bottomRight());

            int stop = ((right.column() - left.column()));
            int picker[10] = {40,20,20,15,13,11,9,8,7,5};
            int cellSize = ui->tableView->columnWidth(1);
            int select = (cellSize /5)-1;


                QModelIndex scrollIndex = ui->tableView->currentIndex().sibling(
                            ui->tableView->currentIndex().row(), ui->tableView->currentIndex().column() + (stop - picker[select])-1);
                        ui->tableView->scrollTo(scrollIndex, QAbstractItemView::EnsureVisible);



            qWarning() << ui->tableView->currentIndex().column();
            qWarning() << ui->tableView->columnAt(ui->scrollLine->x() - ui->tableView->verticalHeader()->width());
            qWarning() << "here";





       // }

//        else if(os.compare("windows") == 0 || os.compare("mac") == 0){
//            for(int i = 0; i < 2; i++){
//                QModelIndex nextIndex1 = ui->tableView->currentIndex().sibling(
//                           ui->tableView->currentIndex().row(), ui->tableView->currentIndex().column() + 1);
//               if(nextIndex1.isValid()){

//                   ui->tableView->setSelectionMode(QAbstractItemView::ExtendedSelection);
//                   ui->tableView->setCurrentIndex(nextIndex1);
//                   ui->tableView->setSelectionMode(QAbstractItemView::NoSelection);
//                   QModelIndex left = ui->tableView->indexAt(ui->tableView->rect().topLeft());
//                   QModelIndex right = ui->tableView->indexAt(ui->tableView->rect().bottomRight());
//                   int stop = ( (( (right.column() - left.column())))/5);
//                   QModelIndex nextIndex = ui->tableView->currentIndex().sibling(
//                               ui->tableView->currentIndex().row(), ui->tableView->indexAt(QPoint( ui->tableView->verticalHeader()->width() + (stop * ui->tableView->columnWidth(1)) , ui->tableView->currentIndex().row())).column() );

//                   QModelIndex scrollIndex = ui->tableView->currentIndex().sibling(
//                               ui->tableView->currentIndex().row(), ui->tableView->currentIndex().column() + (right.column() - nextIndex.column())  - 1);
//                   ui->tableView->scrollTo(scrollIndex);
//               }
//            }

//        }

    }
}

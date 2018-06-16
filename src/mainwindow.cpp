#include "./include/mainwindow.h"

bool running = false;
QMediaPlayer *player;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QAbstractTableModel *cellRender = new CellRenderer(this);
    QAbstractTableModel *headerModel = new timeDisplay(this);
    QAbstractItemDelegate *GridPainter = new GridDelegate(this);

    ui->tableView->installEventFilter(this);

    ui->tableView->setModel(cellRender);
    ui->timeHeader->setModel(headerModel);

    ui->tableView->setItemDelegate(GridPainter);
    ui->tableView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->tableView->horizontalHeader()->setSectionResizeMode (QHeaderView::Fixed);
    ui->tableView->verticalHeader()->setSectionResizeMode (QHeaderView::Fixed);
    ui->timeHeader->move(ui->tableView->x(), (ui->tableView->y()) - (ui->timeHeader->rowHeight(0)));

    QString sequenceName = "current";

    Sequence *currentSequence = new Sequence(cellRender->rowCount(),cellRender->columnCount(),25,sequenceName); // does little

    setUpUI();
    ui->tableView->setAutoScroll(true);
    ui->timeHeader->horizontalScrollBar()->setRange(0,cellRender->columnCount());
    ui->timeHeader->horizontalHeader()->hide();
    ui->timeHeader->verticalHeader()->setFixedSize(QSize(ui->tableView->verticalHeader()->size()));

    ui->timeHeader->horizontalScrollBar()->setStyleSheet("QScrollBar {height:0px;}");
    ui->tableView->setFont(QFont( "Arial", 11, QFont::Bold));
    ui->timeHeader->move(ui->tableView->x(), (ui->tableView->y()) - (ui->timeHeader->rowHeight(0)));

    QScreen *screen = QGuiApplication::primaryScreen();
    QMainWindow::resize(QSize(screen->geometry().width(),screen->geometry().height()));
    QMainWindow::showMaximized();

    //transfer changes to the model to the window title
    connect(ui->tableView, SIGNAL(doubleClicked(const QModelIndex &)), cellRender, SLOT(editData(const QModelIndex &)));
    connect(this, SIGNAL(space(const QModelIndexList &)), cellRender, SLOT(editDataSpace(const QModelIndexList &)));
    connect(ui->actionSave, SIGNAL(triggered(bool)), currentSequence, SLOT(save()));
    connect(ui->actionLoad, SIGNAL(triggered(bool)), currentSequence, SLOT(load()));
    connect(this, SIGNAL(save()), currentSequence, SLOT(save()));
    connect(ui->playButton, SIGNAL(clicked()), this, SLOT(onTimer()));
    connect(ui->playButton, SIGNAL(clicked()), this, SLOT(startMusicPlayer()));
    connect(this, SIGNAL(stop()), this, SLOT(stopPlaying()));
    connect(ui->stopButton, SIGNAL(clicked()), this, SLOT(stopPlaying()));

    connect(ui->tableView->horizontalScrollBar(), SIGNAL(valueChanged(int)),
            ui->timeHeader->horizontalScrollBar(), SLOT(setValue(int)));
    connect(ui->timeHeader->horizontalScrollBar(), SIGNAL(valueChanged(int)),
            ui->tableView->horizontalScrollBar(), SLOT(setValue(int)));
}

bool MainWindow::eventFilter(QObject *object, QEvent *e)
{
    // Get selected text on keypress event ctrl+e
    if (object == ui->tableView)
    {
        if (e->type() == QEvent::KeyPress)
        {
            QKeyEvent *keyEvent = static_cast<QKeyEvent*>(e);

            if (keyEvent->key() == Qt::Key_Space)
            {
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

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *resizeEvent)
{
    ui->tableView->resize((resizeEvent->size()) - QSize(90,130));
    ui->timeHeader->resize((resizeEvent->size()) - QSize(90,1000));
    ui->timeHeader->move(ui->tableView->x(), (ui->tableView->y()) - (ui->timeHeader->rowHeight(0)));
    ui->timeHeader->verticalHeader()->setFixedSize(QSize(ui->tableView->verticalHeader()->size()));
}

void MainWindow::setUpResize()
{
    QMenu *rowSize = ui->menuView->addMenu("Row Size");
    QMenu *colSize = ui->menuView->addMenu("Column Size");

    for (int i = 0; i< 10; i++)
    {
        QString s = QString::number((i*10) +10).append("%");
        QAction *resizeRow = new QAction(s);
        QAction *resizeCol = new QAction(s);
        resizeRow->setProperty("Size", i);
        resizeCol->setProperty("Size", i);
        connect(resizeRow, SIGNAL(triggered()), this, SLOT(updateViewRow()));
        connect(resizeCol, SIGNAL(triggered()), this, SLOT(updateViewColumn()));
        rowSize->addAction(resizeRow);
        colSize->addAction(resizeCol);
    }
}

void MainWindow::setUpUI()
{
    setUpResize();
    ui->playButton->setText("\u25B6");
    ui->playButton->setFont(QFont( "Arial", 15, QFont::Bold));
    ui->stopButton->setText("\u25FC");
    ui->stopButton->setFont(QFont( "Arial", 15, QFont::Bold));
    ui->scrollLine->hide();
    for (int col = 0; col <= ui->tableView->model()->columnCount(); col++)
    {
        ui->tableView->setColumnWidth(col, 25);
        ui->timeHeader->setColumnWidth(col, 25);
        if( col % 40 == 0)
        {
            ui->timeHeader->setSpan(0, col, 40, 40); // HARD CODE
        }
    }
    for (int row = 0; row <= ui->tableView->model()->rowCount(); row++)
    {
        ui->tableView->setRowHeight(row, 25);
        ui->timeHeader->setRowHeight(row, 25);
    }
}


void MainWindow::updateViewRow()
{
    QAction* resize = static_cast<QAction*>(sender());
    int size = resize->property("Size").toInt();
    ui->tableView->setFont(QFont( "Arial", size+1, QFont::Bold));
    int cellSize = (5 * size) + 5;
    for (int row = 0; row < ui->tableView->model()->rowCount(); row++)
    {
        ui->tableView->setRowHeight(row, cellSize); // pixel height of cells // needs to be resizeable in future
    }
}

void MainWindow::updateViewColumn()
{
    QAction* resize = static_cast<QAction*>(sender());
    int size = resize->property("Size").toInt();
    ui->tableView->setFont(QFont( "Arial", size+1, QFont::Bold));
    int cellSize = (5 * size) + 5;
    for (int col = 0; col < ui->tableView->model()->columnCount(); col++)
    {
        ui->tableView->horizontalHeader()->resizeSection(col,cellSize);
        ui->timeHeader->horizontalHeader()->resizeSection(col,cellSize);
    }
}

void MainWindow::startMusicPlayer(){
    running = true;
        QString filePath = QDir().homePath();
        QString miimusic =  filePath + "/Rudolph/Mii.mp3" ;

        player = new QMediaPlayer(this);
        player->setMedia(QUrl::fromLocalFile(miimusic));

        player->setVolume(50);

        player->setPosition(ui->tableView->currentIndex().column() * 25);  // Hard Code
        connect(ui->stopButton, SIGNAL(clicked()), player, SLOT(stop()));

        player->play();
}

void MainWindow::stopPlaying()
{
    running = false;
    ui->tableView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->scrollLine->hide();
    setFocus();
}

void MainWindow::onTimer()
{
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(moveIndex()));
    connect(timer, SIGNAL(timeout()), this, SLOT(animateLine()));
    ui->tableView->setFocus();
    timer->setTimerType(Qt::PreciseTimer);
    connect(ui->stopButton, SIGNAL(clicked(bool)), timer, SLOT(stop()));
    timer->start(25);
}

void MainWindow::animateLine()
{
    if (running == true)
    {
        QPropertyAnimation *animation = new QPropertyAnimation(ui->scrollLine, "geometry");
        QModelIndex left = ui->tableView->indexAt(ui->tableView->rect().topLeft());
        QModelIndex current = ui->tableView-> currentIndex();
        ui->scrollLine->show();
        int start = ( (( (current.column() - left.column()))));

        animation->setStartValue(QRect(ui->tableView->x() + ui->tableView->verticalHeader()->width() + (start *  ui->tableView->columnWidth(1)), // place at beginning of first column
                                       ui->tableView->y(),
                                       2,
                                       ui->tableView->height() - ui->tableView->horizontalScrollBar()->height())); // don't overlap bottom scroll bar);

        animation->setEndValue(QRect(ui->tableView->x() + ui->tableView->verticalHeader()->width() + ((start+1) *  ui->tableView->columnWidth(1)), // place at beginning of first column
                                     ui->tableView->y(),
                                     2,
                                     ui->tableView->height() - ui->tableView->horizontalScrollBar()->height())); // don't overlap bottom scroll bar)
        animation->setDuration( 25 );
        connect(animation, SIGNAL(finished()), animation, SLOT(deleteLater()));
        connect(ui->stopButton, SIGNAL(clicked()), animation, SLOT(stop()));
        animation->start();
    }
}

void MainWindow::moveIndex()
{
    if (running == true)
    {
        QModelIndex right = ui->tableView->indexAt(ui->tableView->rect().bottomRight() - QPoint(ui->tableView->verticalHeader()->width(),0) - QPoint(ui->tableView->x(),0));
        QModelIndex left = ui->tableView->indexAt(ui->tableView->rect().topLeft());
        QModelIndex current = ui->tableView-> currentIndex();
        int visible = ( (( (right.column() - left.column()))));

        if (ui->tableView->currentIndex().column() == right.column())
        {
            QModelIndex nextIndex = ui->tableView->currentIndex().sibling(
                        current.row(), current.column()  + visible);
            ui->tableView->scrollTo(nextIndex);
        }
        else
        {
            QModelIndex nextIndex1 = ui->tableView->currentIndex().sibling(
                        ui->tableView->currentIndex().row(), player->position() / 25);

            ui->tableView->setSelectionMode(QAbstractItemView::ExtendedSelection);
            ui->tableView->setCurrentIndex(nextIndex1);
            ui->tableView->setSelectionMode(QAbstractItemView::NoSelection);
        }
        if(ui->tableView->currentIndex().column() == ui->tableView->model()->columnCount())
        {
            emit(stop());
        }
    }
}










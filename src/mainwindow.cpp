#include "./include/mainwindow.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow)

{
    ui->setupUi(this);

    ui->tableView->installEventFilter(this);
    QAbstractTableModel *CellRender = new CellRenderer(this);

    ui->tableView->setModel(CellRender);
    QAbstractItemDelegate *GridPainter = new GridDelegate(this);

    ui->tableView->setItemDelegate(GridPainter);
    ui->tableView->setSelectionMode(QAbstractItemView::ExtendedSelection);

    QString sequenceName = "current";
    Sequence *currentSequence = new Sequence(CellRender->rowCount(),CellRender->columnCount(),25,sequenceName);

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

    //transfer changes to the model to the window title
    connect(ui->tableView, SIGNAL(doubleClicked(const QModelIndex &)), CellRender, SLOT(editData(const QModelIndex &)));
    connect(this, SIGNAL(space(const QModelIndexList &)), CellRender, SLOT(editDataSpace(const QModelIndexList &)));
    connect(ui->save, SIGNAL(clicked()), currentSequence, SLOT(save()));
    connect(ui->load, SIGNAL(clicked()), currentSequence, SLOT(load()));
    connect(this, SIGNAL(save()), currentSequence, SLOT(save()));
    connect(ui->rowSizeBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateViewRow(int)));
    connect(ui->columnSizeBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateViewColumn(int)));

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








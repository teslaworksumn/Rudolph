#include "./include/newsequencedialog.h"
#include "ui_newsequencedialog.h"

NewSequenceDialog::NewSequenceDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewSequenceDialog)
{
    ui->setupUi(this);
}

NewSequenceDialog::~NewSequenceDialog()
{
    delete ui;
}

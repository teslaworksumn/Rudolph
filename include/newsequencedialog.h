#ifndef NEWSEQUENCEDIALOG_H
#define NEWSEQUENCEDIALOG_H

#include <QDialog>

namespace Ui {
class NewSequenceDialog;
}

class NewSequenceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewSequenceDialog(QWidget *parent = 0);
    ~NewSequenceDialog();

private:
    Ui::NewSequenceDialog *ui;
};

#endif // NEWSEQUENCEDIALOG_H

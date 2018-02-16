#include "./include/sequence.h"
#include <QWidget>
#include <QFileDialog>
#include <QtDebug>
#include <sstream>
#include <iostream>
#include <QList>
#include <QString>
#include <QStandardPaths>

// code modified from http://www.codebind.com/cpp-tutorial/qt-tutorial/qt-tutorials-beginners-qfiledialog-getopenfilename-example/
class QFileDialogTester : public QWidget
{
public:
  QString openFile()
  {
    QString filename =  QFileDialog::getOpenFileName(
          this,
          "Open Document",
          QDir::currentPath(),
          "All files (*.*) ;; Document files (*.doc *.rtf);; PNG files (*.png)");

    if( !filename.isNull() )
    {
     return filename;
    }
    return NULL;
  }
};

Sequence::Sequence() {
    ROWS =0;
    COLS =0;
    sequenceName = "untitled";
}

Sequence::Sequence(int rows, int cols, int dur, QString name) {
    ROWS =rows;
    COLS =cols;
    sequenceName = name;
    frame_dur = dur;
}

QString Sequence::getName() {
    return sequenceName;
}

int Sequence::getRows() {
    return ROWS;
}

int Sequence::getCols() {
    return COLS;
}

int Sequence::getFrameDur() {
    return frame_dur;
}

QList<QList<uint8_t> > Sequence::getGrid() {
    return gridData;
}

void Sequence::save() {
    QString filePath = QDir().homePath();
    qDebug() << "Home: " << filePath << endl;

    if (QDir(filePath).mkdir("Rudolph")) {
        qDebug() << "Created folder" << endl;
    } else {
        qDebug() << "Could not create folder" << endl;
    }

    QString fileName = filePath + "/Rudolph/" + sequenceName + ".rud";
    QFile file(fileName);
    file.open(QIODevice::WriteOnly | QIODevice::Truncate);

    QDataStream out(&file);   // we will serialize the data into the file
    out << gridData;   // serialize a string

    file.close();
 }

void Sequence::load() {
    QFileDialogTester test;
    QString filePath = test.openFile();
    QFile file(filePath);
    file.open(QIODevice::ReadOnly);
    QDataStream in(&file);    // read the data serialized from the file
    QList<QList<uint8_t> > gridIn;
    in >> gridIn;
    gridData = gridIn;

}

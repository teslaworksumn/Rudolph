#include <QtWidgets/QApplication>
#include "./include/mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/resources/teslaworks.ico"));
    MainWindow w;
    w.show();
    return a.exec();
}

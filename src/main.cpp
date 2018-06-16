#include <QtWidgets/QApplication>
#include "./include/mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.processEvents();
    a.setWindowIcon(QIcon(":teslaworks.ico"));
    MainWindow w;
    w.show();
    w.setWindowTitle("Rudolph");
    return a.exec();
}

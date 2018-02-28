#include <QtWidgets/QApplication>
#include <QSplashScreen>
#include "./include/mainwindow.h"
#include <QPixmap>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QPixmap pm(":teslaworks.png");
    QSplashScreen splash(pm, Qt::WindowStaysOnTopHint);

    splash.show();
    a.processEvents();
    a.setWindowIcon(QIcon(":teslaworks.ico"));
    MainWindow w;
    QTimer::singleShot(2500, &splash, SLOT(close()));
    QTimer::singleShot(2500, &w, SLOT(show()));
    w.setWindowTitle("Rudolph");
    w.showMaximized();
    //splash.finish(&w);
    //delete &splash;
    return a.exec();
}

#include "ui/mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setApplicationName("ANASLO");
    MainWindow w;
    w.show();
    return QCoreApplication::exec();
}

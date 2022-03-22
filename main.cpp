#include <QApplication>
#include "mainWindow.h"
#include <QPushButton>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow mainWindow;
    mainWindow.show();
    return QApplication::exec();
}

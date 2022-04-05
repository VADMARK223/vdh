#include "MainWindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    QApplication::setWindowIcon(QIcon(":images/ava.png"));
    MainWindow mainWindow;
    mainWindow.show();

    return QApplication::exec();
}

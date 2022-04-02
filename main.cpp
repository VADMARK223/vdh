
#include "MainWindow.h"
#include <QApplication>
#include <QSettings>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    QApplication::setWindowIcon(QIcon(":images/ava.png"));
    QSettings settings("settings.ini", QSettings::IniFormat);

    settings.beginGroup("MainWindowPosition");
    int x = settings.value("x", -1).toInt();
    int y = settings.value("y", -1).toInt();
    int width = settings.value("width", -1).toInt();
    int height = settings.value("height", -1).toInt();
    settings.endGroup();

    MainWindow mainWindow;
    mainWindow.setMinimumSize(1000, 500);
    if (x > 0 && y > 0 && width > 0 && height > 0) {
        mainWindow.setGeometry(x, y, width, height);
    }
    mainWindow.show();
    int res = QApplication::exec();

    settings.beginGroup("MainWindowPosition");
    settings.setValue("x", mainWindow.x());
    settings.setValue("y", mainWindow.y());
    settings.setValue("width", mainWindow.width());
    settings.setValue("height", mainWindow.height());
    settings.endGroup();

    return res;
}

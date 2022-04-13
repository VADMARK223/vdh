//#include "src/MainWindow.h"
//#include <QPushButton>

#include <QGuiApplication>
#include <QIcon>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <QDebug>

int main(int argc, char *argv[]) {
    qDebug() << "Start";

    QQuickStyle::setStyle("Fusion");
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);
    QGuiApplication::setWindowIcon(QIcon(":images/ava.png"));

    QQmlApplicationEngine engine;
    const QUrl url("qrc:qml/main.qml");
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
                if (!obj && url == objUrl)
                    QCoreApplication::exit(-1);
            }, Qt::QueuedConnection);
    engine.load(url);

    return QGuiApplication::exec();
}

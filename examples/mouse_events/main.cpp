#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "eventitem.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    qmlRegisterType<EventItem>("EventItem", 1, 0, "EventItem");
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}

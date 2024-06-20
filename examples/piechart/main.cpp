#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "piechart.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_UseOpenGLES);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    qmlRegisterType<PieChart>("PieChart", 1, 0, "PieChart");
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}

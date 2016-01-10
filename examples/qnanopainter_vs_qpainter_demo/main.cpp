#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "src/demoqpitem.h"
#include "src/demoqnanoitem.h"

int main(int argc, char *argv[])
{
#ifdef  QT_OPENGL_ES_2
    QCoreApplication::setAttribute(Qt::AA_UseOpenGLES);
#endif
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    qmlRegisterType<DemoQPItem>("DemoQPItem", 1, 0, "DemoQPItem");
    qmlRegisterType<DemoQNanoItem>("DemoQNanoItem", 1, 0, "DemoQNanoItem");

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}

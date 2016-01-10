#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "helloitem.h"

int main(int argc, char *argv[])
{
#ifdef  QT_OPENGL_ES_2 
    QCoreApplication::setAttribute(Qt::AA_UseOpenGLES);
#endif
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    qmlRegisterType<HelloItem>("HelloItem", 1, 0, "HelloItem");
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}

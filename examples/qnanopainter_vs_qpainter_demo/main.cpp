#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "src/demoqpitem.h"
#include "src/demoqnanoitem.h"

int main(int argc, char *argv[])
{
#ifdef Q_OS_WIN
    QCoreApplication::setAttribute(Qt::AA_UseOpenGLES);
#endif

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    qmlRegisterType<DemoQPItem>("DemoQPItem", 1, 0, "DemoQPItem");
    qmlRegisterType<DemoQNanoItem>("DemoQNanoItem", 1, 0, "DemoQNanoItem");

    bool enableShapeDemo = false;
    QStringList version = QString(qVersion()).split(".");
    if (version.size() >= 2) {
        int major = version.at(0).toInt();
        int minor = version.at(1).toInt();
        if (major > 5 || (major == 5 && minor >= 10)) {
            qDebug() << "Running Qt >= 5.10, so enabling QML Shape example";
            enableShapeDemo = true;
        }
    }
    engine.rootContext()->setContextProperty("gEnableShapeDemo", enableShapeDemo);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}

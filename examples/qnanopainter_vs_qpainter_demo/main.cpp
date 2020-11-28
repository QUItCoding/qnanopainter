#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickWindow>
#include <QFontDatabase>
#include "src/demoqpitem.h"
#include "src/demoqnanoitem.h"

int g_customFontId = 0;

int main(int argc, char *argv[])
{

#ifdef Q_OS_WIN
    // Select between OpenGL and OpenGL ES (Angle)
    //QCoreApplication::setAttribute(Qt::AA_UseOpenGLES);
    QCoreApplication::setAttribute(Qt::AA_UseDesktopOpenGL);
#endif
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

#if (QT_VERSION >= 0x060000)
    QQuickWindow::setGraphicsApi(QSGRendererInterface::OpenGL);
#endif

    g_customFontId = QFontDatabase::addApplicationFont(":/qml/fonts/Roboto-Regular.ttf");

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

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "src/galleryitem.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_UseOpenGLES);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    qmlRegisterType<GalleryItem>("NanoVGQuick", 1, 0, "GalleryItem");

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}

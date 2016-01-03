#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "qnanopainter_features/src/galleryitem.h"
#include "mouse_events/src/eventitem.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_UseOpenGLES);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    qmlRegisterType<GalleryItem>("GalleryItem", 1, 0, "GalleryItem");
    qmlRegisterType<EventItem>("EventItem", 1, 0, "EventItem");

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}

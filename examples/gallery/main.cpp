#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "qnanopainter_features/src/galleryitem.h"
#include "mouse_events/src/eventitem.h"
#include "painting/src/paintingitem.h"

int main(int argc, char *argv[])
{
#ifdef Q_OS_WIN
    QCoreApplication::setAttribute(Qt::AA_UseOpenGLES);
#endif

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    qmlRegisterType<GalleryItem>("GalleryItem", 1, 0, "GalleryItem");
    qmlRegisterType<EventItem>("EventItem", 1, 0, "EventItem");
    qmlRegisterType<PaintingItem>("PaintingItem", 1, 0, "PaintingItem");

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}

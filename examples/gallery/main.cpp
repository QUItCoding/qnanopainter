#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickWindow>
#include "qnanopainter_features/src/galleryitem.h"
#include "mouse_events/src/eventitem.h"
#include "painting/src/paintingitem.h"

int main(int argc, char *argv[])
{
    // load res from libqnanopainter
    Q_INIT_RESOURCE(libqnanopainterdata);

    QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts);

#ifdef Q_OS_WIN
    // Select between OpenGL and OpenGL ES (Angle)
    //QCoreApplication::setAttribute(Qt::AA_UseOpenGLES);
    QCoreApplication::setAttribute(Qt::AA_UseDesktopOpenGL);
#endif

    QGuiApplication app(argc, argv);

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    QQuickWindow::setGraphicsApi(QSGRendererInterface::OpenGL);
#endif

    QQmlApplicationEngine engine;
    qmlRegisterType<GalleryItem>("GalleryItem", 1, 0, "GalleryItem");
    qmlRegisterType<EventItem>("EventItem", 1, 0, "EventItem");
    qmlRegisterType<PaintingItem>("PaintingItem", 1, 0, "PaintingItem");

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}

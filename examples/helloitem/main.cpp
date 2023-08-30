#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickWindow>
#include "helloitem.h"

int main(int argc, char *argv[])
{
    // load res from libqnanopainter
    Q_INIT_RESOURCE(libqnanopainterdata);

#ifdef Q_OS_WIN
    // Select between OpenGL and OpenGL ES (Angle)
    //QCoreApplication::setAttribute(Qt::AA_UseOpenGLES);
    QCoreApplication::setAttribute(Qt::AA_UseDesktopOpenGL);
#endif

    QGuiApplication app(argc, argv);

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#ifndef QNANO_USE_RHI
    QQuickWindow::setGraphicsApi(QSGRendererInterface::OpenGL);
#endif
#endif

    QQmlApplicationEngine engine;
    qmlRegisterType<HelloItem>("HelloItem", 1, 0, "HelloItem");
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}

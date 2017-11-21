#ifndef QNANOBACKENDFACTORY_H
#define QNANOBACKENDFACTORY_H

#ifndef QT_OPENGL_ES_2
#include "private/qnanobackendgl2.h"
#endif
#ifdef QT_OPENGL_3_2
#include "private/qnanobackendgl3.h"
#endif
#if defined(QT_OPENGL_ES_2) || defined(QT_OPENGL_ES_2_ANGLE)
#include "private/qnanobackendgles2.h"
#endif
#ifdef QT_OPENGL_ES_3
#include "private/qnanobackendgles3.h"
#endif

class QNanoBackendFactory
{
public:

    static QNanoBackend *createBackend(int major, int minor, bool isGLES) {

#if defined(Q_OS_ANDROID)
        // TODO: Currently GLES3 backend doesn't work on android as QT_OPENGL_ES_3
        // isn't defined there. So use GLES2 backend on Android for now.
        major = 2;
        minor = 0;
#endif

        //qDebug("Creating QNanoBackend for %s %d.%d", (isGLES ? "OpenGL ES" : "OpenGL"), major, minor);
        Q_UNUSED(minor);
        if (isGLES) {
            if (major >= 3) {
#ifdef QT_OPENGL_ES_3
                return new QNanoBackendGLES3();
#endif
            } else {
#if defined(QT_OPENGL_ES_2) || defined(QT_OPENGL_ES_2_ANGLE)
                return new QNanoBackendGLES2();
#endif
            }
        } else {
            if (major >= 3) {
#ifdef QT_OPENGL_3
                return new QNanoBackendGL3();
#endif
            } else {
#ifndef QT_OPENGL_ES_2
                return new QNanoBackendGL2();
#endif
            }
        }
        qWarning("Creating QNanoBackend for %s %d.%d failed!", (isGLES ? "OpenGL ES" : "OpenGL"), major, minor);
        return nullptr;
    }
};

#endif // QNANOBACKENDFACTORY_H

#include "qnanobackendgl2.h"

#ifdef Q_OS_WIN
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#include <QOpenGLVersionFunctionsFactory>
#endif
#include <QOpenGLFunctions_2_0>
static QOpenGLFunctions_2_0 *glf;
#define QNANO_GLFWRAPPER glf->
#else
#define QNANO_GLFWRAPPER
#endif

#define NANOVG_GL_IMPLEMENTATION 1
#include "nanovg/nanovg_gl_wrapped.h"

QNanoBackendGL2::QNanoBackendGL2()
{
#ifdef Q_OS_WIN
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    glf = QOpenGLVersionFunctionsFactory::get<QOpenGLFunctions_2_0>(QOpenGLContext::currentContext());
#else
    glf = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_2_0>();
#endif
#endif
}

const QString QNanoBackendGL2::backendName()
{
    return "OpenGL 2";
}

NVGcontext* QNanoBackendGL2::nvgCreate(int flags)
{
    return nvgCreateGL2(flags);
}

void QNanoBackendGL2::nvgDelete(NVGcontext* nvgCtx)
{
    nvgDeleteGL2(nvgCtx);
}

int QNanoBackendGL2::nvglCreateImageFromHandle(NVGcontext* ctx, GLuint textureId, int w, int h, int imageFlags)
{
    return nvglCreateImageFromHandleGL2(ctx, textureId, w, h, imageFlags);
}

NVGparams *QNanoBackendGL2::internalParams(NVGcontext* nvgCtx) {
    return nvgInternalParams(nvgCtx);
}

void QNanoBackendGL2::setFlag(NVGcontext* nvgCtx, int flag, bool enable) {
    QNANOBACKEND_SETFLAG(nvgCtx, flag, enable)
}

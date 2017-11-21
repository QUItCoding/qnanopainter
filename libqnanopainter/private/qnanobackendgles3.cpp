#include "qnanobackendgles3.h"

#ifdef QT_OPENGL_ES_3

#define NANOVG_GL_IMPLEMENTATION 1
#include "nanovg/nanovg_gl.h"

#endif

QNanoBackendGLES3::QNanoBackendGLES3()
{
}

const QString QNanoBackendGLES3::backendName()
{
    return "OpenGLES 3";
}

NVGcontext* QNanoBackendGLES3::nvgCreate(int flags)
{
#ifdef QT_OPENGL_ES_3
    return nvgCreateGLES3(flags);
#else
    Q_UNUSED(flags);
    return nullptr;
#endif
}

void QNanoBackendGLES3::nvgDelete(NVGcontext* nvgCtx)
{
#ifdef QT_OPENGL_ES_3
    nvgDeleteGLES3(nvgCtx);
#else
    Q_UNUSED(nvgCtx);
#endif
}

NVGparams *QNanoBackendGLES3::internalParams(NVGcontext* nvgCtx) {
    return nvgInternalParams(nvgCtx);
}

void QNanoBackendGLES3::setFlag(NVGcontext* nvgCtx, int flag, bool enable) {
#ifdef QT_OPENGL_ES_3
    setFlagT<GLNVGcontext>(nvgCtx, flag, enable);
#else
    Q_UNUSED(nvgCtx);
    Q_UNUSED(flag);
    Q_UNUSED(enable);
#endif
}

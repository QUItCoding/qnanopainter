#include "qnanobackendgles3.h"

#if !defined(QT_OPENGL_ES_3) && !defined(QT_OPENGL_ES_2_ANGLE)

// QT_OPENGL_ES_3 not defined, so include manually
// Can include either own headers or system headers
#include "../ext/GLES3/gl3.h"
//#include <GLES3/gl3.h>

#endif

#define NANOVG_GL_IMPLEMENTATION 1
#include "nanovg/nanovg_gl.h"

QNanoBackendGLES3::QNanoBackendGLES3()
{
}

const QString QNanoBackendGLES3::backendName()
{
    return "OpenGLES 3";
}

NVGcontext* QNanoBackendGLES3::nvgCreate(int flags)
{
    return nvgCreateGLES3(flags);
}

void QNanoBackendGLES3::nvgDelete(NVGcontext* nvgCtx)
{
    nvgDeleteGLES3(nvgCtx);
}

int QNanoBackendGLES3::nvglCreateImageFromHandle(NVGcontext* ctx, GLuint textureId, int w, int h, int imageFlags)
{
    return nvglCreateImageFromHandleGLES3(ctx, textureId, w, h, imageFlags);
}

NVGparams *QNanoBackendGLES3::internalParams(NVGcontext* nvgCtx) {
    return nvgInternalParams(nvgCtx);
}

void QNanoBackendGLES3::setFlag(NVGcontext* nvgCtx, int flag, bool enable) {
    QNANOBACKEND_SETFLAG(nvgCtx, flag, enable)
}

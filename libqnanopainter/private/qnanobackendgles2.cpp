#include "qnanobackendgles2.h"

#define NANOVG_GL_IMPLEMENTATION 1
#include "nanovg/nanovg_gl.h"

QNanoBackendGLES2::QNanoBackendGLES2()
{
}

const QString QNanoBackendGLES2::backendName()
{
    return "OpenGLES 2";
}

NVGcontext* QNanoBackendGLES2::nvgCreate(int flags)
{
    return nvgCreateGLES2(flags);
}

void QNanoBackendGLES2::nvgDelete(NVGcontext* nvgCtx)
{
    nvgDeleteGLES2(nvgCtx);
}

int QNanoBackendGLES2::nvglCreateImageFromHandle(NVGcontext* ctx, GLuint textureId, int w, int h, int imageFlags)
{
    return nvglCreateImageFromHandleGLES2(ctx, textureId, w, h, imageFlags);
}

NVGparams *QNanoBackendGLES2::internalParams(NVGcontext* nvgCtx) {
    return nvgInternalParams(nvgCtx);
}

void QNanoBackendGLES2::setFlag(NVGcontext* nvgCtx, int flag, bool enable) {
    QNANOBACKEND_SETFLAG(nvgCtx, flag, enable)
}

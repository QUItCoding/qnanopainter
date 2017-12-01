#include "qnanobackendgl2.h"

#define NANOVG_GL_IMPLEMENTATION 1
#include "nanovg/nanovg_gl.h"

QNanoBackendGL2::QNanoBackendGL2()
{
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

NVGparams *QNanoBackendGL2::internalParams(NVGcontext* nvgCtx) {
    return nvgInternalParams(nvgCtx);
}

void QNanoBackendGL2::setFlag(NVGcontext* nvgCtx, int flag, bool enable) {
    QNANOBACKEND_SETFLAG(nvgCtx, flag, enable)
}

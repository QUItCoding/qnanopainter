#include "qnanobackendgl3.h"

#define NANOVG_GL_IMPLEMENTATION 1
#include "nanovg/nanovg_gl.h"

QNanoBackendGL3::QNanoBackendGL3()
{
}

const QString QNanoBackendGL3::backendName()
{
    return "OpenGL 3";
}

NVGcontext* QNanoBackendGL3::nvgCreate(int flags)
{
    return nvgCreateGL3(flags);
}

void QNanoBackendGL3::nvgDelete(NVGcontext* nvgCtx)
{
    nvgDeleteGL3(nvgCtx);
}

NVGparams *QNanoBackendGL3::internalParams(NVGcontext* nvgCtx) {
    return nvgInternalParams(nvgCtx);
}

void QNanoBackendGL3::setFlag(NVGcontext* nvgCtx, int flag, bool enable) {
    setFlagT<GLNVGcontext>(nvgCtx, flag, enable);
}

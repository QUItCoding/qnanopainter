#include "qnanobackendrhi.h"

#include "nanovg/nanovg_rhi.h"

QNanoBackendRHI::QNanoBackendRHI()
{
}

const QString QNanoBackendRHI::backendName()
{
    return "RHI";
}

NVGcontext* QNanoBackendRHI::nvgCreate(int flags)
{
    m_vg.create(m_rhi, flags);
    return m_vg.ctx;
}

void QNanoBackendRHI::nvgDelete(NVGcontext* nvgCtx)
{
    Q_UNUSED(nvgCtx)
    m_vg.destroy();
}

int QNanoBackendRHI::nvglCreateImageFromHandle(NVGcontext* ctx, GLuint textureId, int w, int h, int imageFlags)
{
    // TODO: RHI backend doesn't have nvglCreateImageFromHandle
    return 0;
}

NVGparams *QNanoBackendRHI::internalParams(NVGcontext* nvgCtx) {
    return nvgInternalParams(nvgCtx);
}

void QNanoBackendRHI::setFlag(NVGcontext* nvgCtx, int flag, bool enable) {
    // TODO: Changing flags not implemented into RHI backend
    //QNANOBACKEND_SETFLAG(nvgCtx, flag, enable)
}

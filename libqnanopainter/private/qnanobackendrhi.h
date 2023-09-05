#ifndef QNANOBACKENDRHI_H
#define QNANOBACKENDRHI_H

#include "qnanobackend.h"
#include "nanovg/nanovg_rhi.h"

class QNanoBackendRHI : public QNanoBackend
{
public:
    QNanoBackendRHI();
    const QString backendName() Q_DECL_OVERRIDE;
    NVGcontext* nvgCreate(int flags) Q_DECL_OVERRIDE;
    void nvgDelete(NVGcontext* nvgCtx) Q_DECL_OVERRIDE;
    int nvglCreateImageFromHandle(NVGcontext* ctx, GLuint textureId, int w, int h, int imageFlags) Q_DECL_OVERRIDE;
    NVGparams *internalParams(NVGcontext* nvgCtx) Q_DECL_OVERRIDE;
    void setFlag(NVGcontext* nvgCtx, int flag, bool enable) Q_DECL_OVERRIDE;

    QRhi *m_rhi = nullptr;
    NanoVG m_vg;
};

#endif // QNANOBACKENDRHI_H

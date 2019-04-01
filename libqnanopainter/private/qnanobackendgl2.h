#ifndef QNANOBACKENDGL2_H
#define QNANOBACKENDGL2_H

#include "qnanobackend.h"

#define NANOVG_GL2 1
#include "nanovg/nanovg_gl.h"

class QNanoBackendGL2 : public QNanoBackend
{
public:
    QNanoBackendGL2();
    const QString backendName() Q_DECL_OVERRIDE;
    NVGcontext* nvgCreate(int flags) Q_DECL_OVERRIDE;
    void nvgDelete(NVGcontext* nvgCtx) Q_DECL_OVERRIDE;
    int nvglCreateImageFromHandle(NVGcontext* ctx, GLuint textureId, int w, int h, int imageFlags) Q_DECL_OVERRIDE;
    NVGparams *internalParams(NVGcontext* nvgCtx) Q_DECL_OVERRIDE;
    void setFlag(NVGcontext* nvgCtx, int flag, bool enable) Q_DECL_OVERRIDE;
};

#endif // QNANOBACKENDGL2_H

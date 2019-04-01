#ifndef QNANOBACKENDGLES3_H
#define QNANOBACKENDGLES3_H

#include "qnanobackend.h"

#define NANOVG_GLES3 1
#include "nanovg/nanovg_gl.h"

class QNanoBackendGLES3 : public QNanoBackend
{
public:
    QNanoBackendGLES3();
    const QString backendName() Q_DECL_OVERRIDE;
    NVGcontext* nvgCreate(int flags) Q_DECL_OVERRIDE;
    void nvgDelete(NVGcontext* nvgCtx) Q_DECL_OVERRIDE;
    int nvglCreateImageFromHandle(NVGcontext* ctx, GLuint textureId, int w, int h, int imageFlags) Q_DECL_OVERRIDE;
    NVGparams *internalParams(NVGcontext* nvgCtx) Q_DECL_OVERRIDE;
    void setFlag(NVGcontext* nvgCtx, int flag, bool enable) Q_DECL_OVERRIDE;
};

#endif // QNANOBACKENDGLES3_H

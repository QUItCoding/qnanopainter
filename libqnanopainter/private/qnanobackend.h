#ifndef QNANOBACKEND_H
#define QNANOBACKEND_H

#if defined(QNANO_QT_GL_INCLUDE)
// Let the Qt include OpenGL headers

#define GL_GLEXT_PROTOTYPES
#include <QtGui/qopengl.h>

#else
// Manually include OpenGL headers

#if defined(Q_OS_IOS)
#include <OpenGLES/ES2/gl.h>
#elif defined(Q_OS_ANDROID)
#include <GLES2/gl2.h>
#elif defined(Q_OS_OSX)
#include <OpenGL/gl.h>
#elif defined(Q_OS_LINUX)
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#else
#include <GLES2/gl2.h>
#endif

#endif //QNANO_QT_GL_INCLUDE

// Undefine all so only correct version will get included
#undef NANOVG_GL2
#undef NANOVG_GL3
#undef NANOVG_GLES2
#undef NANOVG_GLES3

#include <QString>

#include "nanovg/nanovg.h"

class QRhi;

class QNanoBackend
{
public:
    virtual ~QNanoBackend() {}
    virtual const QString backendName() = 0;
    virtual NVGcontext* nvgCreate(int flags) = 0;
    virtual void nvgDelete(NVGcontext* nvgCtx) = 0;
    virtual int nvglCreateImageFromHandle(NVGcontext* ctx, GLuint textureId, int w, int h, int imageFlags) = 0;
    virtual NVGparams *internalParams(NVGcontext* nvgCtx) = 0;
    virtual void setFlag(NVGcontext* nvgCtx, int flag, bool enable) = 0;

    // Common setFlag implementation
    // Using macro as GLNVGcontext & glnvg__renderCreate aren't known here
    #define QNANOBACKEND_SETFLAG(nvgCtx, flag, enable) {\
        NVGparams *params = nvgInternalParams(nvgCtx);\
        GLNVGcontext *gl = static_cast<GLNVGcontext*>(params->userPtr);\
        if (gl) {\
            if (enable) {\
                gl->flags |= flag;\
            } else {\
                gl->flags &= ~flag;\
            }\
            glnvg__renderCreate(gl);\
        }\
    }

};


#endif // QNANOBACKEND_H

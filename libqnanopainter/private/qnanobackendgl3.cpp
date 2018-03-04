#include "qnanobackendgl3.h"

#ifdef Q_OS_WIN
#include <QOpenGLFunctions_3_2_Core>
static QOpenGLFunctions_3_2_Core *glf;
#define QNANO_GLFWRAPPER glf->
#else
#define QNANO_GLFWRAPPER
#endif

#define NANOVG_GL_IMPLEMENTATION 1
#include "nanovg/nanovg_gl_wrapped.h"

QNanoBackendGL3::QNanoBackendGL3()
{
#ifdef Q_OS_WIN
    glf = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_2_Core>();
#endif
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
    QNANOBACKEND_SETFLAG(nvgCtx, flag, enable)
}

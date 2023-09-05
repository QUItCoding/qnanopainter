//
// Copyright (c) 2009-2013 Mikko Mononen memon@inside.org
//
// This software is provided 'as-is', without any express or implied
// warranty.  In no event will the authors be held liable for any damages
// arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.
//
#ifndef NANOVG_RHI_H
#define NANOVG_RHI_H

#include <qglobal.h>
#include "nanovg.h"

QT_BEGIN_NAMESPACE

class QRhi;
class QRhiCommandBuffer;
class QRhiRenderTarget;

enum NVGcreateFlags {
    // Flag indicating if geometry based anti-aliasing is used (may not be needed when using MSAA).
    NVG_ANTIALIAS 		= 1<<0,
    // Flag indicating if strokes should be drawn using stencil buffer. The rendering will be a little
    // slower, but path overlaps (i.e. self-intersecting or sharp turns) will be drawn just once.
    NVG_STENCIL_STROKES	= 1<<1,
    // TODO
    NVG_DEBUG 			= 1<<2,
    // Flag indicating if scissoring is used.
    NVG_SCISSORING 		= 1<<3,
};

struct NanoVG
{
    ~NanoVG();
    void create(QRhi *rhi, int flags = 0);
    void destroy();
    bool isValid() const { return ctx != nullptr; }

    // begin() and end() must be called outside a render pass.
    //
    // If devicePixelRatio is 0, rt->devicePixelRatio() is used. Specifying an
    // override is relevant when the target is a texture that is independent
    // from any on-screen window: rt's dpr is 1 then, but we may still want to
    // take an on-screen widget or window's dpr into account. (because we know
    // that the texture is eventually used in that window for something)
    //
    void begin(QRhiCommandBuffer *cb, QRhiRenderTarget *rt, float devicePixelRatio = 0.0f);
    void end();

    // then when recording the render pass, call render()
    void render();

    NVGcontext *ctx = nullptr;
};

// These are additional flags on top of NVGimageFlags.
enum NVGimageFlagsRhi {
    NVG_IMAGE_NODELETE			= 1<<16,	// Do not delete QRhiTexture.
};

QT_END_NAMESPACE

#endif /* NANOVG_RHI_H */

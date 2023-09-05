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

#include "nanovg_rhi.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include <QFile>
#include <QtGui/rhi/qrhi.h>

QT_BEGIN_NAMESPACE

enum RHINVGshaderType {
    NVG_SHADER_FILLGRAD,
    NVG_SHADER_FILLIMG,
    NVG_SHADER_SIMPLE,
    NVG_SHADER_IMG
};

struct RHINVGtexture {
    QRhiTexture *tex;
    int id;
    int width;
    int height;
    int type;
    int flags;
};

struct RHINVGblend
{
    QRhiGraphicsPipeline::BlendFactor srcRGB;
    QRhiGraphicsPipeline::BlendFactor dstRGB;
    QRhiGraphicsPipeline::BlendFactor srcAlpha;
    QRhiGraphicsPipeline::BlendFactor dstAlpha;
};

enum RHINVGcallType {
    RHINVG_NONE = 0,
    RHINVG_FILL,
    RHINVG_CONVEXFILL,
    RHINVG_STROKE,
    RHINVG_TRIANGLES,
};

struct RHINVGcall {
    int type;
    int image;
    int pathOffset;
    int pathCount;
    int indexOffset;
    int indexCount;
    int triangleOffset;
    int triangleCount;
    int fragmentUniformBufferOffset;
    int fragmentUniformBufferIndex;
    RHINVGblend blendFunc;
    QRhiShaderResourceBindings *srb[2];
    QRhiGraphicsPipeline *ps[4];
};

struct RHINVGpath {
    int fillOffset;
    int fillCount;
    int strokeOffset;
    int strokeCount;
};

struct RHINVGfragUniforms {
    float scissorMat[12]; // matrices are actually 3 vec4s
    float paintMat[12];
    struct NVGcolor innerCol;
    struct NVGcolor outerCol;
    float scissorExt[2];
    float scissorScale[2];
    float extent[2];
    float radius;
    float feather;
    float strokeMult;
    float strokeThr;
    int texType;
    int type;
};

struct RHINVGsamplerDesc
{
    QRhiSampler::Filter minFilter;
    QRhiSampler::Filter magFilter;
    QRhiSampler::Filter mipmap;
    QRhiSampler::AddressMode hTiling;
    QRhiSampler::AddressMode vTiling;
    QRhiSampler::AddressMode zTiling;
};

inline bool operator==(const RHINVGsamplerDesc &a, const RHINVGsamplerDesc &b) Q_DECL_NOTHROW
{
    return a.hTiling == b.hTiling && a.vTiling == b.vTiling && a.zTiling == b.zTiling
           && a.minFilter == b.minFilter && a.magFilter == b.magFilter
           && a.mipmap == b.mipmap;
}

struct RHINVGpipelineState
{
    bool edgeAA = false;
    bool stencilStrokes = false;
    bool scissoring = false;

    QRhiGraphicsPipeline::Topology topology = QRhiGraphicsPipeline::Triangles;

    QRhiGraphicsPipeline::CullMode cullMode = QRhiGraphicsPipeline::Back;

    bool depthTestEnable = false;
    bool depthWriteEnable = false;
    QRhiGraphicsPipeline::CompareOp depthFunc = QRhiGraphicsPipeline::LessOrEqual;

    bool stencilTestEnable = false;
    bool usesStencilRef = false;
    QRhiGraphicsPipeline::StencilOpState stencilFront;
    QRhiGraphicsPipeline::StencilOpState stencilBack;
    quint32 stencilReadMask = 0xFFFFFFFF;
    quint32 stencilWriteMask = 0xFFFFFFFF;

    bool blendEnable = true;
    QRhiGraphicsPipeline::TargetBlend targetBlend;

    int sampleCount = 1;
};

inline bool operator==(const RHINVGpipelineState &a, const RHINVGpipelineState &b) Q_DECL_NOTHROW
{
    return a.edgeAA == b.edgeAA
           && a.stencilStrokes == b.stencilStrokes
           && a.scissoring == b.scissoring
           && a.topology == b.topology
           && a.cullMode == b.cullMode
           && a.depthTestEnable == b.depthTestEnable
           && a.depthWriteEnable == b.depthWriteEnable
           && a.depthFunc == b.depthFunc
           && a.stencilTestEnable == b.stencilTestEnable
           && a.usesStencilRef == b.usesStencilRef
           && a.stencilFront.failOp == b.stencilFront.failOp
           && a.stencilFront.depthFailOp == b.stencilFront.depthFailOp
           && a.stencilFront.passOp == b.stencilFront.passOp
           && a.stencilFront.compareOp == b.stencilFront.compareOp
           && a.stencilBack.failOp == b.stencilBack.failOp
           && a.stencilBack.depthFailOp == b.stencilBack.depthFailOp
           && a.stencilBack.passOp == b.stencilBack.passOp
           && a.stencilBack.compareOp == b.stencilBack.compareOp
           && a.stencilReadMask == b.stencilReadMask
           && a.stencilWriteMask == b.stencilWriteMask
           && a.blendEnable == b.blendEnable
           // NB! not memcmp
           && a.targetBlend.colorWrite == b.targetBlend.colorWrite
           && a.targetBlend.srcColor == b.targetBlend.srcColor
           && a.targetBlend.dstColor == b.targetBlend.dstColor
           && a.targetBlend.opColor == b.targetBlend.opColor
           && a.targetBlend.srcAlpha == b.targetBlend.srcAlpha
           && a.targetBlend.dstAlpha == b.targetBlend.dstAlpha
           && a.targetBlend.opAlpha == b.targetBlend.opAlpha
           && a.sampleCount == b.sampleCount;
}

inline bool operator!=(const RHINVGpipelineState &a, const RHINVGpipelineState &b) Q_DECL_NOTHROW
{
    return !(a == b);
}

inline size_t qHash(const RHINVGpipelineState &s, size_t seed) Q_DECL_NOTHROW
{
    // do not bother with all fields
    return qHash(s.edgeAA, seed)
           ^ qHash(s.stencilStrokes)
           ^ qHash(s.scissoring)
           ^ qHash(s.sampleCount)
           ^ qHash(s.targetBlend.dstColor)
           ^ qHash(s.depthFunc)
           ^ qHash(s.cullMode)
           ^ qHashBits(&s.stencilFront, sizeof(QRhiGraphicsPipeline::StencilOpState))
           ^ (s.depthTestEnable << 1)
           ^ (s.depthWriteEnable << 2)
           ^ (s.stencilTestEnable << 3)
           ^ (s.blendEnable << 4)
           ^ (s.usesStencilRef << 5);
}

struct RHINVGpipelineStateKey
{
    RHINVGpipelineState state;
    QVector<quint32> renderTargetDescription;
    QVector<quint32> srbLayoutDescription;
    struct {
        size_t renderTargetDescriptionHash;
        size_t srbLayoutDescriptionHash;
    } extra;
    static RHINVGpipelineStateKey create(const RHINVGpipelineState &state,
                                         const QRhiRenderPassDescriptor *rpDesc,
                                         const QRhiShaderResourceBindings *srb)
    {
        const QVector<quint32> rtDesc = rpDesc->serializedFormat();
        const QVector<quint32> srbDesc = srb->serializedLayoutDescription();
        return { state, rtDesc, srbDesc, { qHash(rtDesc), qHash(srbDesc) } };
    }
};

inline bool operator==(const RHINVGpipelineStateKey &a, const RHINVGpipelineStateKey &b) Q_DECL_NOTHROW
{
    return a.state == b.state
           && a.renderTargetDescription == b.renderTargetDescription
           && a.srbLayoutDescription == b.srbLayoutDescription;
}

inline bool operator!=(const RHINVGpipelineStateKey &a, const RHINVGpipelineStateKey &b) Q_DECL_NOTHROW
{
    return !(a == b);
}

inline size_t qHash(const RHINVGpipelineStateKey &k, size_t seed) Q_DECL_NOTHROW
{
    return qHash(k.state, seed)
           ^ k.extra.renderTargetDescriptionHash
           ^ k.extra.srbLayoutDescriptionHash;
}

static QShader getShader(const QString &name)
{
    QFile f(name);
    return f.open(QIODevice::ReadOnly) ? QShader::fromSerialized(f.readAll()) : QShader();
}

struct RHINVGshaders
{
    RHINVGshaders() {
        vs = getShader(QLatin1String(":/qnanopainter/data/fill.vert.qsb"));
        fs = getShader(QLatin1String(":/qnanopainter/data/fill.frag.qsb"));
        fsSC = getShader(QLatin1String(":/qnanopainter/data/fill_sc.frag.qsb"));
        fsAA = getShader(QLatin1String(":/qnanopainter/data/fill_aa.frag.qsb"));
        fsAASS = getShader(QLatin1String(":/qnanopainter/data/fill_aa_ss.frag.qsb"));
        fsAASC = getShader(QLatin1String(":/qnanopainter/data/fill_aa_sc.frag.qsb"));
        fsAASSSC = getShader(QLatin1String(":/qnanopainter/data/fill_aa_ss_sc.frag.qsb"));

        if (!vs.isValid() || !fs.isValid() || !fsAA.isValid() || !fsSC.isValid() ||
            !fsAASS.isValid() || !fsAASC.isValid() || !fsAASSSC.isValid()) {
            qFatal("Failed to load shaders!");
        }

        vertexInputLayout.setBindings({
            { 4 * sizeof(float) }
        });
        vertexInputLayout.setAttributes({
            { 0, 0, QRhiVertexInputAttribute::Float2, 0 },
            { 0, 1, QRhiVertexInputAttribute::Float2, 2 * sizeof(float) }
        });
    }

    QShader vs;
    QShader fs;
    QShader fsAA; // EDGE_AA enabled
    QShader fsSC; // SCISSORING enabled
    QShader fsAASS; // EDGE_AA and STENCIL_STROKES enabled
    QShader fsAASC; // EDGE_AA and SCISSORING enabled
    QShader fsAASSSC; // EDGE_AA, STENCIL_STROKES and SCISSORING enabled
    QRhiVertexInputLayout vertexInputLayout;
};

Q_GLOBAL_STATIC(RHINVGshaders, rhinvg_shaders)

struct RHINVGcontext
{
    QRhi *rhi = nullptr;
    QRhiCommandBuffer *cb = nullptr;
    QRhiRenderTarget *rt = nullptr;
    float dpr = 1.0f;

    RHINVGtexture* textures = nullptr;
    float view[2] = {};
    int ntextures = 0;
    int ctextures = 0;
    int textureId = 0;
    int oneFragmentUniformBufferSize = 0;
    int flags = 0;

    // Per frame buffers
    RHINVGcall* calls = nullptr;
    int ccalls = 0;
    int ncalls = 0;
    RHINVGpath* paths = nullptr;
    int cpaths = 0;
    int npaths = 0;
    struct NVGvertex* verts = nullptr;
    int cverts = 0;
    int nverts = 0;
    uint32_t *indices = nullptr;
    int cindices = 0;
    int nindices = 0;
    unsigned char* uniforms = nullptr;
    int cuniforms = 0;
    int nuniforms = 0;

    int dummyTex = 0;

    RHINVGshaders shaders;
    QVector<std::pair<RHINVGsamplerDesc, QRhiSampler*>> samplers;
    QHash<RHINVGpipelineStateKey, QRhiGraphicsPipeline *> pipelines;
    QRhiBuffer *vertexBuffer = nullptr;
    QRhiBuffer *indexBuffer = nullptr;
    QRhiBuffer *vsUniformBuffer = nullptr;
    QRhiBuffer *fsUniformBuffer = nullptr;
    QRhiResourceUpdateBatch *resourceUpdates = nullptr;
    // The uniform buffers are the same for every draw call (the fragment
    // uniform buffer uses dynamic offsets), only the QRhiTexture and Sampler
    // can be different. The image flags (which defines the QRhiSampler) are
    // static once an image_id is created, so a simple image_id -> srb mapping
    // is possible.
    QHash<int, QRhiShaderResourceBindings *> srbs;
};

static QRhiGraphicsPipeline *pipeline(RHINVGcontext *rc,
                                      const RHINVGpipelineStateKey &key,
                                      QRhiRenderPassDescriptor *rpDesc,
                                      QRhiShaderResourceBindings *srb)
{
    auto it = rc->pipelines.constFind(key);
    if (it != rc->pipelines.constEnd())
        return it.value();

    QRhiGraphicsPipeline *ps = rc->rhi->newGraphicsPipeline();

    RHINVGshaders *shaders = rhinvg_shaders();
    QShader fs = shaders->fs;
    if (key.state.edgeAA) {
        if (key.state.stencilStrokes) {
            if (key.state.scissoring)
                fs = shaders->fsAASSSC;
            else
                fs = shaders->fsAASS;
        } else {
            if (key.state.scissoring)
                fs = shaders->fsAASC;
            else
                fs = shaders->fsAA;
        }
    } else if (key.state.scissoring) {
        fs = shaders->fsSC;
    }

    ps->setShaderStages({
                         { QRhiShaderStage::Vertex, shaders->vs },
                         { QRhiShaderStage::Fragment, fs }
    });
    ps->setVertexInputLayout(shaders->vertexInputLayout);
    ps->setShaderResourceBindings(srb);
    ps->setRenderPassDescriptor(rpDesc);

    QRhiGraphicsPipeline::Flags flags;
    if (key.state.usesStencilRef)
        flags |= QRhiGraphicsPipeline::UsesStencilRef;
    ps->setFlags(flags);

    ps->setTopology(key.state.topology);
    ps->setCullMode(key.state.cullMode);

    QRhiGraphicsPipeline::TargetBlend blend = key.state.targetBlend;
    blend.enable = key.state.blendEnable;
    const int colorAttachmentCount = 1;
    QVarLengthArray<QRhiGraphicsPipeline::TargetBlend, 8> targetBlends(colorAttachmentCount);
    for (int i = 0; i < colorAttachmentCount; ++i)
        targetBlends[i] = blend;
    ps->setTargetBlends(targetBlends.cbegin(), targetBlends.cend());

    ps->setSampleCount(key.state.sampleCount);

    ps->setDepthTest(key.state.depthTestEnable);
    ps->setDepthWrite(key.state.depthWriteEnable);
    ps->setDepthOp(key.state.depthFunc);

    ps->setStencilTest(key.state.stencilTestEnable);
    ps->setStencilFront(key.state.stencilFront);
    ps->setStencilBack(key.state.stencilBack);
    ps->setStencilReadMask(key.state.stencilReadMask);
    ps->setStencilWriteMask(key.state.stencilWriteMask);

    if (!ps->create()) {
        qWarning("Failed to build graphics pipeline state");
        delete ps;
        return nullptr;
    }

    rc->pipelines.insert(key, ps);
    return ps;
}

static QRhiSampler *sampler(RHINVGcontext *rc, const RHINVGsamplerDesc &samplerDescription)
{
    auto compareSampler = [samplerDescription](const std::pair<RHINVGsamplerDesc, QRhiSampler*> &info) {
        return info.first == samplerDescription;
    };
    const auto found = std::find_if(rc->samplers.cbegin(), rc->samplers.cend(), compareSampler);
    if (found != rc->samplers.cend())
        return found->second;

    QRhiSampler *newSampler = rc->rhi->newSampler(samplerDescription.magFilter,
                                                  samplerDescription.minFilter,
                                                  samplerDescription.mipmap,
                                                  samplerDescription.hTiling,
                                                  samplerDescription.vTiling,
                                                  samplerDescription.zTiling);
    if (!newSampler->create()) {
        qWarning("Failed to build image sampler");
        delete newSampler;
        return nullptr;
    }
    rc->samplers << std::make_pair(samplerDescription, newSampler);
    return newSampler;
}

static bool ensureBufferCapacity(QRhiBuffer **buf, quint32 size)
{
    if ((*buf)->size() < size) {
        (*buf)->setSize(size);
        if (!(*buf)->create()) {
            qWarning("Failed to recreate buffer with size %u", size);
            return false;
        }
    }
    return true;
}

static QRhiResourceUpdateBatch *resourceUpdateBatch(RHINVGcontext *rc)
{
    if (!rc->resourceUpdates)
        rc->resourceUpdates = rc->rhi->nextResourceUpdateBatch();
    return rc->resourceUpdates;
}

static void commitResourceUpdates(RHINVGcontext *rc)
{
    if (rc->resourceUpdates) {
        rc->cb->resourceUpdate(rc->resourceUpdates);
        rc->resourceUpdates = nullptr;
    }
}

static RHINVGtexture *findTexture(RHINVGcontext *rc, int id)
{
    for (int i = 0; i < rc->ntextures; i++) {
        if (rc->textures[i].id == id)
            return &rc->textures[i];
    }
    return nullptr;
}

static QRhiShaderResourceBindings *createSrb(RHINVGcontext *rc, int image)
{
    RHINVGtexture* tex = NULL;
    if (image != 0) {
        tex = findTexture(rc, image);
    }
    // If no image is set, use empty texture
    if (tex == NULL) {
        tex = findTexture(rc, rc->dummyTex);
    }

    if (tex && tex->tex) {
        RHINVGsamplerDesc samplerDesc;
        samplerDesc.minFilter = (tex->flags & NVG_IMAGE_NEAREST) ? QRhiSampler::Nearest : QRhiSampler::Linear;
        samplerDesc.magFilter = (tex->flags & NVG_IMAGE_NEAREST) ? QRhiSampler::Nearest : QRhiSampler::Linear;
        samplerDesc.mipmap = (tex->flags & NVG_IMAGE_GENERATE_MIPMAPS) ? ((tex->flags & NVG_IMAGE_NEAREST) ? QRhiSampler::Nearest : QRhiSampler::Linear) : QRhiSampler::None;
        samplerDesc.hTiling = (tex->flags & NVG_IMAGE_REPEATX) ? QRhiSampler::Repeat : QRhiSampler::ClampToEdge;
        samplerDesc.vTiling = (tex->flags & NVG_IMAGE_REPEATY) ? QRhiSampler::Repeat : QRhiSampler::ClampToEdge;
        samplerDesc.zTiling = QRhiSampler::Repeat;
        QRhiShaderResourceBindings *srb = rc->rhi->newShaderResourceBindings();
        srb->setBindings({
            QRhiShaderResourceBinding::uniformBuffer(0, QRhiShaderResourceBinding::VertexStage, rc->vsUniformBuffer),
            QRhiShaderResourceBinding::uniformBufferWithDynamicOffset(1, QRhiShaderResourceBinding::FragmentStage, rc->fsUniformBuffer, sizeof(RHINVGfragUniforms)),
            QRhiShaderResourceBinding::sampledTexture(2, QRhiShaderResourceBinding::FragmentStage, tex->tex, sampler(rc, samplerDesc))
        });
        if (!srb->create()) {
            qWarning("Failed to create resource bindings");
            delete srb;
            return nullptr;
        }
        return srb;
    }

    return nullptr;
}

static int renderCreateTexture(void* uptr, int type, int w, int h, int imageFlags, const unsigned char* data);

static int renderCreate(void* uptr)
{
    RHINVGcontext *rc = (RHINVGcontext*)uptr;

    // Some platforms does not allow to have unset textures.
    // Create empty one which is bound when there's no texture specified.
    rc->dummyTex = renderCreateTexture(rc, NVG_TEXTURE_ALPHA, 16, 16, 0, NULL);

    rc->vertexBuffer = rc->rhi->newBuffer(QRhiBuffer::Static, QRhiBuffer::VertexBuffer, 16384);
    if (!rc->vertexBuffer->create()) {
        qWarning("Failed to create vertex buffer");
        return 0;
    }
    rc->indexBuffer = rc->rhi->newBuffer(QRhiBuffer::Static, QRhiBuffer::IndexBuffer, 16384);
    if (!rc->indexBuffer->create()) {
        qWarning("Failed to create index buffer");
        return 0;
    }
    rc->vsUniformBuffer = rc->rhi->newBuffer(QRhiBuffer::Dynamic, QRhiBuffer::UniformBuffer, 64);
    if (!rc->vsUniformBuffer->create()) {
        qWarning("Failed to create uniform buffer 0");
        return 0;
    }
    rc->fsUniformBuffer = rc->rhi->newBuffer(QRhiBuffer::Dynamic, QRhiBuffer::UniformBuffer, 16384);
    if (!rc->fsUniformBuffer->create()) {
        qWarning("Failed to create uniform buffer 1");
        return 0;
    }

    rc->oneFragmentUniformBufferSize = rc->rhi->ubufAligned(sizeof(RHINVGfragUniforms));

    return 1;
}

static int renderCreateTexture(void* uptr, int type, int w, int h, int imageFlags, const unsigned char* data)
{
    RHINVGcontext *rc = (RHINVGcontext*)uptr;
    RHINVGtexture* tex = NULL;
    int i;

    QRhiTexture::Format format = QRhiTexture::RGBA8;
    quint32 byteSize = w * h * 4;
    if (type == NVG_TEXTURE_ALPHA) {
        format = QRhiTexture::R8; // this excludes supporting OpenGL ES 2.0 but perhaps that's fine
        byteSize = w * h;
    }

    QRhiTexture::Flags flags;
    if (imageFlags & NVG_IMAGE_GENERATE_MIPMAPS)
        flags |= QRhiTexture::MipMapped | QRhiTexture::UsedWithGenerateMips;

    QRhiTexture *t = rc->rhi->newTexture(format, QSize(w, h), 1, flags);
    if (!t->create()) {
        qWarning("Failed to create texture of size %dx%d", w, h);
        delete t;
        return 0;
    }

    for (i = 0; i < rc->ntextures; i++) {
        if (rc->textures[i].id == 0) {
            tex = &rc->textures[i];
            break;
        }
    }
    if (tex == NULL) {
        if (rc->ntextures+1 > rc->ctextures) {
            RHINVGtexture* textures;
            int ctextures = std::max(rc->ntextures+1, 4) +  rc->ctextures/2; // 1.5x Overallocate
            textures = (RHINVGtexture*)realloc(rc->textures, sizeof(RHINVGtexture)*ctextures);
            if (textures == NULL)
                return 0;
            rc->textures = textures;
            rc->ctextures = ctextures;
        }
        tex = &rc->textures[rc->ntextures++];
    }
    memset(tex, 0, sizeof(*tex));
    tex->id = ++rc->textureId;

    tex->width = w;
    tex->height = h;
    tex->type = type;
    tex->flags = imageFlags;
    tex->tex = t;

    QRhiResourceUpdateBatch *u = resourceUpdateBatch(rc);
    if (data) {
        QRhiTextureSubresourceUploadDescription image(data, byteSize);
        QRhiTextureUploadDescription desc({ 0, 0, image });
        u->uploadTexture(tex->tex, desc);
    }

    if (imageFlags & NVG_IMAGE_GENERATE_MIPMAPS)
        u->generateMips(tex->tex);

    return tex->id;
}

static int renderDeleteTexture(void* uptr, int image)
{
    RHINVGcontext *rc = (RHINVGcontext*)uptr;
    int i;
    for (i = 0; i < rc->ntextures; i++) {
        if (rc->textures[i].id == image) {
            if (rc->textures[i].tex && (rc->textures[i].flags & NVG_IMAGE_NODELETE) == 0)
                delete rc->textures[i].tex;
            memset(&rc->textures[i], 0, sizeof(rc->textures[i]));
            return 1;
        }
    }
    return 0;
}

static int renderUpdateTexture(void* uptr, int image, int x, int y, int w, int h, const unsigned char* data)
{
    RHINVGcontext *rc = (RHINVGcontext*)uptr;
    RHINVGtexture* tex = findTexture(rc, image);

    if (tex == NULL) return 0;

    QRhiResourceUpdateBatch *u = resourceUpdateBatch(rc);
    if (data) {
        quint32 bytesPerPixel = 1;
        if (tex->type != NVG_TEXTURE_ALPHA)
            bytesPerPixel *= 4;

        quint32 stride = tex->width * bytesPerPixel;
        quint32 byteSize = stride * tex->height;

        const quint32 startOffset = x * bytesPerPixel + y * stride;
        QRhiTextureSubresourceUploadDescription image(data + startOffset, byteSize - startOffset);
        image.setDataStride(stride);
        image.setDestinationTopLeft(QPoint(x, y));
        image.setSourceSize(QSize(w, h));

        QRhiTextureUploadDescription desc({ 0, 0, image });
        u->uploadTexture(tex->tex, desc);
    }

    return 1;
}

static int renderGetTextureSize(void* uptr, int image, int* w, int* h)
{
    RHINVGcontext *rc = (RHINVGcontext*)uptr;
    RHINVGtexture* tex = findTexture(rc, image);
    if (tex == NULL) return 0;
    *w = tex->width;
    *h = tex->height;
    return 1;
}

static void renderViewport(void* uptr, float width, float height, float devicePixelRatio)
{
    NVG_NOTUSED(devicePixelRatio);
    RHINVGcontext *rc = (RHINVGcontext*)uptr;
    rc->view[0] = width;
    rc->view[1] = height;
}

inline QRhiGraphicsPipeline::BlendFactor convertBlendFuncFactor(int factor)
{
    if (factor == NVG_ZERO)
        return QRhiGraphicsPipeline::Zero;
    if (factor == NVG_ONE)
        return QRhiGraphicsPipeline::One;
    if (factor == NVG_SRC_COLOR)
        return QRhiGraphicsPipeline::SrcColor;
    if (factor == NVG_ONE_MINUS_SRC_COLOR)
        return QRhiGraphicsPipeline::OneMinusSrcColor;
    if (factor == NVG_DST_COLOR)
        return QRhiGraphicsPipeline::DstColor;
    if (factor == NVG_ONE_MINUS_DST_COLOR)
        return QRhiGraphicsPipeline::OneMinusDstColor;
    if (factor == NVG_SRC_ALPHA)
        return QRhiGraphicsPipeline::SrcAlpha;
    if (factor == NVG_ONE_MINUS_SRC_ALPHA)
        return QRhiGraphicsPipeline::OneMinusSrcAlpha;
    if (factor == NVG_DST_ALPHA)
        return QRhiGraphicsPipeline::DstAlpha;
    if (factor == NVG_ONE_MINUS_DST_ALPHA)
        return QRhiGraphicsPipeline::OneMinusDstAlpha;
    if (factor == NVG_SRC_ALPHA_SATURATE)
        return QRhiGraphicsPipeline::SrcAlphaSaturate;
    return QRhiGraphicsPipeline::One;
}

inline RHINVGblend blendCompositeOperation(NVGcompositeOperationState op)
{
    RHINVGblend blend;
    blend.srcRGB = convertBlendFuncFactor(op.srcRGB);
    blend.dstRGB = convertBlendFuncFactor(op.dstRGB);
    blend.srcAlpha = convertBlendFuncFactor(op.srcAlpha);
    blend.dstAlpha = convertBlendFuncFactor(op.dstAlpha);
    return blend;
}

static int maxVertCount(const NVGpath* paths, int npaths, int *indexCount = nullptr)
{
    int i, count = 0;
    if (indexCount)
        *indexCount = 0;
    for (i = 0; i < npaths; i++) {
        if (paths[i].nfill > 2) {
            count += paths[i].nfill;
            if (indexCount)
                *indexCount += (paths[i].nfill - 2) * 3;
        }
        count += paths[i].nstroke;
    }
    return count;
}

static RHINVGcall* allocCall(RHINVGcontext *rc)
{
    RHINVGcall* ret = NULL;
    if (rc->ncalls+1 > rc->ccalls) {
        RHINVGcall* calls;
        int ccalls = std::max(rc->ncalls+1, 128) + rc->ccalls/2; // 1.5x Overallocate
        calls = (RHINVGcall*)realloc(rc->calls, sizeof(RHINVGcall) * ccalls);
        if (calls == NULL) return NULL;
        rc->calls = calls;
        rc->ccalls = ccalls;
    }
    ret = &rc->calls[rc->ncalls++];
    memset(ret, 0, sizeof(RHINVGcall));
    return ret;
}

static int allocPaths(RHINVGcontext *rc, int n)
{
    int ret = 0;
    if (rc->npaths+n > rc->cpaths) {
        RHINVGpath* paths;
        int cpaths = std::max(rc->npaths + n, 128) + rc->cpaths/2; // 1.5x Overallocate
        paths = (RHINVGpath*)realloc(rc->paths, sizeof(RHINVGpath) * cpaths);
        if (paths == NULL) return -1;
        rc->paths = paths;
        rc->cpaths = cpaths;
    }
    ret = rc->npaths;
    rc->npaths += n;
    return ret;
}

static int allocVerts(RHINVGcontext *rc, int n)
{
    int ret = 0;
    if (rc->nverts+n > rc->cverts) {
        NVGvertex* verts;
        int cverts = std::max(rc->nverts + n, 4096) + rc->cverts/2; // 1.5x Overallocate
        verts = (NVGvertex*)realloc(rc->verts, sizeof(NVGvertex) * cverts);
        if (verts == NULL) return -1;
        rc->verts = verts;
        rc->cverts = cverts;
    }
    ret = rc->nverts;
    rc->nverts += n;
    return ret;
}

static int allocIndices(RHINVGcontext *rc, int n)
{
    int ret = 0;
    if (rc->nindices+n > rc->cindices) {
        uint32_t *indices;
        int cindices = std::max(rc->nindices + n, 4096) + rc->cindices/2; // 1.5x Overallocate
        indices = (uint32_t*)realloc(rc->indices, sizeof(uint32_t) * cindices);
        if (indices == NULL) return -1;
        rc->indices = indices;
        rc->cindices = cindices;
    }
    ret = rc->nindices;
    rc->nindices += n;
    return ret;
}

static int allocFragUniforms(RHINVGcontext *rc, int n)
{
    int ret = 0, structSize = rc->oneFragmentUniformBufferSize;
    if (rc->nuniforms+n > rc->cuniforms) {
        unsigned char* uniforms;
        int cuniforms = std::max(rc->nuniforms+n, 128) + rc->cuniforms/2; // 1.5x Overallocate
        uniforms = (unsigned char*)realloc(rc->uniforms, structSize * cuniforms);
        if (uniforms == NULL) return -1;
        rc->uniforms = uniforms;
        rc->cuniforms = cuniforms;
    }
    ret = rc->nuniforms * structSize;
    rc->nuniforms += n;
    return ret;
}

inline RHINVGfragUniforms *fragUniformPtr(RHINVGcontext *rc, int i)
{
    return (RHINVGfragUniforms*)&rc->uniforms[i];
}

inline void vset(NVGvertex* vtx, float x, float y, float u, float v)
{
    vtx->x = x;
    vtx->y = y;
    vtx->u = u;
    vtx->v = v;
}

inline void xformToMat3x4(float* m3, float* t)
{
    m3[0] = t[0];
    m3[1] = t[1];
    m3[2] = 0.0f;
    m3[3] = 0.0f;
    m3[4] = t[2];
    m3[5] = t[3];
    m3[6] = 0.0f;
    m3[7] = 0.0f;
    m3[8] = t[4];
    m3[9] = t[5];
    m3[10] = 1.0f;
    m3[11] = 0.0f;
}

inline NVGcolor premulColor(NVGcolor c)
{
    c.r *= c.a;
    c.g *= c.a;
    c.b *= c.a;
    return c;
}

static int convertPaint(RHINVGcontext *rc, RHINVGfragUniforms* frag, NVGpaint* paint,
                        NVGscissor* scissor, float width, float fringe, float strokeThr)
{
    RHINVGtexture* tex = NULL;
    float invxform[6];

    memset(frag, 0, sizeof(*frag));

    frag->innerCol = premulColor(paint->innerColor);
    frag->outerCol = premulColor(paint->outerColor);

    if (rc->flags & NVG_SCISSORING) {
        if (scissor->extent[0] < -0.5f || scissor->extent[1] < -0.5f) {
            memset(frag->scissorMat, 0, sizeof(frag->scissorMat));
            frag->scissorExt[0] = 1.0f;
            frag->scissorExt[1] = 1.0f;
            frag->scissorScale[0] = 1.0f;
            frag->scissorScale[1] = 1.0f;
        } else {
            nvgTransformInverse(invxform, scissor->xform);
            xformToMat3x4(frag->scissorMat, invxform);
            frag->scissorExt[0] = scissor->extent[0];
            frag->scissorExt[1] = scissor->extent[1];
            frag->scissorScale[0] = sqrtf(scissor->xform[0]*scissor->xform[0] + scissor->xform[2]*scissor->xform[2]) / fringe;
            frag->scissorScale[1] = sqrtf(scissor->xform[1]*scissor->xform[1] + scissor->xform[3]*scissor->xform[3]) / fringe;
        }
    }

    memcpy(frag->extent, paint->extent, sizeof(frag->extent));
    frag->strokeMult = (width*0.5f + fringe*0.5f) / fringe;
    frag->strokeThr = strokeThr;

    if (paint->image != 0) {
        tex = findTexture(rc, paint->image);
        if (tex == NULL) return 0;
        if ((tex->flags & NVG_IMAGE_FLIPY) != 0) {
            float m1[6], m2[6];
            nvgTransformTranslate(m1, 0.0f, frag->extent[1] * 0.5f);
            nvgTransformMultiply(m1, paint->xform);
            nvgTransformScale(m2, 1.0f, -1.0f);
            nvgTransformMultiply(m2, m1);
            nvgTransformTranslate(m1, 0.0f, -frag->extent[1] * 0.5f);
            nvgTransformMultiply(m1, m2);
            nvgTransformInverse(invxform, m1);
        } else {
            nvgTransformInverse(invxform, paint->xform);
        }
        frag->type = NVG_SHADER_FILLIMG;

        if (tex->type == NVG_TEXTURE_RGBA)
            frag->texType = (tex->flags & NVG_IMAGE_PREMULTIPLIED) ? 0 : 1;
        else
            frag->texType = 2;
    } else {
        frag->type = NVG_SHADER_FILLGRAD;
        frag->radius = paint->radius;
        frag->feather = paint->feather;
        nvgTransformInverse(invxform, paint->xform);
    }

    xformToMat3x4(frag->paintMat, invxform);

    return 1;
}

static void renderFill(void* uptr, NVGpaint* paint, NVGcompositeOperationState compositeOperation, NVGscissor* scissor, float fringe,
                       const float* bounds, const NVGpath* paths, int npaths)
{
    RHINVGcontext *rc = (RHINVGcontext*)uptr;
    RHINVGcall* call = allocCall(rc);
    NVGvertex* quad;
    RHINVGfragUniforms* frag;
    int i, maxverts, offset, indexOffset, indexCount;
    uint32_t *indexPtr;

    if (call == NULL) return;

    call->type = RHINVG_FILL;
    call->triangleCount = 4;
    call->pathOffset = allocPaths(rc, npaths);
    if (call->pathOffset == -1) goto error;
    call->pathCount = npaths;
    call->image = paint->image;
    call->blendFunc = blendCompositeOperation(compositeOperation);

    if (npaths == 1 && paths[0].convex)
    {
        call->type = RHINVG_CONVEXFILL;
        call->triangleCount = 0;	// Bounding box fill quad not needed for convex fill
    }

    // Allocate vertices for all the paths.
    maxverts = maxVertCount(paths, npaths, &indexCount) + call->triangleCount;
    offset = allocVerts(rc, maxverts);
    if (offset == -1) goto error;

    indexOffset = allocIndices(rc, indexCount);
    if (indexOffset == -1) goto error;
    call->indexOffset = indexOffset;
    call->indexCount = indexCount;
    indexPtr = &rc->indices[indexOffset];

    for (i = 0; i < npaths; i++) {
        RHINVGpath* copy = &rc->paths[call->pathOffset + i];
        const NVGpath* path = &paths[i];
        memset(copy, 0, sizeof(RHINVGpath));
        if (path->nfill > 2) {
            copy->fillOffset = offset;
            copy->fillCount = path->nfill;
            memcpy(&rc->verts[offset], path->fill, sizeof(NVGvertex) * path->nfill);

            int baseVertexIndex = offset;
            for (int j = 2; j < path->nfill; j++) {
                *indexPtr++ = baseVertexIndex;
                *indexPtr++ = baseVertexIndex + j - 1;
                *indexPtr++ = baseVertexIndex + j;
            }

            offset += path->nfill;
        }
        if (path->nstroke > 0) {
            copy->strokeOffset = offset;
            copy->strokeCount = path->nstroke;
            memcpy(&rc->verts[offset], path->stroke, sizeof(NVGvertex) * path->nstroke);
            offset += path->nstroke;
        }
    }

    // Setup uniforms for draw calls
    if (call->type == RHINVG_FILL) {
        // Quad
        call->triangleOffset = offset;
        quad = &rc->verts[call->triangleOffset];
        vset(&quad[0], bounds[2], bounds[3], 0.5f, 1.0f);
        vset(&quad[1], bounds[2], bounds[1], 0.5f, 1.0f);
        vset(&quad[2], bounds[0], bounds[3], 0.5f, 1.0f);
        vset(&quad[3], bounds[0], bounds[1], 0.5f, 1.0f);

        call->fragmentUniformBufferOffset = allocFragUniforms(rc, 2);
        if (call->fragmentUniformBufferOffset == -1) goto error;
        // Simple shader for stencil
        frag = fragUniformPtr(rc, call->fragmentUniformBufferOffset);
        memset(frag, 0, sizeof(*frag));
        frag->strokeThr = -1.0f;
        frag->type = NVG_SHADER_SIMPLE;
        // Fill shader
        convertPaint(rc, fragUniformPtr(rc, call->fragmentUniformBufferOffset + rc->oneFragmentUniformBufferSize), paint, scissor, fringe, fringe, -1.0f);
    } else {
        call->fragmentUniformBufferOffset = allocFragUniforms(rc, 1);
        if (call->fragmentUniformBufferOffset == -1) goto error;
        // Fill shader
        convertPaint(rc, fragUniformPtr(rc, call->fragmentUniformBufferOffset), paint, scissor, fringe, fringe, -1.0f);
    }

    return;

error:
    // We get here if call alloc was ok, but something else is not.
    // Roll back the last call to prevent drawing it.
    if (rc->ncalls > 0) rc->ncalls--;
}

static void renderStroke(void* uptr, NVGpaint* paint, NVGcompositeOperationState compositeOperation, NVGscissor* scissor, float fringe,
                         float strokeWidth, const NVGpath* paths, int npaths)
{
    RHINVGcontext *rc = (RHINVGcontext*)uptr;
    RHINVGcall* call = allocCall(rc);
    int i, maxverts, offset;

    if (call == NULL) return;

    call->type = RHINVG_STROKE;
    call->pathOffset = allocPaths(rc, npaths);
    if (call->pathOffset == -1) goto error;
    call->pathCount = npaths;
    call->image = paint->image;
    call->blendFunc = blendCompositeOperation(compositeOperation);

    // Allocate vertices for all the paths.
    maxverts = maxVertCount(paths, npaths);
    offset = allocVerts(rc, maxverts);
    if (offset == -1) goto error;

    for (i = 0; i < npaths; i++) {
        RHINVGpath* copy = &rc->paths[call->pathOffset + i];
        const NVGpath* path = &paths[i];
        memset(copy, 0, sizeof(RHINVGpath));
        if (path->nstroke) {
            copy->strokeOffset = offset;
            copy->strokeCount = path->nstroke;
            memcpy(&rc->verts[offset], path->stroke, sizeof(NVGvertex) * path->nstroke);
            offset += path->nstroke;
        }
    }

    if (rc->flags & NVG_STENCIL_STROKES) {
        // Fill shader
        call->fragmentUniformBufferOffset = allocFragUniforms(rc, 2);
        if (call->fragmentUniformBufferOffset == -1) goto error;

        convertPaint(rc, fragUniformPtr(rc, call->fragmentUniformBufferOffset), paint, scissor, strokeWidth, fringe, -1.0f);
        convertPaint(rc, fragUniformPtr(rc, call->fragmentUniformBufferOffset + rc->oneFragmentUniformBufferSize), paint, scissor, strokeWidth, fringe, 1.0f - 0.5f/255.0f);

    } else {
        // Fill shader
        call->fragmentUniformBufferOffset = allocFragUniforms(rc, 1);
        if (call->fragmentUniformBufferOffset == -1) goto error;
        convertPaint(rc, fragUniformPtr(rc, call->fragmentUniformBufferOffset), paint, scissor, strokeWidth, fringe, -1.0f);
    }

    return;

error:
    // We get here if call alloc was ok, but something else is not.
    // Roll back the last call to prevent drawing it.
    if (rc->ncalls > 0) rc->ncalls--;
}

static void renderTriangles(void* uptr, NVGpaint* paint, NVGcompositeOperationState compositeOperation, NVGscissor* scissor,
                                   const NVGvertex* verts, int nverts, float fringe)
{
    RHINVGcontext *rc = (RHINVGcontext*)uptr;
    RHINVGcall* call = allocCall(rc);
    RHINVGfragUniforms* frag;

    if (call == NULL) return;

    call->type = RHINVG_TRIANGLES;
    call->image = paint->image;
    call->blendFunc = blendCompositeOperation(compositeOperation);

    // Allocate vertices for all the paths.
    call->triangleOffset = allocVerts(rc, nverts);
    if (call->triangleOffset == -1) goto error;
    call->triangleCount = nverts;

    memcpy(&rc->verts[call->triangleOffset], verts, sizeof(NVGvertex) * nverts);

    // Fill shader
    call->fragmentUniformBufferOffset = allocFragUniforms(rc, 1);
    if (call->fragmentUniformBufferOffset == -1) goto error;
    frag = fragUniformPtr(rc, call->fragmentUniformBufferOffset);
    convertPaint(rc, frag, paint, scissor, 1.0f, fringe, -1.0f);
    frag->type = NVG_SHADER_IMG;

    return;

error:
    // We get here if call alloc was ok, but something else is not.
    // Roll back the last call to prevent drawing it.
    if (rc->ncalls > 0) rc->ncalls--;
}

static void renderCancel(void* uptr)
{
    RHINVGcontext *rc = (RHINVGcontext*)uptr;
    rc->nverts = 0;
    rc->npaths = 0;
    rc->ncalls = 0;
    rc->nuniforms = 0;
}

static void renderEndPrepare(void* uptr)
{
    RHINVGcontext *rc = (RHINVGcontext*)uptr;

    if (rc->ncalls > 0) {
        ensureBufferCapacity(&rc->vsUniformBuffer, 2 * sizeof(float));
        ensureBufferCapacity(&rc->fsUniformBuffer, rc->nuniforms * rc->oneFragmentUniformBufferSize);
        ensureBufferCapacity(&rc->vertexBuffer, rc->nverts * sizeof(NVGvertex));
        ensureBufferCapacity(&rc->indexBuffer, rc->nindices * sizeof(uint32_t));

        QRhiResourceUpdateBatch *u = resourceUpdateBatch(rc);
        u->updateDynamicBuffer(rc->vsUniformBuffer, 0, 2 * sizeof(float), rc->view);
        qint32 ndcIsYDown = !rc->rhi->isYUpInNDC();
        u->updateDynamicBuffer(rc->vsUniformBuffer, 2 * sizeof(float), sizeof(qint32), &ndcIsYDown);
        u->updateDynamicBuffer(rc->fsUniformBuffer, 0, rc->nuniforms * rc->oneFragmentUniformBufferSize, rc->uniforms);
        u->uploadStaticBuffer(rc->vertexBuffer, 0, rc->nverts * sizeof(NVGvertex), rc->verts);
        if (rc->nindices)
            u->uploadStaticBuffer(rc->indexBuffer, 0, rc->nindices * sizeof(uint32_t), rc->indices);

        commitResourceUpdates(rc);

        const int dummyTexId = findTexture(rc, rc->dummyTex)->id;
        QRhiShaderResourceBindings *&srbWithDummyTexture(rc->srbs[dummyTexId]);
        if (!srbWithDummyTexture)
            srbWithDummyTexture = createSrb(rc, dummyTexId);

        QRhiShaderResourceBindings *srbForLayout = srbWithDummyTexture; // all of them are layout-compatible, could use any

        for (int i = 0; i < rc->ncalls; i++) {
            RHINVGcall *call = &rc->calls[i];
            QRhiRenderPassDescriptor *rpDesc = rc->rt->renderPassDescriptor();

            QRhiShaderResourceBindings *&srbWithCallTexture(rc->srbs[call->image]);
            if (!srbWithCallTexture)
                srbWithCallTexture = createSrb(rc, call->image);

            RHINVGpipelineState basePs;
            basePs.edgeAA = (rc->flags & NVG_ANTIALIAS) != 0;
            basePs.stencilStrokes = (rc->flags & NVG_STENCIL_STROKES) != 0;
            basePs.scissoring = (rc->flags & NVG_SCISSORING) != 0;
            basePs.targetBlend.srcColor = call->blendFunc.srcRGB;
            basePs.targetBlend.dstColor = call->blendFunc.dstRGB;
            basePs.targetBlend.srcAlpha = call->blendFunc.srcAlpha;
            basePs.targetBlend.dstAlpha = call->blendFunc.dstAlpha;
            basePs.sampleCount = rc->rt->sampleCount();

            if (call->type == RHINVG_FILL) {
                call->srb[0] = srbWithDummyTexture;
                call->srb[1] = srbWithCallTexture;

                // 1. Draw shapes
                RHINVGpipelineState ps = basePs;
                ps.stencilTestEnable = true;
                ps.stencilWriteMask = 0xFF;
                ps.stencilReadMask = 0xFF;
                ps.stencilFront = {
                    QRhiGraphicsPipeline::Keep,
                    QRhiGraphicsPipeline::Keep,
                    QRhiGraphicsPipeline::IncrementAndWrap,
                    QRhiGraphicsPipeline::Always
                };
                ps.stencilBack = {
                    QRhiGraphicsPipeline::Keep,
                    QRhiGraphicsPipeline::Keep,
                    QRhiGraphicsPipeline::DecrementAndWrap,
                    QRhiGraphicsPipeline::Always
                };
                ps.cullMode = QRhiGraphicsPipeline::None;
                ps.targetBlend.colorWrite = {};

                call->ps[0] = pipeline(rc, RHINVGpipelineStateKey::create(ps, rpDesc, srbForLayout), rpDesc, srbForLayout);

                // 2. Draw anti-aliased pixels
                ps.cullMode = QRhiGraphicsPipeline::Back;
                ps.targetBlend.colorWrite = QRhiGraphicsPipeline::ColorMask(0xF);
                ps.stencilFront = {
                    QRhiGraphicsPipeline::Keep,
                    QRhiGraphicsPipeline::Keep,
                    QRhiGraphicsPipeline::Keep,
                    QRhiGraphicsPipeline::Equal
                };
                ps.stencilBack = ps.stencilFront;
                ps.topology = QRhiGraphicsPipeline::TriangleStrip;

                call->ps[1] = pipeline(rc, RHINVGpipelineStateKey::create(ps, rpDesc, srbForLayout), rpDesc, srbForLayout);

                // 3. Draw fill
                ps.stencilFront = {
                    QRhiGraphicsPipeline::StencilZero,
                    QRhiGraphicsPipeline::StencilZero,
                    QRhiGraphicsPipeline::StencilZero,
                    QRhiGraphicsPipeline::NotEqual
                };
                ps.stencilBack = ps.stencilFront;

                call->ps[2] = pipeline(rc, RHINVGpipelineStateKey::create(ps, rpDesc, srbForLayout), rpDesc, srbForLayout);
            } else if (call->type == RHINVG_CONVEXFILL) {
                call->srb[0] = srbWithCallTexture;

                // 1. Draw fill
                call->ps[0] = pipeline(rc, RHINVGpipelineStateKey::create(basePs, rpDesc, srbForLayout), rpDesc, srbForLayout);

                // 2. Draw fringes
                RHINVGpipelineState ps = basePs;
                ps.topology = QRhiGraphicsPipeline::TriangleStrip;

                call->ps[1] = pipeline(rc, RHINVGpipelineStateKey::create(ps, rpDesc, srbForLayout), rpDesc, srbForLayout);
            } else if (call->type == RHINVG_STROKE) {
                call->srb[0] = srbWithCallTexture;

                // 1. Draw Strokes (no stencil)
                RHINVGpipelineState ps = basePs;
                ps.topology = QRhiGraphicsPipeline::TriangleStrip;

                call->ps[0] = pipeline(rc, RHINVGpipelineStateKey::create(ps, rpDesc, srbForLayout), rpDesc, srbForLayout);

                // 2. Fill the stroke base without overlap
                ps.stencilTestEnable = true;
                ps.stencilWriteMask = 0xFF;
                ps.stencilReadMask = 0xFF;
                ps.stencilFront = {
                    QRhiGraphicsPipeline::Keep,
                    QRhiGraphicsPipeline::Keep,
                    QRhiGraphicsPipeline::IncrementAndClamp,
                    QRhiGraphicsPipeline::Equal
                };
                ps.stencilBack = ps.stencilFront;

                call->ps[1] = pipeline(rc, RHINVGpipelineStateKey::create(ps, rpDesc, srbForLayout), rpDesc, srbForLayout);

                // 3. Draw anti-aliased pixels.
                ps.stencilFront = {
                    QRhiGraphicsPipeline::Keep,
                    QRhiGraphicsPipeline::Keep,
                    QRhiGraphicsPipeline::Keep,
                    QRhiGraphicsPipeline::Equal
                };
                ps.stencilBack = ps.stencilFront;

                call->ps[2] = pipeline(rc, RHINVGpipelineStateKey::create(ps, rpDesc, srbForLayout), rpDesc, srbForLayout);

                // 4. Clear stencil buffer
                ps.targetBlend.colorWrite = {};
                ps.stencilFront = {
                    QRhiGraphicsPipeline::StencilZero,
                    QRhiGraphicsPipeline::StencilZero,
                    QRhiGraphicsPipeline::StencilZero,
                    QRhiGraphicsPipeline::Always
                };
                ps.stencilBack = ps.stencilFront;

                call->ps[3] = pipeline(rc, RHINVGpipelineStateKey::create(ps, rpDesc, srbForLayout), rpDesc, srbForLayout);
            } else if (call->type == RHINVG_TRIANGLES) {
                call->srb[0] = srbWithCallTexture;

                // 1.
                call->ps[0] = pipeline(rc, RHINVGpipelineStateKey::create(basePs, rpDesc, srbForLayout), rpDesc, srbForLayout);
            }
        }
    }

    //qDebug() << rc->pipelines.count() << rc->srbs.count();
}



// ********** render phase stuff (that is called only after the command buffer has started to record a render pass)

inline void bindPipeline(RHINVGcontext *rc, RHINVGcall *call,
                         int pipelineIndex, int srbIndex,
                         const QRhiCommandBuffer::DynamicOffset &dynamicOffset,
                         bool indexedDraw,
                         bool *needsViewport)
{
    rc->cb->setGraphicsPipeline(call->ps[pipelineIndex]);
    rc->cb->setShaderResources(call->srb[srbIndex], 1, &dynamicOffset);
    if (*needsViewport) {
        *needsViewport = false;
        const QSize size = rc->rt->pixelSize();
        rc->cb->setViewport({ 0.0f, 0.0f, float(size.width()), float(size.height()) });
    }
    QRhiCommandBuffer::VertexInput vbufBinding(rc->vertexBuffer, 0);
    if (indexedDraw)
        rc->cb->setVertexInput(0, 1, &vbufBinding, rc->indexBuffer, 0, QRhiCommandBuffer::IndexUInt32);
    else
        rc->cb->setVertexInput(0, 1, &vbufBinding);
}

static void renderpass_render(void* uptr)
{
    RHINVGcontext *rc = (RHINVGcontext*)uptr;
    bool needsViewport = true;

    for (int i = 0; i < rc->ncalls; i++) {
        RHINVGcall *call = &rc->calls[i];
        RHINVGpath *paths = call->pathCount ? &rc->paths[call->pathOffset] : nullptr;
        int npaths = call->pathCount;
        QRhiCommandBuffer::DynamicOffset dynamicOffsetForCall(1, call->fragmentUniformBufferOffset);
        QRhiCommandBuffer::DynamicOffset dynamicOffsetForCallPlusOne(1, call->fragmentUniformBufferOffset + rc->oneFragmentUniformBufferSize);

        if (call->type == RHINVG_FILL) {
            // 1. Draw shapes
            if (call->indexCount) {
                bindPipeline(rc, call, 0, 0, dynamicOffsetForCall, true, &needsViewport);
                rc->cb->drawIndexed(call->indexCount, 1, call->indexOffset);
            }

            // 2. Draw anti-aliased pixels
            if (rc->flags & NVG_ANTIALIAS) {
                bindPipeline(rc, call, 1, 1, dynamicOffsetForCallPlusOne, false, &needsViewport);
                // Draw fringes
                for (int i = 0; i < npaths; i++)
                    rc->cb->draw(paths[i].strokeCount, 1, paths[i].strokeOffset);
            }

            // 3. Draw fill
            bindPipeline(rc, call, 2, 1, dynamicOffsetForCallPlusOne, false, &needsViewport);
            rc->cb->draw(call->triangleCount, 1, call->triangleOffset);
        } else if (call->type == RHINVG_CONVEXFILL) {
            // 1. Draw fill
            if (call->indexCount) {
                bindPipeline(rc, call, 0, 0, dynamicOffsetForCall, true, &needsViewport);
                rc->cb->drawIndexed(call->indexCount, 1, call->indexOffset);
            }

            // 2. Draw fringes
            bindPipeline(rc, call, 1, 0, dynamicOffsetForCall, false, &needsViewport);
            for (int i = 0; i < npaths; i++) {
                if (paths[i].strokeCount > 0) {
                    rc->cb->draw(paths[i].strokeCount, 1, paths[i].strokeOffset);
                }
            }
        } else if (call->type == RHINVG_STROKE) {
            if (!(rc->flags & NVG_STENCIL_STROKES)) {
                // 1. Draw Strokes
                bindPipeline(rc, call, 0, 0, dynamicOffsetForCall, false, &needsViewport);
                for (int i = 0; i < npaths; i++)
                    rc->cb->draw(paths[i].strokeCount, 1, paths[i].strokeOffset);
            } else {
                // 2. Fill the stroke base without overlap
                bindPipeline(rc, call, 1, 0, dynamicOffsetForCallPlusOne, false, &needsViewport);
                for (int i = 0; i < npaths; i++)
                    rc->cb->draw(paths[i].strokeCount, 1, paths[i].strokeOffset);

                // 3. Draw anti-aliased pixels.
                bindPipeline(rc, call, 2, 0, dynamicOffsetForCall, false, &needsViewport);
                for (int i = 0; i < npaths; i++) {
                    if (paths[i].strokeCount > 0) {
                        rc->cb->draw(paths[i].strokeCount, 1, paths[i].strokeOffset);
                    }
                }

                //  4. Clear stencil buffer.
                bindPipeline(rc, call, 3, 0, dynamicOffsetForCall, false, &needsViewport);
                for (int i = 0; i < npaths; i++) {
                    if (paths[i].strokeCount > 0) {
                        rc->cb->draw(paths[i].strokeCount, 1, paths[i].strokeOffset);
                    }
                }
            }
        } else if (call->type == RHINVG_TRIANGLES) {
            bindPipeline(rc, call, 0, 0, dynamicOffsetForCall, false, &needsViewport);
            rc->cb->draw(call->triangleCount, 1, call->triangleOffset);
        }
    }

    // Reset calls
    rc->ncalls = 0;
    rc->npaths = 0;
    rc->nverts = 0;
    rc->nindices = 0;
    rc->nuniforms = 0;
}

// ********** end of render phase stuff



static void renderDelete(void* uptr)
{
    RHINVGcontext *rc = (RHINVGcontext*)uptr;
    int i;
    if (rc == NULL) return;

    for (i = 0; i < rc->ntextures; i++) {
        if (rc->textures[i].tex && (rc->textures[i].flags & NVG_IMAGE_NODELETE) == 0)
            delete rc->textures[i].tex;
    }
    free(rc->textures);

    for (const auto &samplerInfo : std::as_const(rc->samplers))
        delete samplerInfo.second;

    qDeleteAll(rc->pipelines);

    qDeleteAll(rc->srbs);

    delete rc->vertexBuffer;
    delete rc->indexBuffer;
    delete rc->vsUniformBuffer;
    delete rc->fsUniformBuffer;

    free(rc->paths);
    free(rc->verts);
    free(rc->indices);
    free(rc->uniforms);
    free(rc->calls);

    delete rc;
}

NVGcontext* nvgCreateRhi(QRhi *rhi, int flags)
{
    NVGparams params;
    NVGcontext* ctx = NULL;
    RHINVGcontext *rc = new RHINVGcontext;
    if (rc == NULL) goto error;

    memset(&params, 0, sizeof(params));
    params.renderCreate = renderCreate;
    params.renderCreateTexture = renderCreateTexture;
    params.renderDeleteTexture = renderDeleteTexture;
    params.renderUpdateTexture = renderUpdateTexture;
    params.renderGetTextureSize = renderGetTextureSize;
    params.renderViewport = renderViewport;
    params.renderCancel = renderCancel;
    params.renderEndPrepare = renderEndPrepare;
    params.renderFill = renderFill;
    params.renderStroke = renderStroke;
    params.renderTriangles = renderTriangles;
    params.renderDelete = renderDelete;
    params.userPtr = rc;
    params.edgeAntiAlias = flags & NVG_ANTIALIAS ? 1 : 0;

    rc->rhi = rhi;
    rc->flags = flags;

    ctx = nvgCreateInternal(&params);
    if (ctx == NULL) goto error;

    return ctx;

error:
    // 'gl' is freed by nvgDeleteInternal.
    if (ctx != NULL) nvgDeleteInternal(ctx);
    return NULL;
}

NanoVG::~NanoVG()
{
    destroy();
}

void NanoVG::create(QRhi *rhi, int flags)
{
    if (ctx)
        destroy();

    ctx = nvgCreateRhi(rhi, flags);
}

void NanoVG::destroy()
{
    if (ctx) {
        nvgDeleteInternal(ctx);
        ctx = nullptr;
    }
}

void NanoVG::begin(QRhiCommandBuffer *cb, QRhiRenderTarget *rt, float devicePixelRatio)
{
    RHINVGcontext *rc = (RHINVGcontext*) nvgInternalParams(ctx)->userPtr;
    rc->cb = cb;
    rc->rt = rt;
    rc->dpr = qFuzzyIsNull(devicePixelRatio) ? rt->devicePixelRatio() : devicePixelRatio;
    const QSize outputPixelSize = rt->pixelSize();
    const QSize outputLogicalSize = outputPixelSize / rc->dpr;
    nvgBeginFrame(ctx, outputLogicalSize.width(), outputLogicalSize.height(), rc->dpr);
}

void NanoVG::end()
{
    nvgEndFrame(ctx);
}

void NanoVG::render()
{
    renderpass_render(nvgInternalParams(ctx)->userPtr);
}

QT_END_NAMESPACE

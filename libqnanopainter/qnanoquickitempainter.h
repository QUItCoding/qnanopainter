/**********************************************************
** Copyright (c) 2015 QUIt Coding <info@quitcoding.com>
**
** This software is provided 'as-is', without any express or implied
** warranty.  In no event will the authors be held liable for any damages
** arising from the use of this software.
**
** Permission is granted to anyone to use this software for any purpose,
** including commercial applications, and to alter it and redistribute it
** freely, subject to the following restrictions:
**
** 1. The origin of this software must not be misrepresented; you must not
**    claim that you wrote the original software. If you use this software
**    in a product, an acknowledgment in the product documentation would be
**    appreciated but is not required.
** 2. Altered source versions must be plainly marked as such, and must not be
**    misrepresented as being the original software.
** 3. This notice may not be removed or altered from any source distribution.
**
**********************************************************/

#ifndef QNANOQUICKITEMPAINTER_H
#define QNANOQUICKITEMPAINTER_H

#include <QtGlobal>
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#include <QtOpenGL>          //for Qt>=6.0.0 opengl compatibility module needed (QT += opengl)
#else                        //for Qt5 -- original code.
#include <QtGui/QOpenGLFunctions>
#endif

#ifdef QNANO_USE_RHI
#include <QQuickRhiItem>
#include <QtGui/rhi/qrhi.h>
#include "nanovg/nanovg_rhi.h"
#else
#include <QtQuick/QQuickFramebufferObject>
#endif

#include <QColor>
#include <QElapsedTimer>
#ifdef QNANO_USE_RENDERNODE
#include <QSGRenderNode>
#endif
#include "qnanopainter.h"
#include "qnanoquickitem.h"
#ifdef QNANO_DEBUG_RENDER
#include "private/qnanodebug.h"
#endif

class QQuickWindow;

#ifdef QNANO_USE_RHI
class QNanoQuickItemPainter : public QQuickRhiItemRenderer
#elif QNANO_USE_RENDERNODE
class QNanoQuickItemPainter : public QSGRenderNode, protected QOpenGLFunctions
#else
class QNanoQuickItemPainter : public QQuickFramebufferObject::Renderer, protected QOpenGLFunctions
#endif
{

    struct ItemData {
        float x = 0.0f;
        float y = 0.0f;
        float width = 0.0f;
        float height = 0.0f;
        float devicePixelRatio = 1.0f;
        float rotation = 0.0f;
        float scale = 1.0f;
        float opacity = 1.0f;
        QQuickItem::TransformOrigin transformOrigin = QQuickItem::Center;
        bool clip = false;
    };

public:
    explicit QNanoQuickItemPainter();
    virtual ~QNanoQuickItemPainter() Q_DECL_OVERRIDE;

    QColor fillColor() const;
    inline QNanoPainter *painter() const {
        return m_painter;
    }
    inline float width() const {
        return m_itemData.width;
    }
    inline float height() const {
        return m_itemData.height;
    }
    inline const ItemData itemData() const {
        return m_itemData;
    }

protected:

#ifdef QNANO_USE_RENDERNODE
    StateFlags changedStates() const Q_DECL_OVERRIDE
    {
         return BlendState;
    }
#endif

    virtual void paint(QNanoPainter *painter);
    virtual void synchronize(QNanoQuickItem *item);
    virtual void sizeChanged(float width, float height);

private:

    friend class QNanoQuickItem;

    // Re-implemented from QQuickFramebufferObject::Renderer

#ifdef QNANO_USE_RHI
    virtual void initialize(QRhiCommandBuffer *cb) Q_DECL_OVERRIDE;
#elif !QNANO_USE_RENDERNODE
    // Creates initial FBO.
    // Gets also called whenever item size changes if textureFollowsItemSize = true
    QOpenGLFramebufferObject *createFramebufferObject(const QSize &size) Q_DECL_OVERRIDE;
#endif

#ifdef QNANO_USE_RHI
    void render(QRhiCommandBuffer *cb) Q_DECL_OVERRIDE;
#elif QNANO_USE_RENDERNODE
    // Gets called when FBO should be rendered into.
    void render(const RenderState *) Q_DECL_OVERRIDE;
#else
    void render() Q_DECL_OVERRIDE;
#endif

#ifdef QNANO_USE_RENDERNODE
    void synchronizePainter(QNanoQuickItem * item);
#elif QNANO_USE_RHI
    void synchronize(QQuickRhiItem *item) Q_DECL_OVERRIDE;
#else
    // Gets called as a result of QQuickFramebufferObject::update().
    void synchronize(QQuickFramebufferObject * item) Q_DECL_OVERRIDE;
#endif

    // These are internal
    void initialize();
#ifdef QNANO_USE_RHI
    void prepaint(QRhiCommandBuffer *cb);
#else
    void prepaint();
#endif
    void postpaint();
    void setViewSize(int width, int height);
    QPointF itemTransformOrigin() const;

    QQuickWindow *m_window = nullptr;

    QNanoPainter *m_painter = nullptr;
    QColor m_fillColor = {0, 0, 0, 0};
    int m_viewWidth = 0;
    int m_viewHeight = 0;
    int m_textureWidth = -1;
    int m_textureHeight = -1;
    ItemData m_itemData;
    bool m_antialiasing = true;
    bool m_highQualityRendering = false;
    QNanoQuickItem::PixelAlign m_pixelAlign = QNanoQuickItem::PixelAlignNone;
    QNanoQuickItem::PixelAlign m_pixelAlignText = QNanoQuickItem::PixelAlignNone;
    bool m_setupDone = false;
    NVGdrawDebug m_drawDebug;
#ifdef QNANO_DEBUG_RENDER
    QNanoDebug m_debug;
#endif
#ifdef QNANO_USE_RHI
    NanoVG m_vg;
    QRhi *m_rhi = nullptr;
    QRhiRenderTarget *m_rt = nullptr;
    bool m_initialized = false;
#endif
};

#endif // QNANOQUICKITEMPAINTER_H

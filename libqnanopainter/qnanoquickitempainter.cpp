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

#include "qnanoquickitempainter.h"
#include "qnanopainter.h"

#include <QDebug>
#include <QQuickWindow>
#include <QOpenGLFramebufferObjectFormat>
#include <QOpenGLFunctions>
#include <QtMath>

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#include <QQuickOpenGLUtils> // for QQuickOpenGLUtils::resetOpenGLState()
#endif

/*!
    \class QNanoQuickItemPainter
    \brief The QNanoQuickItemPainter handles all painting of a QNanoQuickItem.
    \inmodule QNanoPainter

    TODO: Write more documentation here.
*/


/*!
    Constructs a QNanoQuickItemPainter.
*/

QNanoQuickItemPainter::QNanoQuickItemPainter()
    : m_painter(QNanoPainter::getInstance())
{
#ifndef QNANO_USE_RHI
    // Initialize QOpenGLFunctions for the context
    initializeOpenGLFunctions();
#endif
}

/*!
    Destroys the QNanoQuickItemPainter.
*/

QNanoQuickItemPainter::~QNanoQuickItemPainter()
{
}

/*!
    \fn void QNanoQuickItemPainter::paint(QNanoPainter *painter)

    Reimplement this method to paint using \a painter.

    This will get called after the item has been filled with fillColor().

    Paint is called from renderer thread, to access item data do it in
    synchronize().

    \sa synchronize()
*/

/*!
    \fn QNanoPainter *QNanoQuickItemPainter::painter() const

    Returns the painter attached to this painter item.
*/

/*!
    \fn float QNanoQuickItemPainter::width() const

    Returns the width of this painter item.
*/

/*!
    \fn float QNanoQuickItemPainter::height() const

    Returns the height of this painter item.
*/

/*!
    \fn void QNanoQuickItemPainter::synchronize(QNanoQuickItem *item)

    Reimplement this method to synchronize data between \a item and
    item painter instances. This will be called before paint() each
    time item needs to be repainted.

    This method is the only place where it is safe for the painter and the
    item to read and write each others variables.

    Usually you should static_cast \a item to your real item type, and then
    exchange the data.
*/

void QNanoQuickItemPainter::synchronize(QNanoQuickItem *item)
{
    Q_UNUSED(item);
}

/*!
    \fn void QNanoQuickItemPainter::sizeChanged(float width, float height)

    Reimplement this method to react to item size changes. New size of
    the item is \a width and \a height.

    When item textureFollowsItemSize property is true (default) paint() gets
    called whenever item size changes so often reimplementing this method is
    not needed.

    This will be called before first call to paint() and can be used to
    e.g. load images based on size, prepare gradients etc.
*/

void QNanoQuickItemPainter::sizeChanged(float width, float height)
{
    Q_UNUSED(width);
    Q_UNUSED(height);
}

/*!
    \fn QColor QNanoQuickItemPainter::fillColor() const

    Return the current fill color of the item. This can be set
    by the parent QNanoQuickItem.

*/

QColor QNanoQuickItemPainter::fillColor() const
{
    return m_fillColor;
}

/*!
   \internal
*/

#if !defined(QNANO_USE_RENDERNODE) && !defined(QNANO_USE_RHI)
QOpenGLFramebufferObject *QNanoQuickItemPainter::createFramebufferObject(const QSize &size)
{
    QOpenGLFramebufferObjectFormat format;
    format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
    QSize fboSize(size);
    if (m_textureWidth > -1) fboSize.setWidth(static_cast<int>(m_textureWidth*m_itemData.devicePixelRatio));
    if (m_textureHeight > -1) fboSize.setHeight(static_cast<int>(m_textureHeight*m_itemData.devicePixelRatio));
    return new QOpenGLFramebufferObject(fboSize, format);
}
#endif

#ifdef QNANO_USE_RHI
void QNanoQuickItemPainter::synchronize(QQuickRhiItem * item)
#elif QNANO_USE_RENDERNODE
void QNanoQuickItemPainter::synchronizePainter(QNanoQuickItem * item)
#else
void QNanoQuickItemPainter::synchronize(QQuickFramebufferObject * item)
#endif
{
    if (!item)
        return;

    m_window = item->window();
    QNanoQuickItem *realItem = static_cast<QNanoQuickItem*>(item);
    if (!realItem)
        return;

#ifdef QNANO_USE_RHI
    // Make sure initialization has been done
    if (!m_initialized)
        initialize(nullptr);
#endif

    QPointF origoPoint = realItem->mapToScene(QPointF(0, 0));
    m_itemData.x = static_cast<float>(origoPoint.x());
    m_itemData.y = static_cast<float>(origoPoint.y());
    m_itemData.width = static_cast<float>(realItem->width());
    m_itemData.height = static_cast<float>(realItem->height());
#ifdef QNANO_USE_RENDERNODE
    if ((m_viewWidth != m_window->width()) || (m_viewHeight != m_window->height())) {
        setViewSize(m_window->width(), m_window->height());
        sizeChanged(m_window->width(), m_window->height());
    }
    m_itemData.transformOrigin = realItem->transformOrigin();
    m_itemData.rotation = static_cast<float>(realItem->rotation());
    m_itemData.scale = static_cast<float>(realItem->scale());
    m_itemData.opacity = static_cast<float>(this->inheritedOpacity());
    m_itemData.clip = realItem->clip();
#else
    // See if user has adjusted manual texture size
    int newTextureWidth = realItem->textureWidth();
    int newTextureHeight = realItem->textureHeight();
    if ((newTextureWidth != m_textureWidth) || (newTextureHeight != m_textureHeight)) {
        m_textureWidth = newTextureWidth;
        m_textureHeight = newTextureHeight;
#ifdef QNANO_USE_RHI
        update();
#else
        invalidateFramebufferObject();
#endif
    }

    // If size has changed, update
    int viewWidth = (m_textureWidth > -1) ? m_textureWidth : static_cast<int>(item->width());
    int viewHeight = (m_textureHeight > -1) ? m_textureHeight : static_cast<int>(item->height());

    if ((static_cast<int>(width()) != viewWidth) || (static_cast<int>(height()) != viewHeight)) {
        setViewSize(viewWidth, viewHeight);
        sizeChanged(viewWidth, viewHeight);
        // Note: invalidated automatically when size changes if textureFollowsItemSize is true
        //invalidateFramebufferObject();
    }
#endif

    m_pixelAlign = realItem->pixelAlign();
    m_pixelAlignText = realItem->pixelAlignText();
    m_fillColor = realItem->fillColor();
    m_itemData.devicePixelRatio = static_cast<float>(realItem->window()->devicePixelRatio());
    bool hqr = realItem->highQualityRendering();
    if (hqr != m_highQualityRendering) {
        m_highQualityRendering = hqr;
        m_painter->enableHighQualityRendering(hqr);
    }
    bool antialiasing = realItem->antialiasing();
    if (antialiasing != m_antialiasing) {
        m_antialiasing = antialiasing;
        m_painter->enableAntialiasing(antialiasing);
    }

    realItem->setContextName(m_painter->m_openglContextName);
    if (m_painter->m_backend) {
        realItem->setBackendName(m_painter->m_backend->backendName());
    }

#ifdef QNANO_DEBUG_COLLECT
    if (m_setupDone)
        realItem->updateDebugData(m_drawDebug);
#endif
#ifdef QNANO_DEBUG_RENDER
    m_debug.start();
#endif

    synchronize(realItem);
}

void QNanoQuickItemPainter::paint(QNanoPainter *painter)
{
    Q_UNUSED(painter);
}

// Initializations to OpenGL and vg context before paint()
#ifdef QNANO_USE_RHI
void QNanoQuickItemPainter::prepaint(QRhiCommandBuffer *cb)
#else
void QNanoQuickItemPainter::prepaint()
#endif
{
#if !defined(QNANO_USE_RENDERNODE) && !defined(QNANO_USE_RHI)
    QOpenGLFunctions glF(QOpenGLContext::currentContext());
    glF.glClearColor(GLclampf(m_fillColor.redF()),
                     GLclampf(m_fillColor.greenF()),
                     GLclampf(m_fillColor.blueF()),
                     GLclampf(m_fillColor.alphaF()));
    glF.glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
#endif
    // Note: Last parameter can be used to render in lower resolution
#ifdef QNANO_USE_RHI
    m_vg.begin(cb, m_rt, m_itemData.devicePixelRatio);
    m_painter->setNvgCtx(m_vg.ctx);
#elif QNANO_USE_RENDERNODE
    nvgBeginFrameAt(m_painter->nvgCtx(), m_itemData.x, m_itemData.y, m_viewWidth, m_viewHeight, m_itemData.devicePixelRatio);
#else
    nvgBeginFrame(m_painter->nvgCtx(),
                  static_cast<float>(m_itemData.width),
                  static_cast<float>(m_itemData.height),
                  static_cast<float>(m_itemData.devicePixelRatio));
#endif

#ifdef QNANO_USE_RENDERNODE

    // Clipping from items tree
    auto cList = clipList();
    if (cList) {
        QRectF clipRect = cList->clipRect();
        nvgScissor(m_painter->nvgCtx(),
                   static_cast<float>(clipRect.x()),
                   static_cast<float>(clipRect.y()),
                   static_cast<float>(clipRect.width()),
                   static_cast<float>(clipRect.height()));
    }

    nvgGlobalAlpha(m_painter->nvgCtx(), m_itemData.opacity);
    nvgRotate(m_painter->nvgCtx(), qDegreesToRadians(m_itemData.rotation));
    nvgScale(m_painter->nvgCtx(), m_itemData.scale, m_itemData.scale);
    if (m_itemData.clip) {
          // Clipping of this item
          // Note: NanoVG clipping is single rectangular area, so both items tree clipping
          // and item clipping with different rotations can't be on at the same time.
          nvgScissor(m_painter->nvgCtx(), 0, 0, m_itemData.width, m_itemData.height);
    }
    if (m_fillColor.alpha() > 0) {
        // Background with QSGRenderNode
        m_painter->setFillStyle(QNanoColor::fromQColor(m_fillColor));
        m_painter->fillRect(0, 0, width(), height());
        m_painter->setFillStyle(QNanoColor::fromQColor(Qt::transparent));
    }

#endif
}

void QNanoQuickItemPainter::postpaint()
{
#ifdef QNANO_DEBUG_COLLECT
    m_drawDebug = nvgDrawDebug(m_painter->nvgCtx());
#endif
#ifdef QNANO_DEBUG_RENDER
    m_debug.paintDrawDebug(m_painter, width(), height());
#endif

#ifdef QNANO_USE_RHI
    m_vg.end();
#else
    nvgEndFrame(m_painter->nvgCtx());
#endif
}

#ifdef QNANO_USE_RHI
void QNanoQuickItemPainter::initialize(QRhiCommandBuffer *cb)
{
    if (!m_window)
        return;

    if (rhi() != m_rhi) {
        m_rhi = rhi();
        m_vg.destroy();
    }
    if (renderTarget() != m_rt) {
        m_rt = renderTarget();
        // in case the QRhiRenderPassDescriptor is incompatible with the new rt
        m_vg.destroy();
    }
    if (!m_vg.isValid()) {
        // Note: Enable this if rendering requires NVG_STENCIL_STROKES to be correct.
        // Also add NVG_SCISSORING if scissoring is used.
        //m_vg.create(m_rhi, NVG_ANTIALIAS | NVG_STENCIL_STROKES | NVG_SCISSORING);
        m_vg.create(m_rhi, NVG_ANTIALIAS);
        m_painter->updateContextName(m_rhi);
    }
    m_initialized = true;
}

void QNanoQuickItemPainter::render(QRhiCommandBuffer *cb)
{
    if (!m_initialized)
        return;

    if ((m_itemData.width > 0 && m_itemData.height > 0) || m_setupDone) {
        m_setupDone = true;
        prepaint(cb);
        paint(m_painter);
        postpaint();
    }

    cb->beginPass(m_rt, m_fillColor, { 1.0f, 0 });
    m_vg.render();
    cb->endPass();

}
#else
#ifdef QNANO_USE_RENDERNODE
void QNanoQuickItemPainter::render(const RenderState *)
#else
void QNanoQuickItemPainter::render()
#endif
{
    m_painter->reset(); // reset context data as painter is shared.

    m_painter->setPixelAlign(static_cast<QNanoPainter::PixelAlign>(m_pixelAlign));
    m_painter->setPixelAlignText(static_cast<QNanoPainter::PixelAlign>(m_pixelAlignText));
    m_painter->m_devicePixelRatio = m_itemData.devicePixelRatio;

    // Draw only when item has visible size. This way setup and painting is
    // not called until size has been properly set.
    if ((m_itemData.width > 0 && m_itemData.height > 0) || m_setupDone) {
        m_setupDone = true;
        prepaint();
        paint(m_painter);
        postpaint();
    }
    // Reset the GL state for Qt side
    if (m_window) {
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        QQuickOpenGLUtils::resetOpenGLState();
#else
        m_window->resetOpenGLState();
#endif
    }

}
#endif // QNANO_USE_RHI

void QNanoQuickItemPainter::setViewSize(int width, int height)
{
    m_viewWidth = width;
    m_viewHeight = height;
}

QPointF QNanoQuickItemPainter::itemTransformOrigin() const
{
    double w = static_cast<double>(m_itemData.width);
    double h = static_cast<double>(m_itemData.height);
    switch (m_itemData.transformOrigin) {
    case QQuickItem::TopLeft:
        return QPointF(0,0);
    case QQuickItem::Top:
        return QPointF(w/2,0);
    case QQuickItem::TopRight:
        return QPointF(w,0);
    case QQuickItem::Left:
        return QPointF(0,h/2);
    case QQuickItem::Center:
        return QPointF(w/2,h/2);
    case QQuickItem::Right:
        return QPointF(w,h/2);
    case QQuickItem::BottomLeft:
        return QPointF(0,h);
    case QQuickItem::Bottom:
        return QPointF(w/2,h);
    case QQuickItem::BottomRight:
        return QPointF(w,h);
    }
    return QPointF(w/2,h/2);
}


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
#include <QtMath>

/*!
    \class QNanoQuickItemPainter
    \brief The QNanoQuickItemPainter handles all painting of a QNanoQuickItem.
    \inmodule QNanoPainter

    TODO: Write more documentation here.
*/

static QSharedPointer<QNanoPainter> getSharedPainter()
{
    static QSharedPointer<QNanoPainter> s_nano = QSharedPointer<QNanoPainter>::create();
    return s_nano;
}

/*!
    Constructs a QNanoQuickItemPainter.
*/

QNanoQuickItemPainter::QNanoQuickItemPainter()
    : m_window(nullptr)
    , m_painter(getSharedPainter())
    , m_fillColor(0.0, 0.0, 0.0, 0.0)
    , m_viewWidth(0)
    , m_viewHeight(0)
    , m_antialiasing(true)
    , m_highQualityRendering(false)
    , m_pixelAlign(QNanoQuickItem::PixelAlignNone)
    , m_pixelAlignText(QNanoQuickItem::PixelAlignNone)
    , m_setupDone(false)
#ifdef QNANO_DEBUG
    , m_debugNsElapsed(0)
    , m_debugCounter(0)
    , m_debugMsElapsed("0.000")
#endif
{
    // Initialize QOpenGLFunctions for the context
    initializeOpenGLFunctions();
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
    item painter instances. This will be called before point() each
    time item needs to be repainted.

    This method is the only place where it is safe for the painter and the
    item to read and write each others variables.

    Usually you should cast \a item to your real item type, check that
    it is not null and then exchange the data.
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

#ifndef QNANO_USE_RENDERNODE
QOpenGLFramebufferObject *QNanoQuickItemPainter::createFramebufferObject(const QSize &size)
{
    QOpenGLFramebufferObjectFormat format;
    format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
    return new QOpenGLFramebufferObject(size, format);
}
#endif

#ifdef QNANO_USE_RENDERNODE
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

    QPointF origoPoint = realItem->mapToScene(QPointF(0, 0));
    m_itemData.x = origoPoint.x();
    m_itemData.y = origoPoint.y();
    m_itemData.width = realItem->width();
    m_itemData.height = realItem->height();
#ifdef QNANO_USE_RENDERNODE
    if ((m_viewWidth != m_window->width()) || (m_viewHeight != m_window->height())) {
        setViewSize(m_window->width(), m_window->height());
        sizeChanged(m_window->width(), m_window->height());
    }
    m_itemData.transformOrigin = realItem->transformOrigin();
    m_itemData.rotation = realItem->rotation();
    m_itemData.scale = realItem->scale();
    m_itemData.opacity = this->inheritedOpacity();
    m_itemData.clip = realItem->clip();
#else
    // If size has changed, update
    if (static_cast<int>(width()) != static_cast<int>(item->width()) || static_cast<int>(height()) != static_cast<int>(item->height())) {
        setViewSize(item->width(), item->height());
        sizeChanged(item->width(), item->height());
        // Note: invalidated automatically when size changes if textureFollowsItemSize is true
        //invalidateFramebufferObject();
    }
#endif

    m_pixelAlign = realItem->pixelAlign();
    m_pixelAlignText = realItem->pixelAlignText();
    m_fillColor = realItem->fillColor();
    m_itemData.devicePixelRatio = realItem->window()->devicePixelRatio();
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

#ifdef QNANO_DEBUG
    m_debugTimer.start();
#endif

    synchronize(realItem);
}

void QNanoQuickItemPainter::paint(QNanoPainter *painter)
{
    Q_UNUSED(painter);
}

// Initializations to OpenGL and vg context before paint()
void QNanoQuickItemPainter::prepaint()
{
#ifndef QNANO_USE_RENDERNODE
    glClearColor(m_fillColor.redF(), m_fillColor.greenF(), m_fillColor.blueF(), m_fillColor.alphaF());
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
#endif
    // Note: Last parameter can be used to render in lower resolution
#ifdef QNANO_USE_RENDERNODE
    nvgBeginFrameAt(m_painter->nvgCtx(), m_itemData.x, m_itemData.y, m_viewWidth, m_viewHeight, m_itemData.devicePixelRatio);
#else
    nvgBeginFrame(m_painter->nvgCtx(), m_itemData.width, m_itemData.height, m_itemData.devicePixelRatio);
#endif

#ifdef QNANO_USE_RENDERNODE

    // Clipping from items tree
    auto cList = clipList();
    if (cList) {
        QRectF clipRect = cList->clipRect();
        nvgScissor(m_painter->nvgCtx(), clipRect.x(), clipRect.y(), clipRect.width(), clipRect.height());
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
#ifdef QNANO_DEBUG
    m_drawDebug = nvgDrawDebug(m_painter->nvgCtx());
    paintDrawDebug();
#endif

    nvgEndFrame(m_painter->nvgCtx());
}


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
        paint(m_painter.data());
        postpaint();
    }
    // Reset the GL state for Qt side
    if (m_window) {
        m_window->resetOpenGLState();
    }

}

void QNanoQuickItemPainter::setViewSize(int width, int height)
{
    m_viewWidth = width;
    m_viewHeight = height;
}

QPointF QNanoQuickItemPainter::itemTransformOrigin() const
{
    double w = m_itemData.width;
    double h = m_itemData.height;
    switch (m_itemData.transformOrigin) {
    case QQuickItem::TopLeft:
        return QPointF(0,0);
        break;
    case QQuickItem::Top:
        return QPointF(w/2,0);
        break;
    case QQuickItem::TopRight:
        return QPointF(w,0);
        break;
    case QQuickItem::Left:
        return QPointF(0,h/2);
        break;
    default:
    case QQuickItem::Center:
        return QPointF(w/2,h/2);
        break;
    case QQuickItem::Right:
        return QPointF(w,h/2);
        break;
    case QQuickItem::BottomLeft:
        return QPointF(0,h);
        break;
    case QQuickItem::Bottom:
        return QPointF(w/2,h);
        break;
    case QQuickItem::BottomRight:
        return QPointF(w,h);
        break;
    }
}

#ifdef QNANO_DEBUG

void QNanoQuickItemPainter::paintDrawDebug()
{
    int totalTriCount = m_drawDebug.fillTriCount + m_drawDebug.strokeTriCount + m_drawDebug.textTriCount;
    qint64 elapsed = m_debugTimer.nsecsElapsed();
    m_debugNsElapsed += elapsed;
    m_debugCounter++;
    if(!m_debugUpdateTimer.isValid()) m_debugUpdateTimer.start();

    if (m_debugUpdateTimer.elapsed() >= 1000) {
        qint64 ms = 1000000;
        double msElapsed = (double)m_debugNsElapsed/(ms*m_debugCounter);
        m_debugMsElapsed = QString::number(msElapsed, 'f', 3);
        m_debugNsElapsed = 0;
        m_debugCounter = 0;
        m_debugUpdateTimer.start();
    }
    float fontSize = qMin((double)QNanoPainter::ptToPx(14), width()*0.05);
    int debugHeight = fontSize*2;
    int debugY = height() - debugHeight;
    m_painter->reset();

    // Background
    m_painter->setFillStyle(0xB0000000);
    m_painter->fillRect(0, debugY, width(), debugHeight);

    // Texts
    m_painter->setFillStyle(0xFFFFFFFF);
    QNanoFont font(QNanoFont::DEFAULT_FONT_NORMAL);
    font.setPixelSize(fontSize);
    m_painter->setFont(font);
    m_painter->setTextAlign(QNanoPainter::ALIGN_CENTER);
    m_painter->setTextBaseline(QNanoPainter::BASELINE_TOP);
    m_painter->setPixelAlignText(QNanoPainter::PIXEL_ALIGN_HALF);
    int textY = debugY;
    int textWidth = width()/6;
    QString debugText1 = QString("TIME\n%1").arg(m_debugMsElapsed);
    m_painter->fillText(debugText1, 0, textY, textWidth);
    QString debugText2 = QString("DCC\n%1").arg(m_drawDebug.drawCallCount);
    m_painter->fillText(debugText2, textWidth, textY, textWidth);
    QString debugText3 = QString("FILL\n%1").arg(m_drawDebug.fillTriCount);
    m_painter->fillText(debugText3, 2*textWidth, textY, textWidth);
    QString debugText4 = QString("STROKE\n%1").arg(m_drawDebug.strokeTriCount);
    m_painter->fillText(debugText4, 3*textWidth, textY, textWidth);
    QString debugText5 = QString("TEXT\n%1").arg(m_drawDebug.textTriCount);
    m_painter->fillText(debugText5, 4*textWidth, textY, textWidth);
    QString debugText6 = QString("TOTAL\n%1").arg(totalTriCount);
    m_painter->fillText(debugText6, 5*textWidth, textY, textWidth);
}

#endif

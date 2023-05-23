/**********************************************************
** Copyright (c) 2018 QUIt Coding <info@quitcoding.com>
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

#include "qnanowidget.h"
#include <QOpenGLFunctions>

QNanoWidget::QNanoWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
#ifdef QNANO_ENABLE_TOUCH_SIGNALS
    setAttribute(Qt::WA_AcceptTouchEvents);
#endif
}

/*!
    \fn void QNanoWidget::paint(QNanoPainter *painter)

    Reimplement this method to paint using \a painter.

    This will get called after the item has been filled with fillColor().
*/

/*!
    \fn QColor QNanoWidget::fillColor() const

    Returns the current fill color.

    \sa setFillColor()
*/

QColor QNanoWidget::fillColor() const
{
    return m_fillColor;
}

/*!
    \fn void QNanoWidget::setFillColor (const QColor &color)

    Set the fill color to \a color. This color will be used to draw
    the background of the item. The default color is transparent.

    \sa fillColor()
*/

void QNanoWidget::setFillColor(const QColor &color)
{
    if (m_fillColor == color)
        return;
    m_fillColor = color;

    update();
}

/*!
   \internal
*/

void QNanoWidget::initializeGL()
{
    m_painter = QNanoPainter::getInstance();
}

void QNanoWidget::paintGL()
{
    if (!m_painter)
        return;

#ifdef QNANO_DEBUG_RENDER
    m_debug.start();
#endif

    // Draw only when item has visible size. This way setup and painting is
    // not called until size has been properly set.
    if ((width() > 0 && height() > 0) || m_setupDone) {
        m_setupDone = true;
        prepaint();
        paint(m_painter);
        postpaint();
    }
}

void QNanoWidget::paint(QNanoPainter *painter)
{
#ifdef QNANO_ENABLE_PAINT_SIGNALS
    Q_EMIT paintSignal(painter);
#else
    Q_UNUSED(painter);
#endif
}

// Initializations to OpenGL and vg context before paint()
void QNanoWidget::prepaint()
{
    if (m_fillColor.alpha() > 0) {
        QOpenGLFunctions glF(QOpenGLContext::currentContext());
        glF.glClearColor(GLclampf(m_fillColor.redF()),
                         GLclampf(m_fillColor.greenF()),
                         GLclampf(m_fillColor.blueF()),
                         GLclampf(m_fillColor.alphaF()));
        glF.glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
    }

    nvgBeginFrame(m_painter->nvgCtx(), width(), height(), devicePixelRatio());
}

void QNanoWidget::postpaint()
{
#ifdef QNANO_DEBUG_RENDER
    m_debug.paintDrawDebug(m_painter, width(), height());
#endif

    nvgEndFrame(m_painter->nvgCtx());
}

#ifdef QNANO_ENABLE_TOUCH_SIGNALS
bool QNanoWidget::event(QEvent *event)
{
    switch (event->type()) {
    case QEvent::TouchBegin:
    case QEvent::TouchEnd:
    case QEvent::TouchCancel:
    case QEvent::TouchUpdate: {
        QTouchEvent *tEvent = static_cast<QTouchEvent *>(event);
        Q_EMIT touchSignal(tEvent);
        return event->isAccepted();
    }
    default:
        break;
    }
    return QWidget::event(event);
}
#endif

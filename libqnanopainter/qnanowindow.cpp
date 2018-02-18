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

#include "qnanowindow.h"

QNanoWindow::QNanoWindow()
    : QOpenGLWindow(QOpenGLWindow::NoPartialUpdate)
    , m_fillColor(0.0, 0.0, 0.0, 0.0)
    , m_painter(nullptr)
    , m_setupDone(false)
{

}

/*!
    \fn void QNanoWindow::paint(QNanoPainter *painter)

    Reimplement this method to paint using \a painter.

    This will get called after the item has been filled with fillColor().
*/

/*!
    \fn QColor QNanoWindow::fillColor() const

    Returns the current fill color.

    \sa setFillColor()
*/

QColor QNanoWindow::fillColor() const
{
    return m_fillColor;
}

/*!
    \fn void QNanoWindow::setFillColor (const QColor &color)

    Set the fill color to \a color. This color will be used to draw
    the background of the item. The default color is transparent.

    \sa fillColor()
*/

void QNanoWindow::setFillColor(const QColor &color)
{
    if (m_fillColor == color)
        return;
    m_fillColor = color;

    update();
}

/*!
   \internal
*/

void QNanoWindow::initializeGL()
{
    m_painter = QNanoPainter::getInstance();
}

void QNanoWindow::paintGL()
{
    if (!m_painter)
        return;

#ifdef QNANO_DEBUG
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

void QNanoWindow::paint(QNanoPainter *painter)
{
    Q_UNUSED(painter);
}

// Initializations to OpenGL and vg context before paint()
void QNanoWindow::prepaint()
{
    if (m_fillColor.alpha() > 0) {
        m_painter->setFillStyle(QNanoColor::fromQColor(m_fillColor));
        m_painter->fillRect(0, 0, width(), height());
        m_painter->setFillStyle(QNanoColor::fromQColor(Qt::transparent));
    }

    nvgBeginFrame(m_painter->nvgCtx(), width(), height(), devicePixelRatio());
}

void QNanoWindow::postpaint()
{
#ifdef QNANO_DEBUG
    m_debug.paintDrawDebug(m_painter, width(), height());
#endif

    nvgEndFrame(m_painter->nvgCtx());
}

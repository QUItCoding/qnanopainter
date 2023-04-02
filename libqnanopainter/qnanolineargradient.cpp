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

#include "qnanolineargradient.h"
#include "qnanopainter.h"

/*!
    \class QNanoLinearGradient
    \brief QNanoLinearGradient is a brush for linear gradient painting.
    \inmodule QNanoPainter

    TODO: Write more documentation here.
*/

/*!
    \fn QNanoLinearGradient::QNanoLinearGradient()

    Constructs a default linear gradient.
    Gradient start color position is (0,0) and end color position (0,100).
    Gradient start color is white (255,255,255) and end color transparent (0,0,0,0).
*/

QNanoLinearGradient::QNanoLinearGradient()
{
}

/*!
    \fn QNanoLinearGradient::QNanoLinearGradient(float startX, float startY, float endX, float endY)

    Constructs a linear gradient.
    Gradient start color position is ( \a startX, \a startY) and end color position ( \a endX, \a endY).
    Gradient start color is white (255,255,255) and end color transparent (0,0,0,0).
*/

QNanoLinearGradient::QNanoLinearGradient(float startX, float startY, float endX, float endY)
    : m_sx(startX)
    , m_sy(startY)
    , m_ex(endX)
    , m_ey(endY)
{
}

/*!
    \fn QNanoLinearGradient::QNanoLinearGradient(const QPointF &start, const QPointF &end)

    Constructs a linear gradient.
    Gradient start color position is \a start and end color position \a end.
    Gradient start color is white (255,255,255) and end color transparent (0,0,0,0).
*/

QNanoLinearGradient::QNanoLinearGradient(const QPointF &start, const QPointF &end)
    : m_sx(static_cast<float>(start.x()))
    , m_sy(static_cast<float>(start.y()))
    , m_ex(static_cast<float>(end.x()))
    , m_ey(static_cast<float>(end.y()))
{
}

/*!
    \fn QNanoLinearGradient::QNanoLinearGradient(float startX, float startY, float endX, float endY, const QNanoColor &startColor, const QNanoColor &endColor)

    Constructs a linear gradient.
    Gradient start color position is ( \a startX, \a startY) and end color position ( \a endX, \a endY).
    Gradient start color is \a startColor and end color \a endColor.
*/

QNanoLinearGradient::QNanoLinearGradient(float startX, float startY, float endX, float endY, const QNanoColor &startColor, const QNanoColor &endColor)
    : m_sx(startX)
    , m_sy(startY)
    , m_ex(endX)
    , m_ey(endY)
    , m_icol(startColor.nvgColor())
    , m_ocol(endColor.nvgColor())
{
}

/*!
    \fn void QNanoLinearGradient::setStartPosition(float x, float y)

    Sets the start point of linear gradient to ( \a x, \a y).
*/

void QNanoLinearGradient::setStartPosition(float x, float y)
{
    m_sx = x;
    m_sy = y;
    m_changed = true;
}

/*!
    \fn void QNanoLinearGradient::setStartPosition(const QPointF &start)

    Sets the start point of linear gradient to \a start.
*/

void QNanoLinearGradient::setStartPosition(const QPointF &start)
{
    setStartPosition(static_cast<float>(start.x()),
                     static_cast<float>(start.y()));
    m_changed = true;
}

/*!
    \fn void QNanoLinearGradient::setEndPosition(float x, float y)

    Sets the end point of linear gradient to ( \a x, \a y).
*/

void QNanoLinearGradient::setEndPosition(float x, float y)
{
    m_ex = x;
    m_ey = y;
    m_changed = true;
}

/*!
    \fn void QNanoLinearGradient::setEndPosition(const QPointF &end)

    Sets the end point of linear gradient to \a end.
*/

void QNanoLinearGradient::setEndPosition(const QPointF &end)
{
    setEndPosition(static_cast<float>(end.x()),
                   static_cast<float>(end.y()));
    m_changed = true;
}

/*!
    \fn void QNanoLinearGradient::setStartColor(const QNanoColor &color)

    Sets the start color of linear gradient to \a color.
*/

void QNanoLinearGradient::setStartColor(const QNanoColor &color)
{
    m_icol = color.nvgColor();
    m_changed = true;
}

/*!
    \fn void QNanoLinearGradient::setEndColor(const QNanoColor &color)

    Sets the end color of linear gradient to \a color.
*/

void QNanoLinearGradient::setEndColor(const QNanoColor &color)
{
    m_ocol = color.nvgColor();
    m_changed = true;
}

// ***** Private *****

/*!
   \internal
*/

NVGpaint QNanoLinearGradient::nvgPaint(NVGcontext* nvg) const
{
    if (m_changed) {
        float sx, sy, ex, ey;
        sx = m_sx;
        sy = m_sy;
        ex = m_ex;
        ey = m_ey;
        QNanoPainter *painter = parentPainter();
        Q_ASSERT(painter);
        painter->_checkAlignPixelsAdjust(&sx, &sy, &ex, &ey);
        m_nvgPaint = nvgLinearGradient(nvg, sx, sy, ex, ey, m_icol, m_ocol);
        m_changed = false;
    }
    return m_nvgPaint;
}

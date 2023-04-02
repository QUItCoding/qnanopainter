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

#include "qnanoradialgradient.h"
#include "qnanopainter.h"

/*!
    \class QNanoRadialGradient
    \brief QNanoRadialGradient is a brush for radial gradient painting.
    \inmodule QNanoPainter

    TODO: Write more documentation here.
*/

/*!
    \fn QNanoRadialGradient::QNanoRadialGradient()

    Constructs a default radial gradient.
    Gradient center position is (0,0).
    Gradient outer radius is 100.0 and inner radius is 0.0.
    Gradient start color is white (255,255,255) and end color transparent (0,0,0,0).
*/

QNanoRadialGradient::QNanoRadialGradient()
{
}

/*!
    \fn QNanoRadialGradient::QNanoRadialGradient(float centerX, float centerY, float outerRadius, float innerRadius)

    Constructs a radial gradient.
    Gradient center position is ( \a centerX, \a centerY).
    Gradient outer radius is \a outerRadius and inner radius is \a innerRadius.
    Gradient start color is white (255,255,255) and end color transparent (0,0,0,0).
*/

QNanoRadialGradient::QNanoRadialGradient(float centerX, float centerY, float outerRadius, float innerRadius)
    : m_cx(centerX)
    , m_cy(centerY)
    , m_or(outerRadius)
    , m_ir(innerRadius)
{
}

/*!
    \fn QNanoRadialGradient::QNanoRadialGradient(const QPointF &center, float outerRadius, float innerRadius)

    Constructs a radial gradient.
    Gradient center position is \a center.
    Gradient outer radius is \a outerRadius and inner radius is \a innerRadius.
    Gradient start color is white (255,255,255) and end color transparent (0,0,0,0).
*/

QNanoRadialGradient::QNanoRadialGradient(const QPointF &center, float outerRadius, float innerRadius)
    : m_cx(static_cast<float>(center.x()))
    , m_cy(static_cast<float>(center.y()))
    , m_or(static_cast<float>(outerRadius))
    , m_ir(static_cast<float>(innerRadius))
{
}

/*!
    \fn void QNanoRadialGradient::setCenterPosition(float x, float y)

    Sets the center point of radial gradient to ( \a x, \a y).
*/

void QNanoRadialGradient::setCenterPosition(float x, float y)
{
    m_cx = x;
    m_cy = y;
    m_changed = true;
}

/*!
    \fn void QNanoRadialGradient::setCenterPosition(const QPointF &center)

    Sets the center point of radial gradient to \a center.
*/

void QNanoRadialGradient::setCenterPosition(const QPointF &center)
{
    setCenterPosition(static_cast<float>(center.x()),
                      static_cast<float>(center.y()));
    m_changed = true;
}

/*!
    \fn void QNanoRadialGradient::setOuterRadius(float radius)

    Sets the outer radius of radial gradient to \a radius.
    End color will be drawn at this radius from center position.
*/

void QNanoRadialGradient::setOuterRadius(float radius)
{
    m_or = radius;
    m_changed = true;
}

/*!
    \fn void QNanoRadialGradient::setInnerRadius(float radius)

    Sets the inner radius of radial gradient to \a radius.
    Start color will be drawn at this radius from center position.
    The default inner radius is 0.0 meaning that gradient starts
    directly from center position.
*/

void QNanoRadialGradient::setInnerRadius(float radius)
{
    m_ir = radius;
    m_changed = true;
}


/*!
    \fn void QNanoRadialGradient::setStartColor(const QNanoColor &color)

    Sets the start color of radial gradient to \a color.
*/

void QNanoRadialGradient::setStartColor(const QNanoColor &color)
{
    m_icol = color.nvgColor();
    m_changed = true;
}

/*!
    \fn void QNanoRadialGradient::setEndColor(const QNanoColor &color)

    Sets the end color of radial gradient to \a color.
*/

void QNanoRadialGradient::setEndColor(const QNanoColor &color)
{
    m_ocol = color.nvgColor();
    m_changed = true;
}

// ***** Private *****

/*!
   \internal
*/

NVGpaint QNanoRadialGradient::nvgPaint(NVGcontext* nvg) const
{
    if (m_changed) {
        float cx, cy;
        cx = m_cx;
        cy = m_cy;
        QNanoPainter *painter = parentPainter();
        Q_ASSERT(painter);
        painter->_checkAlignPixelsAdjust(&cx, &cy);
        m_nvgPaint = nvgRadialGradient(nvg, cx, cy, m_ir, m_or, m_icol, m_ocol);
        m_changed = false;
    }
    return m_nvgPaint;
}


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

#include "qnanoboxgradient.h"
#include "qnanopainter.h"

/*!
    \class QNanoBoxGradient
    \brief QNanoLinearGradient is a brush for box gradient painting.
    \inmodule QNanoPainter

    TODO: Write more documentation here.
*/

/*!
    \fn QNanoBoxGradient::QNanoBoxGradient()

    Constructs a default box gradient.
    Position of gradient is (0,0) and size (100, 100)
    Gradient feather is 100.0.
    Gradient radius is 0.0.
    Gradient start color is white (255,255,255) and end color transparent (0,0,0,0).
*/

QNanoBoxGradient::QNanoBoxGradient()
{
}

/*!
    \fn QNanoBoxGradient::QNanoBoxGradient(float x, float y, float width, float height, float feather, float radius)

    Constructs a default box gradient.
    Position of gradient is ( \a x, \a y) and size ( \a width, \a height)
    Gradient feather is \a feather.
    Gradient radius is \a radius.
    Gradient start color is white (255,255,255) and end color transparent (0,0,0,0).
*/

QNanoBoxGradient::QNanoBoxGradient(float x, float y, float width, float height, float feather, float radius)
{
    m_x = x;
    m_y = y;
    m_width = width;
    m_height = height;
    m_feather = feather;
    m_radius = radius;
}

/*!
    \fn QNanoBoxGradient::QNanoBoxGradient(const QRectF &rect, float feather, float radius)

    Constructs a default box gradient.
    Position and size of gradient is \a rect.
    Gradient feather is \a feather.
    Gradient radius is \a radius.
    Gradient start color is white (255,255,255) and end color transparent (0,0,0,0).
*/

QNanoBoxGradient::QNanoBoxGradient(const QRectF &rect, float feather, float radius)
{
    m_x = static_cast<float>(rect.x());
    m_y = static_cast<float>(rect.y());
    m_width = static_cast<float>(rect.width());
    m_height = static_cast<float>(rect.height());
    m_feather = feather;
    m_radius = radius;
}

/*!
    \fn void QNanoBoxGradient::setRect(float x, float y, float width, float height)

    Sets the rectangle of box gradient to position ( \a x, \a y) and size ( \a width, \a height).
*/

void QNanoBoxGradient::setRect(float x, float y, float width, float height)
{
    m_x = x;
    m_y = y;
    m_width = width;
    m_height = height;
    m_changed = true;
}

/*!
    \fn void QNanoBoxGradient::setRect(const QRectF &rect)

    Sets the rectangle of box gradient to \a rect.
*/

void QNanoBoxGradient::setRect(const QRectF &rect)
{
    setRect(static_cast<float>(rect.x()), static_cast<float>(rect.y()),
            static_cast<float>(rect.width()), static_cast<float>(rect.height()));
}

/*!
    \fn void QNanoBoxGradient::setFeather(float feather)

    Sets the feather of box gradient to \a feather.
*/

void QNanoBoxGradient::setFeather(float feather)
{
    m_feather = feather;
    m_changed = true;
}

/*!
    \fn void QNanoBoxGradient::setRadius(float radius)

    Sets the radius of box gradient to \a radius.
*/

void QNanoBoxGradient::setRadius(float radius)
{
    m_radius = radius;
    m_changed = true;
}


/*!
    \fn QNanoBoxGradient::setStartColor(const QNanoColor &color)

    Sets the start color of box gradient to \a color.
*/

void QNanoBoxGradient::setStartColor(const QNanoColor &color)
{
    m_icol = color.nvgColor();
    m_changed = true;
}

/*!
    \fn QNanoBoxGradient::setEndColor(const QNanoColor &color)

    Sets the end color of box gradient to \a color.
*/

void QNanoBoxGradient::setEndColor(const QNanoColor &color)
{
    m_ocol = color.nvgColor();
    m_changed = true;
}

// ***** Private *****

/*!
   \internal
*/

NVGpaint QNanoBoxGradient::nvgPaint(NVGcontext* nvg) const
{
    if (m_changed) {
        float x, y, width, height;
        x = m_x;
        y = m_y;
        width = m_width;
        height = m_height;
        QNanoPainter *painter = parentPainter();
        if (painter) {
            painter->_checkAlignPixelsAdjust(&x, &y);
            painter->_checkAlignPixels(&width, &height);
        }
        m_nvgPaint = nvgBoxGradient(nvg, x+m_feather/2, y+m_feather/2, width-m_feather, height-m_feather, m_radius, m_feather, m_icol, m_ocol);
        m_changed = false;
    }
    return m_nvgPaint;
}

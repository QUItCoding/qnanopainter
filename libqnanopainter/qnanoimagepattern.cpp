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

#include "qnanoimagepattern.h"
#include "qnanopainter.h"

/*!
    \class QNanoImagePattern
    \brief QNanoImagePattern is a brush for painting image patterns.
    \inmodule QNanoPainter

    TODO: Write more documentation here.
*/

/*!
    \fn QNanoImagePattern::QNanoImagePattern()

    Constructs a default image pattern.
    Image will not be set, please use setImage() after this constructor.
    Pattern start position position is (0,0) and pattern size (100,100).
    Pattern angle is 0.0 and alpha 1.0 (fully opaque).

    \sa setImage()
*/

QNanoImagePattern::QNanoImagePattern()
{
}

/*!
    \fn QNanoImagePattern::QNanoImagePattern(const QNanoImage &image)

    Constructs a default image pattern.
    Pattern will use \a image. Please note that \a image should usually
    have REPEATX and REPEATY flags set.
    Pattern start position position is (0,0) and pattern size (100,100).
    Pattern angle is 0.0 and alpha 1.0 (fully opaque).
*/

QNanoImagePattern::QNanoImagePattern(const QNanoImage &image)
{
    m_image = const_cast<QNanoImage*>(&image);
    m_width = m_image->width();
    m_height = m_image->height();
}

/*!
    \fn QNanoImagePattern::QNanoImagePattern(const QNanoImage &image, const QRectF &rect, float angle, float alpha)

    Constructs a default image pattern.
    Pattern will use \a image. Please note that \a image should usually
    have REPEATX and REPEATY flags set.
    Pattern image position position and size is defined with \a rect.
    Pattern angle is \a angle and alpha is \a alpha.
*/

QNanoImagePattern::QNanoImagePattern(const QNanoImage &image, const QRectF &rect, float angle, float alpha)
    : m_x(static_cast<float>(rect.x()))
    , m_y(static_cast<float>(rect.y()))
    , m_width(static_cast<float>(rect.width()))
    , m_height(static_cast<float>(rect.height()))
    , m_angle(angle)
    , m_alpha(alpha)
{
    m_image = const_cast<QNanoImage*>(&image);
}

/*!
    \fn QNanoImagePattern::QNanoImagePattern(const QNanoImage &image, float x, float y, float width, float height, float angle, float alpha)

    Constructs a default image pattern.
    Pattern will use \a image. Please note that \a image should usually
    have REPEATX and REPEATY flags set.
    Pattern image start position position is ( \a x, \a y) and pattern size ( \a width, \a height).
    Pattern angle is \a angle and alpha is \a alpha.
*/

QNanoImagePattern::QNanoImagePattern(const QNanoImage &image, float x, float y, float width, float height, float angle, float alpha)
    : m_x(x)
    , m_y(y)
    , m_width(width)
    , m_height(height)
    , m_angle(angle)
    , m_alpha(alpha)
{
    m_image = const_cast<QNanoImage*>(&image);
}

/*!
    \fn void QNanoImagePattern::setStartPosition(float x, float y)

    Sets the start point of image pattern to ( \a x, \a y).
    Start position means top-left corner of an image in pattern.
    Pattern will then be extended to all positions from here
    (if image REPEATX and REPEATY flags have been set).
*/

void QNanoImagePattern::setStartPosition(float x, float y)
{
    m_x = x;
    m_y = y;
    m_changed = true;
}

/*!
    \fn void QNanoImagePattern::setStartPosition(const QPointF &point)

    Sets the start point of image pattern to \a point.
    Start position means top-left corner of an image in pattern.
    Pattern will then be extended to all positions from here
    (if image REPEATX and REPEATY flags have been set).
*/

void QNanoImagePattern::setStartPosition(const QPointF &point)
{
    setStartPosition(static_cast<float>(point.x()),
                     static_cast<float>(point.y()));
    m_changed = true;
}

/*!
    \fn void QNanoImagePattern::setImageSize(float width, float height)

    Sets the size of a single image in pattern to ( \a width, \a height).
*/

void QNanoImagePattern::setImageSize(float width, float height)
{
    m_width = width;
    m_height = height;
    m_changed = true;
}

/*!
    \fn void QNanoImagePattern::setImageSize(const QSizeF &size)

    Sets the size of a single image in pattern to \a size.
*/

void QNanoImagePattern::setImageSize(const QSizeF &size)
{
    setImageSize(static_cast<float>(size.width()),
                 static_cast<float>(size.height()));
    m_changed = true;
}

/*!
    \fn void QNanoImagePattern::setImage(const QNanoImage &image)

    Sets the image of the pattern to \a image.
    Note: Image should usually have REPEATX and REPEATY flags set
    when used in image pattern.
*/

void QNanoImagePattern::setImage(const QNanoImage &image)
{
    m_image = const_cast<QNanoImage*>(&image);
    m_changed = true;
}

/*!
    \fn void QNanoImagePattern::setAlpha(float alpha)

    Sets the pattern alpha (transparency) to \a alpha.
    The default value is 1.0 meaning fully opaque.
    This alpha will only affect the image pattern.
    To set alpha globally, use QNanoPainter::setGlobalAlpha()
*/

void QNanoImagePattern::setAlpha(float alpha)
{
    m_alpha = alpha;
    m_changed = true;
}

/*!
    \fn void QNanoImagePattern::setRotation(float rotation)

    Sets the pattern rotation to \a rotation.
    Rotation is done around image startPoisition.
    The default value is 0.0 meaning image is not rotated.
*/

void QNanoImagePattern::setRotation(float rotation)
{
    m_angle = rotation;
    m_changed = true;
}

// ***** Private *****

/*!
   \internal
*/

NVGpaint QNanoImagePattern::nvgPaint(NVGcontext* nvg) const
{
    if (m_changed) {
        if (!m_image) {
            qWarning() << "No image set for pattern, please use setImage()";
            // When image is missing, use instead solid gradient paint
            m_nvgPaint = nvgLinearGradient(nvg, 0, 0, 100, 100, nvgRGB(0, 0, 0), nvgRGB(0, 0, 0));
        } else {
            float x, y, width, height;
            x = m_x;
            y = m_y;
            width = m_width;
            height = m_height;
            QNanoPainter *painter = parentPainter();
            Q_ASSERT(painter);
            painter->_checkAlignPixelsAdjust(&x, &y);
            painter->_checkAlignPixels(&width, &height);
            m_nvgPaint = nvgImagePattern(nvg, x, y, width, height, m_angle, m_image->getID(nvg), m_alpha);
        }
        m_changed = false;
    }
    return m_nvgPaint;
}


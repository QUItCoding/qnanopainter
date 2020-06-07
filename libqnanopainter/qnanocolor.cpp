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

#include "qnanocolor.h"
#include <QDebug>

// ***** Constructors *****

/*!
    \class QNanoColor
    \brief QNanoColor is a brush for solid color painting.
    \inmodule QNanoPainter

    TODO: Write more documentation here.
*/

/*!
    \fn QNanoColor::QNanoColor()

    Constructs a default color with the RGB value (0,0,0), so black.
*/

QNanoColor::QNanoColor()
{
    m_nvgColor = nvgRGB(0, 0, 0);
}

/*!
    \fn QNanoColor::QNanoColor(int r, int g, int b, int a)

    Constructs a color with the RGB value \a r ,\a g, \a b and alpha (transparency) \a a.
    All values should be given between 0 and 255. If not given, alpha will be 255.
*/

QNanoColor::QNanoColor(int r, int g, int b, int a)
{
    m_nvgColor = nvgRGBA(static_cast<unsigned char>(r),
                         static_cast<unsigned char>(g),
                         static_cast<unsigned char>(b),
                         static_cast<unsigned char>(a));
}

/*!
    \fn QNanoColor::QNanoColor(unsigned int hex)

    Constructs a color with the 0xAARRGGBB format from \a hex.
    Parameter \a hex is unsigned integer so when assigning
    value into it as hexadecimal, precede it with 0x or 0X.
    Each color pair should be a hex value between 00 (0) and FF (255).

    Correct format is not checked. Select this constructor over char
    parameter version when slight performance gain outweights code clarity.

    \sa setHexColor()
*/

QNanoColor::QNanoColor(unsigned int hex)
{
    setHexColor(hex);
}

/*!
    \fn QNanoColor::QNanoColor(const char *hex)

    Constructs a color with the #RRGGBB or #AARRGGBB format from \a hex.
    First character should be '#' and each pair should be a hex value
    between 00 (0) and FF (255). If not given, alpha will be FF.
    If color is not in valid format it will be initialized as black RGB (0,0,0)

    \sa setHexColor()
*/

QNanoColor::QNanoColor(const char *hex)
{
    setHexColor(hex);
}

// ***** Instance methods *****

/*!
    \fn void QNanoColor::setHexColor(unsigned int hex)

    Set the color with 0xAARRGGBB format from \a hex.
    Parameter \a hex is unsigned integer so when assigning
    value into it as hexadecimal precede it with 0x or 0X.
    Each color pair should be a hex value between 00 (0) and FF (255).

    Correct format is not checked. Select this constructor over char
    parameter version when slight performance gain outweights code clarity.
*/

void QNanoColor::setHexColor(unsigned int hex)
{
    m_nvgColor.b = static_cast<float>(0x000000FF & hex) / 255.0f;
    hex = hex >> 8;
    m_nvgColor.g = static_cast<float>(0x000000FF & hex) / 255.0f;
    hex = hex >> 8;
    m_nvgColor.r = static_cast<float>(0x000000FF & hex) / 255.0f;
    hex = hex >> 8;
    m_nvgColor.a = static_cast<float>(0x000000FF & hex) / 255.0f;
}

/*!
    \fn void QNanoColor::setHexColor(const char *hex)

    Set the color with #RRGGBB or #AARRGGBB format from \a hex.
    First character should be '#' and each pair should be a hex value
    between 00 (0) and FF (255). If not given, alpha will be FF.
    If color is not in valid format it will be initialized as black RGB (0,0,0)
*/

void QNanoColor::setHexColor(const char *hex)
{
    const QString s = QString::fromLatin1(hex);
    const int length = s.length();
    if (!s.startsWith("#") || (length != 7 && length != 9)) {
        qWarning() << "Invalid hex color: " << hex;
        m_nvgColor = nvgRGBf(0.0f, 0.0f, 0.0f);
        return;
    }

    bool ok;
    uchar a,r,g,b;
    if (length == 7) {
        a = 255;
        r = static_cast<unsigned char>(s.mid(1,2).toInt(&ok, 16));
        g = static_cast<unsigned char>(s.mid(3,2).toInt(&ok, 16));
        b = static_cast<unsigned char>(s.mid(5,2).toInt(&ok, 16));
    } else {
        a = static_cast<unsigned char>(s.mid(1,2).toInt(&ok, 16));
        r = static_cast<unsigned char>(s.mid(3,2).toInt(&ok, 16));
        g = static_cast<unsigned char>(s.mid(5,2).toInt(&ok, 16));
        b = static_cast<unsigned char>(s.mid(7,2).toInt(&ok, 16));
    }
    m_nvgColor = nvgRGBA(r, g, b, a);
}

/*!
    \fn int QNanoColor::red() const

    Returns the red color component of this color.
*/

int QNanoColor::red() const
{
    return static_cast<int>(m_nvgColor.r * 255.0f);
}

/*!
    \fn int QNanoColor::green() const

    Returns the green color component of this color.
*/

int QNanoColor::green() const
{
    return static_cast<int>(m_nvgColor.g * 255.0f);
}

/*!
    \fn int QNanoColor::blue() const

    Returns the blue color component of this color.
*/

int QNanoColor::blue() const
{
    return static_cast<int>(m_nvgColor.b * 255.0f);
}

/*!
    \fn int QNanoColor::alpha() const

    Returns the alpha color component of this color.
*/

int QNanoColor::alpha() const
{
    return static_cast<int>(m_nvgColor.a * 255.0f);
}

/*!
    \fn void QNanoColor::setRed(int red)

    Sets the red color component of this color to \a red.
    Value should be between 0 and 255.
*/

void QNanoColor::setRed(int red)
{
    m_nvgColor.r = red / 255.0f;
}

/*!
    \fn void QNanoColor::setGreen(int green)

    Sets the green color component of this color to \a green.
    Value should be between 0 and 255.
*/

void QNanoColor::setGreen(int green)
{
    m_nvgColor.g = green / 255.0f;
}

/*!
    \fn void QNanoColor::setBlue(int blue)

    Sets the blue color component of this color to \a blue.
    Value should be between 0 and 255.
*/

void QNanoColor::setBlue(int blue)
{
    m_nvgColor.b = blue / 255.0f;
}

/*!
    \fn void QNanoColor::setAlpha(int alpha)

    Sets the alpha color component of this color to \a alpha.
    Value should be between 0 and 255.
*/

void QNanoColor::setAlpha(int alpha)
{
    m_nvgColor.a = alpha / 255.0f;
}

/*!
    \fn float QNanoColor::redF() const

    Returns the red color component of this color.
    Returned value will be between 0.0 and 1.0.
*/

float QNanoColor::redF() const
{
    return m_nvgColor.r;
}

/*!
    \fn float QNanoColor::greenF() const

    Returns the green color component of this color.
    Returned value will be between 0.0 and 1.0.
*/

float QNanoColor::greenF() const
{
    return m_nvgColor.g;
}

/*!
    \fn float QNanoColor::blueF() const

    Returns the blue color component of this color.
    Returned value will be between 0.0 and 1.0.
*/

float QNanoColor::blueF() const
{
    return m_nvgColor.b;
}

/*!
    \fn float QNanoColor::alphaF() const

    Returns the alpha color component of this color.
    Returned value will be between 0.0 and 1.0.
*/

float QNanoColor::alphaF() const
{
    return m_nvgColor.a;
}

/*!
    \fn void QNanoColor::setRedF(float red)

    Sets the red color component of this color to \a red.
    Value should be between 0.0 and 1.0.
*/

void QNanoColor::setRedF(float red)
{
    m_nvgColor.r = red;
}

/*!
    \fn void QNanoColor::setGreenF(float green)

    Sets the green color component of this color to \a green.
    Value should be between 0.0 and 1.0.
*/

void QNanoColor::setGreenF(float green)
{
    m_nvgColor.g = green;
}

/*!
    \fn void QNanoColor::setBlueF(float blue)

    Sets the blue color component of this color to \a blue.
    Value should be between 0.0 and 1.0.
*/

void QNanoColor::setBlueF(float blue)
{
    m_nvgColor.b = blue;
}

/*!
    \fn void QNanoColor::setAlphaF(float alpha)

    Sets the alpha color component of this color to \a alpha.
    Value should be between 0.0 and 1.0.
*/

void QNanoColor::setAlphaF(float alpha)
{
    m_nvgColor.a = alpha;
}

// ***** Static methods *****

/*!
    \fn QNanoColor QNanoColor::fromHSLA(int hue, int saturation, int lightness, int alpha)

    Static convenience function that returns a color constructed
    from HSLA (\a hue, \a saturation, \a lightness, \a alpha).
    H value should be in range 0..359 and SLA values in range 0..255
*/

QNanoColor QNanoColor::fromHSLA(int hue, int saturation, int lightness, int alpha)
{
    return QNanoColor::fromNVGColor(nvgHSLA(hue/359.0f, saturation/255.0f, lightness/255.0f, static_cast<uchar>(alpha)));
}

/*!
    \fn QNanoColor QNanoColor::fromQColor(const QColor &color)

    Static convenience function that returns a color constructed
    from QColor \a color. See http://doc.qt.io/qt-5/qcolor.html
*/

QNanoColor QNanoColor::fromQColor(const QColor &color)
{
    return QNanoColor(color.red(), color.green(), color.blue(), color.alpha());
}

/*!
    \fn QNanoColor QNanoColor::fromMix(const QNanoColor &color1, const QNanoColor &color2, float amount)

    Static convenience function that returns a color linearly interpolated between
    \a color1 and \a color2. The share \a amount should be between 0.0 and 1.0.
*/

QNanoColor QNanoColor::fromMix(const QNanoColor &color1, const QNanoColor &color2, float amount)
{
    return QNanoColor::fromNVGColor(nvgLerpRGBA(color1.nvgColor(), color2.nvgColor(), amount));
}

bool QNanoColor::operator ==(const QNanoColor& color) const
{
    return qFuzzyCompare(color.m_nvgColor.r, m_nvgColor.r) &&
           qFuzzyCompare(color.m_nvgColor.g, m_nvgColor.g) &&
           qFuzzyCompare(color.m_nvgColor.b, m_nvgColor.b) &&
           qFuzzyCompare(color.m_nvgColor.a, m_nvgColor.a);
}

bool QNanoColor::operator !=(const QNanoColor& color) const
{
    return !operator==(color);
}

/*!
   \internal
*/

QNanoColor QNanoColor::fromNVGColor(NVGcolor c0)
{
    return QNanoColor(static_cast<int>(c0.r*255), static_cast<int>(c0.g*255), static_cast<int>(c0.b*255), static_cast<int>(c0.a*255));
}

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

#ifndef QNANOCOLOR_H
#define QNANOCOLOR_H

#include "nanovg/nanovg.h"
#include <QColor>

class QNanoColor
{

public:

    QNanoColor();
    QNanoColor(int r, int g, int b, int a = 255);
    QNanoColor(unsigned int hex);
    QNanoColor(const char *hex);

    int red() const;
    int green() const;
    int blue() const;
    int alpha() const;
    void setRed(int red);
    void setGreen(int green);
    void setBlue(int blue);
    void setAlpha(int alpha);

    float redF() const;
    float greenF() const;
    float blueF() const;
    float alphaF() const;
    void setRedF(float red);
    void setGreenF(float green);
    void setBlueF(float blue);
    void setAlphaF(float alpha);

    void setHexColor(unsigned int hex);
    void setHexColor(const char *hex);

    static QNanoColor fromHSLA(int hue, int saturation, int lightness, int alpha = 255);
    static QNanoColor fromQColor(const QColor &c0);
    static QNanoColor fromMix(const QNanoColor &color1, const QNanoColor &color2, float amount);

    bool operator ==(const QNanoColor& color) const;
    bool operator !=(const QNanoColor& color) const;

private:

    friend class QNanoPainter;
    friend class QNanoLinearGradient;
    friend class QNanoRadialGradient;
    friend class QNanoBoxGradient;

    inline NVGcolor nvgColor() const {
        return m_nvgColor;
    }

    static QNanoColor fromNVGColor(NVGcolor color);

    NVGcolor m_nvgColor;

};

#endif // QNANOCOLOR_H

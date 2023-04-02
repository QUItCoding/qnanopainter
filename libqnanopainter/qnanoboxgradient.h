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

#ifndef QNANOBOXGRADIENT_H
#define QNANOBOXGRADIENT_H

#include "nanovg/nanovg.h"
#include "private/qnanobrush.h"
#include "qnanocolor.h"
#include <QRectF>

class QNanoBoxGradient : public QNanoBrush
{
public:
    QNanoBoxGradient();
    QNanoBoxGradient(float x, float y, float width, float height, float feather, float radius = 0.0f);
    QNanoBoxGradient(const QRectF &rect, float feather, float radius = 0.0f);

    void setRect(float x, float y, float width, float height);
    void setRect(const QRectF &rect);
    void setFeather(float feather);
    void setRadius(float radius);
    void setStartColor(const QNanoColor &color);
    void setEndColor(const QNanoColor &color);

private:
    NVGpaint nvgPaint(NVGcontext* nvg) const Q_DECL_OVERRIDE Q_DECL_FINAL;

    float m_x = 0.0f;
    float m_y = 0.0f;
    float m_width = 100.0f;
    float m_height = 100.0f;
    float m_feather = 10.0f;
    float m_radius = 0.0f;
    NVGcolor m_icol = nvgRGB(255, 255, 255);;
    NVGcolor m_ocol = nvgRGBA(0, 0, 0, 0);
    mutable bool m_changed = true;

};

#endif // QNANOBOXGRADIENT_H

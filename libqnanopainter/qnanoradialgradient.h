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

#ifndef QNANORADIALGRADIENT_H
#define QNANORADIALGRADIENT_H

#include "nanovg/nanovg.h"
#include "private/qnanobrush.h"
#include "qnanocolor.h"
#include <QPointF>

class QNanoRadialGradient : public QNanoBrush
{
public:
    QNanoRadialGradient();
    QNanoRadialGradient(float centerX, float centerY, float outerRadius, float innerRadius = 0.0);
    QNanoRadialGradient(const QPointF &center, float outerRadius, float innerRadius = 0.0);

    void setCenterPosition(float x, float y);
    void setCenterPosition(const QPointF &center);
    void setOuterRadius(float radius);
    void setInnerRadius(float radius);
    void setStartColor(const QNanoColor &color);
    void setEndColor(const QNanoColor &color);

private:
    NVGpaint nvgPaint(NVGcontext* nvg) const Q_DECL_OVERRIDE Q_DECL_FINAL;

    mutable bool m_changed;
    float m_cx, m_cy, m_or, m_ir;
    NVGcolor m_icol, m_ocol;
};

#endif // QNANORADIALGRADIENT_H

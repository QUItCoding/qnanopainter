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

#ifndef QNANOLINEARGRADIENT_H
#define QNANOLINEARGRADIENT_H

#include "nanovg/nanovg.h"
#include "private/qnanobrush.h"
#include "qnanocolor.h"
#include <QPointF>

class QNanoLinearGradient : public QNanoBrush
{
public:
    QNanoLinearGradient();
    QNanoLinearGradient(float startX, float startY, float endX, float endY);
    QNanoLinearGradient(const QPointF &start, const QPointF &end);    
    QNanoLinearGradient(float startX, float startY, float endX, float endY, const QNanoColor &startColor, const QNanoColor &endColor);

    void setStartPosition(float x, float y);
    void setStartPosition(const QPointF &start);
    void setEndPosition(float x, float y);
    void setEndPosition(const QPointF &end);
    void setStartColor(const QNanoColor &color);
    void setEndColor(const QNanoColor &color);

private:
    NVGpaint nvgPaint(NVGcontext* nvg) const Q_DECL_OVERRIDE Q_DECL_FINAL;

    mutable bool m_changed = true;
    float m_sx = 0.0f;
    float m_sy = 0.0f;
    float m_ex = 0.0f;
    float m_ey = 100.0f;
    NVGcolor m_icol = nvgRGB(255, 255, 255);
    NVGcolor m_ocol = nvgRGBA(0, 0, 0, 0);
};

#endif // QNANOLINEARGRADIENT_H

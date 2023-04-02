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

#ifndef QNANOIMAGEPATTERN_H
#define QNANOIMAGEPATTERN_H

#include "nanovg/nanovg.h"
#include "private/qnanobrush.h"
#include "qnanocolor.h"
#include "qnanoimage.h"
#include <QPointF>
#include <QRectF>
#include <QSizeF>
#include <QDebug>

class QNanoImagePattern : public QNanoBrush
{
public:
    QNanoImagePattern();
    QNanoImagePattern(const QNanoImage &image);
    QNanoImagePattern(const QNanoImage &image, const QRectF &rect, float angle=0.0f, float alpha=1.0f);
    QNanoImagePattern(const QNanoImage &image, float x, float y, float width, float height, float angle=0.0f, float alpha=1.0f);

    void setStartPosition(float x, float y);
    void setStartPosition(const QPointF &point);
    void setImageSize(float width, float height);
    void setImageSize(const QSizeF &size);
    void setImage(const QNanoImage &image);
    void setAlpha(float alpha);
    void setRotation(float rotation);

private:
    friend class QNanoPainter;

    NVGpaint nvgPaint(NVGcontext* nvg) const Q_DECL_OVERRIDE Q_DECL_FINAL;

    mutable bool m_changed = true;
    float m_x = 0.0f;
    float m_y = 0.0f;
    float m_width = 100.0f;
    float m_height = 100.0f;
    float m_angle = 0.0f;
    float m_alpha = 1.0f;
    QNanoImage *m_image = nullptr;
};

#endif // QNANOIMAGEPATTERN_H

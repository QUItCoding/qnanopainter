/**********************************************************
** Copyright (c) 2018 QUIt Coding <info@quitcoding.com>
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

#ifndef PAINTHELPER_H
#define PAINTHELPER_H

#include <algorithm>
#include "qnanopainter.h"

static void paintHelloItem(QNanoPainter *p, float width, float height)
{
    float size = std::min(width, height);
    QPointF center(width/2, height/2);
    // Paint the background circle
    QNanoRadialGradient gradient1(center.x(), center.y() - size*0.1, size*0.6);
    gradient1.setStartColor("#909090");
    gradient1.setEndColor("#404040");
    p->beginPath();
    p->circle(center, size*0.46);
    p->setFillStyle(gradient1);
    p->fill();
    p->setStrokeStyle("#202020");
    p->setLineWidth(size*0.02);
    p->stroke();
    // Hello text
    p->setTextAlign(QNanoPainter::ALIGN_CENTER);
    p->setTextBaseline(QNanoPainter::BASELINE_MIDDLE);
    QNanoFont font1(QNanoFont::DEFAULT_FONT_BOLD_ITALIC);
    font1.setPixelSize(size*0.08);
    p->setFont(font1);
    p->setFillStyle("#B0D040");
    p->fillText("HELLO", center.x(), center.y() - size*0.18);
    // QNanoPainter text
    QNanoFont font2(QNanoFont::DEFAULT_FONT_THIN);
    font2.setPixelSize(size*0.16);
    p->setFont(font2);
    p->fillText("QNanoPainter", center.x(), center.y() - size*0.08);
    // Paint heart
    QNanoImage logo(":/quitlogo.png",
                    QNanoImage::GENERATE_MIPMAPS |
                    QNanoImage::REPEATX |
                    QNanoImage::REPEATY);
    float patternSize = size*0.02;
    QNanoImagePattern pattern(logo, center.x(), center.y(), patternSize, patternSize);
    p->setFillStyle(pattern);
    p->setLineCap(QNanoPainter::CAP_ROUND);
    p->setStrokeStyle("#B0D040");
    p->beginPath();
    p->moveTo(center.x(), center.y() + size*0.3);
    p->bezierTo(center.x() - size*0.25, center.y() + size*0.1,
                center.x() - size*0.05, center.y() + size*0.05,
                center.x(), center.y() + size*0.15);
    p->bezierTo(center.x() + size*0.05, center.y() + size*0.05,
                center.x() + size*0.25, center.y() + size*0.1,
                center.x(), center.y() + size*0.3);
    p->stroke();
    p->fill();
}

#endif // PAINTHELPER_H

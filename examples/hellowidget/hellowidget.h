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

#ifndef HELLOWIDGET_H
#define HELLOWIDGET_H

#include "qnanowidget.h"
#include "qnanopainter.h"

class HelloWidget : public QNanoWidget
{

public:
    HelloWidget()
    {
        setFillColor("#ffffff");
    }

    void paint(QNanoPainter *p)
    {
        // Paint the background circle
        p->beginPath();
        p->circle(width()*0.5, height()*0.5, width()*0.46);
        QNanoRadialGradient gradient1(width()*0.5, height()*0.4, width()*0.6, width()*0.2);
        gradient1.setStartColor("#808080");
        gradient1.setEndColor("#404040");
        p->setFillStyle(gradient1);
        p->fill();
        p->setStrokeStyle("#202020");
        p->setLineWidth(width()*0.02);
        p->stroke();
        // Hello text
        p->setTextAlign(QNanoPainter::ALIGN_CENTER);
        p->setTextBaseline(QNanoPainter::BASELINE_MIDDLE);
        QNanoFont font1(QNanoFont::DEFAULT_FONT_BOLD_ITALIC);
        font1.setPixelSize(width()*0.08);
        p->setFont(font1);
        p->setFillStyle("#B0D040");
        p->fillText("HELLO", width()*0.5, height()*0.4);
        // QNanoPainter text
        QNanoFont font2(QNanoFont::DEFAULT_FONT_THIN);
        font2.setPixelSize(width()*0.18);
        p->setFont(font2);
        p->fillText("QNanoPainter", width()*0.5, height()*0.5);
    }
};

#endif // HELLOWIDGET_H

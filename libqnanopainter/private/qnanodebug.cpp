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

#include "qnanodebug.h"

QNanoDebug::QNanoDebug()
    : m_debugNsElapsed(0)
    , m_debugCounter(0)
    , m_debugMsElapsed("0.000")
{
}

void QNanoDebug::start()
{
    m_debugTimer.start();
}

void QNanoDebug::paintDrawDebug(QNanoPainter *painter, float width, float height)
{
    m_drawDebug = nvgDrawDebug(painter->nvgCtx());

    int totalTriCount = m_drawDebug.fillTriCount + m_drawDebug.strokeTriCount + m_drawDebug.textTriCount;
    qint64 elapsed = m_debugTimer.nsecsElapsed();
    m_debugNsElapsed += elapsed;
    m_debugCounter++;
    if(!m_debugUpdateTimer.isValid()) m_debugUpdateTimer.start();

    // How often time is updated, in seconds
    // Longer period increases accuracy
    const int UPDATE_FREQUENCY_MS = 1000;
    if (m_debugUpdateTimer.elapsed() >= UPDATE_FREQUENCY_MS) {
        qint64 ms = 1000000;
        double msElapsed = static_cast<double>(m_debugNsElapsed)/(ms*m_debugCounter);
        m_debugMsElapsed = QString::number(msElapsed, 'f', 3);
        m_debugNsElapsed = 0;
        m_debugCounter = 0;
        m_debugUpdateTimer.start();
    }
    float fontSize = qMin(QNanoPainter::ptToPx(14), width*0.04f);
    float debugHeight = fontSize*2;
    float debugY = height - debugHeight;
    painter->reset();

    // Background
    painter->setFillStyle(0xB0000000);
    painter->fillRect(0, debugY, width, debugHeight);

    // Texts
    painter->setFillStyle(0xFFFFFFFF);
    QNanoFont font(QNanoFont::DEFAULT_FONT_NORMAL);
    font.setPixelSize(fontSize);
    painter->setFont(font);
    painter->setTextAlign(QNanoPainter::ALIGN_CENTER);
    painter->setTextBaseline(QNanoPainter::BASELINE_TOP);
    painter->setPixelAlignText(QNanoPainter::PIXEL_ALIGN_HALF);
    float textY = debugY;
    float textWidth = width/6;
    QString debugText1 = QString("TIME\n%1").arg(m_debugMsElapsed);
    painter->fillText(debugText1, 0, textY, textWidth);
    QString debugText2 = QString("DCC\n%1").arg(m_drawDebug.drawCallCount);
    painter->fillText(debugText2, textWidth, textY, textWidth);
    QString debugText3 = QString("FILL\n%1").arg(m_drawDebug.fillTriCount);
    painter->fillText(debugText3, 2*textWidth, textY, textWidth);
    QString debugText4 = QString("STROKE\n%1").arg(m_drawDebug.strokeTriCount);
    painter->fillText(debugText4, 3*textWidth, textY, textWidth);
    QString debugText5 = QString("TEXT\n%1").arg(m_drawDebug.textTriCount);
    painter->fillText(debugText5, 4*textWidth, textY, textWidth);
    QString debugText6 = QString("TOTAL\n%1").arg(totalTriCount);
    painter->fillText(debugText6, 5*textWidth, textY, textWidth);
}

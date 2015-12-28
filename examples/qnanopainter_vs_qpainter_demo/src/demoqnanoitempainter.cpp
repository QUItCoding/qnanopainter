#include "demoqnanoitempainter.h"
#include "demoqnanoitem.h"
#include <math.h>
#include <QtMath>
#include <QVarLengthArray>

DemoQNanoItemPainter::DemoQNanoItemPainter()
    : m_animationTime(0.0)
    , m_enabledTests(0)
    , m_testCount(1)
{
    // Setup colors
    m_colorWhite = QNanoColor(255,255,255,255);
    m_colorGray = QNanoColor(180,180,180,255);
    m_colorBlack = QNanoColor(0,0,0,255);
    m_color1 = QNanoColor(180,190,40,20);
    m_color2 = QNanoColor(255,255,255,150);
    m_color3 = QNanoColor(255,255,255,80);

    m_circleImage = QNanoImage(":/qml/images/circle.png", QNanoImage::GENERATE_MIPMAPS);

    m_testFont = QNanoFont(":/qml/fonts/Roboto-Regular.ttf");
}

void DemoQNanoItemPainter::synchronize(QNanoQuickItem *item)
{
    // Setting values here synchronized
    DemoQNanoItem *realItem = static_cast<DemoQNanoItem*>(item);
    if (realItem) {
        m_animationTime = realItem->animationTime();
        m_enabledTests = realItem->enabledTests();
        m_testCount = realItem->testCount();
    }
}


void DemoQNanoItemPainter::paint(QNanoPainter *painter)
{
    m_painter = painter;

    qreal w = width();
    qreal h = height();
    qreal t = m_animationTime;

    // These painting commands are identical with both renderers
    for (int i=0 ; i < m_testCount ; i++) {
        // Paint ruler
        if (m_enabledTests & 1) {
            drawRuler(0, h*0.02, w, h*0.05, t);
        }
        // Paint circles
        if (m_enabledTests & 2) {
            drawGraphCircles(w*0.15, h*0.1, w*0.7, w*0.7, 10, t*2);
            drawGraphCircles(w*0.05, h*0.55, w*0.25, w*0.25, 8, t*3);
            drawGraphCircles(w*0.70, h*0.55, w*0.25, w*0.25, 3, t);
        }
        // Paint lines
        if (m_enabledTests & 4) {
            drawGraphLine(0, h, w, -h, 4, t);
            drawGraphLine(0, h, w, -h*0.8, 6,  t+10);
            drawGraphLine(0, h, w, -h*0.6, 12, t/2);
        }
        // Paint bars
        if (m_enabledTests & 8) {
            drawGraphBars(0, h, w, -h*0.8, 6, t*3);
            drawGraphBars(0, h, w, -h*0.4, 10, t+2);
            drawGraphBars(0, h, w, -h*0.3, 20, t*2+2);
            drawGraphBars(0, h, w, -h*0.2, 40, t*3+2);
        }
        // Paint icons
        int icons = 50;
        if (m_enabledTests & 16) {
            drawIcons(0, h*0.2, w, h*0.2, icons, t);
        }

        // Increase animation time when m_testCount > 1
        t += 0.3;
    }
}

void DemoQNanoItemPainter::drawGraphLine(float x, float y, float w, float h, int items, float t)
{
    QVarLengthArray<float, 1024> samples(items);
    QVarLengthArray<float, 1024> sx(items); QVarLengthArray<float, 1024> sy(items);
    float dx = w/(items-1);
    float dotSize = w/50;
    int i;

    // Generate positions
    for (i = 0; i<items; i++) {
        samples[i] = 0.5 + sinf((i+1) * t * 0.2) * 0.1;
    }
    for (i = 0; i < items; i++) {
        sx[i] = x+i*dx;
        sy[i] = y+h*samples[i]*0.8f;
    }

    // Draw graph background area
    QNanoLinearGradient bg(x,y,x,y+h, m_color1, m_color2);
    m_painter->beginPath();
    m_painter->moveTo(sx[0], sy[0]);
    for (i = 1; i < items; i++)
        m_painter->bezierTo(sx[i-1]+dx*0.5f,sy[i-1], sx[i]-dx*0.5f,sy[i], sx[i],sy[i]);
    m_painter->lineTo(x+w, y);
    m_painter->lineTo(x, y);
    m_painter->setFillStyle(bg);
    m_painter->fill();

    // Draw graph line
    m_painter->beginPath();
    m_painter->moveTo(sx[0], sy[0]);
    for (i = 1; i < items; i++)
        m_painter->bezierTo(sx[i-1]+dx*0.5f,sy[i-1], sx[i]-dx*0.5f,sy[i], sx[i],sy[i]);
    m_painter->setStrokeStyle(m_colorGray);
    m_painter->setLineWidth(dotSize*0.4);
    m_painter->stroke();

    // Draw dot borders
    m_painter->beginPath();
    for (i = 0; i < items; i++)
        m_painter->circle(sx[i], sy[i], dotSize);
    m_painter->setFillStyle(m_colorBlack);
    m_painter->fill();

    // Draw dot content
    m_painter->beginPath();
    for (i = 0; i < items; i++)
        m_painter->circle(sx[i], sy[i], dotSize*0.6);
    m_painter->setFillStyle(m_colorWhite);
    m_painter->fill();
}

void DemoQNanoItemPainter::drawGraphBars(float x, float y, float w, float h, int items, float t) {

    QVarLengthArray<float, 1024> samples(items);
    QVarLengthArray<float, 1024> sx(items); QVarLengthArray<float, 1024> sy(items);
    float dx = w/(items-1);
    float barWidth = dx * 0.8;
    float margin = dx - barWidth;
    int i;

    // Generate positions
    for (i = 0; i<items; i++) {
        samples[i] = 0.5 + sinf(i*0.1+t)*0.5;
    }
    for (i = 0; i < items; i++) {
        sx[i] = x+i*dx + margin/2;
        sy[i] = h*samples[i];
    }

    // Draw graph bars
    m_painter->beginPath();
    for (i = 0; i < items; i++) {
        m_painter->rect((int)sx[i]+0.5, (int)y+1.5, (int)barWidth, (int)sy[i]);
    }
    qreal lineWidth = 0.5 + w * 0.002;
    m_painter->setLineWidth(lineWidth);
    m_painter->setLineJoin(QNanoPainter::JOIN_MITER);
    m_painter->setFillStyle(m_color3);
    m_painter->setStrokeStyle(m_colorBlack);
    m_painter->fill();
    m_painter->stroke();
}

void DemoQNanoItemPainter::drawGraphCircles(float x, float y, float w, float h, int items, float t)
{
    qreal barWidth = 0.3 * w/items;
    qreal lineMargin = 0.2 * barWidth;
    qreal showAnimationProgress = 0.1 + 0.4*sinf(t*0.8)+0.5;
    qreal lineWidth = barWidth*showAnimationProgress;

    qreal cx = x+w/2;
    qreal cy = y+h/2;
    qreal radius1 = w/2 - lineWidth;
    qreal pi = 3.1415926;
    qreal pi2 = pi*2;
    int i;

    // Setup values
    qreal a1 = -pi/2;
    QVarLengthArray<qreal, 1024> a0(items);
    for (i=0; i<items; i++) {
        a0[i] = -pi/2 + pi2*(((float)items-i)/items)*showAnimationProgress;
    }

    m_painter->setLineWidth(lineWidth);
    m_painter->setLineJoin(QNanoPainter::JOIN_ROUND);
    m_painter->setLineCap(QNanoPainter::CAP_ROUND);

    // Draw cicle backgrounds
    qreal r = radius1;
    QNanoColor c_background(215,215,215,50);
    m_painter->setStrokeStyle(c_background);
    for (int i=0 ; i<items ; i++) {
        m_painter->beginPath();
        m_painter->circle(cx, cy, r);
        m_painter->stroke();
        r -= (lineWidth + lineMargin);
    }

    // Draw circle bars
    r = radius1;
    for (int i=0 ; i<items ; i++) {
        m_painter->beginPath();
        m_painter->arc(cx, cy, r, a0[i], a1, QNanoPainter::WINDING_CCW);
        float s = (float)i/items;
        QNanoColor c(200-150*s, 200-50*s, 100+50*s, 255*showAnimationProgress);
        m_painter->setStrokeStyle(c);
        m_painter->stroke();
        r -= (lineWidth + lineMargin);
    }
}

void DemoQNanoItemPainter::drawIcons(float x, float y, float w, float h, int items, float t)
{
    // Note: Adjust font size to match QPainter sizing
    qreal fontSize = w/22.0 * 1.32 - 1;
    m_testFont.setPixelSize(fontSize);
    m_painter->setFont(m_testFont);
    m_painter->setFillStyle("#FFFFFF");
    m_painter->setTextAlign(QNanoPainter::ALIGN_CENTER);
    m_painter->setTextBaseline(QNanoPainter::BASELINE_MIDDLE);
    qreal size = w/12;
    for (int i=0 ; i<items ; i++) {
        qreal xp = x + (w-size)/items*i;
        qreal yp = y + h*0.5 + h * sinf((i+1) * t * 0.1) * 0.5;
        m_painter->drawImage(m_circleImage, QRectF(xp, yp, size, size));
        m_painter->fillText(QString::number(i+1), QRectF(xp, yp+size/2, size, size));
    }
}

void DemoQNanoItemPainter::drawRuler(float x, float y, float w, float h, float t)
{
    float posX = x + w*0.05;
    double space = w*0.03 + sinf(t)*w*0.02;
    m_painter->setTextAlign(QNanoPainter::ALIGN_CENTER);
    m_painter->setTextBaseline(QNanoPainter::BASELINE_MIDDLE);
    // Note: Adjust font size to match QPainter sizing
    qreal fontSize = w/35.0 * 1.32 - 1;
    m_testFont.setPixelSize(fontSize);
    m_painter->setFont(m_testFont);
    m_painter->setStrokeStyle("#E0E0E0");
    m_painter->setFillStyle("#E0E0B0");
    m_painter->beginPath();
    int i = 0;
    while (posX < w) {
        m_painter->moveTo(posX, y);
        float height = h*0.2;
        QPointF textPoint(posX, y+h);
        if (i%10==0) {
            height = h*0.5;
            m_painter->fillText(QString::number(i), textPoint);
        } else if (i%5==0) {
            height = h*0.3;
            if (space > w*0.02) m_painter->fillText(QString::number(i), textPoint);
        }
        m_painter->lineTo(posX, y+height);
        posX += space;
        i++;
    }
    m_painter->setLineWidth(1.0f);
    m_painter->stroke();
}

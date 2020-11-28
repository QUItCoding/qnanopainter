#include "demoqnanoitempainter.h"
#include "demoqnanoitem.h"
#include <math.h>
#include <QtMath>
#include <QVarLengthArray>
#include <algorithm>

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

    QNanoImage::ImageFlags imageFlags = {};
#ifndef Q_OS_WIN
    // Windows ANGLE seems to have issues with mipmaps, use those in all other platforms.
    // See: https://github.com/QUItCoding/qnanopainter/issues/19
    imageFlags |= QNanoImage::GENERATE_MIPMAPS;
#endif
    m_circleImage = QNanoImage(":/qml/images/circle.png", imageFlags);

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

    float w = width();
    float h = height();
    float s = std::min(w, h);
    float t = m_animationTime;

    // These painting commands are identical with both renderers
    for (int i=0 ; i < m_testCount ; i++) {
        // Paint ruler
        if (m_enabledTests & 1) {
            drawRuler(0, h*0.02f, w, h*0.05f, t);
        }
        // Paint circles
        if (m_enabledTests & 2) {
            float bigCircle = 80.0f + s*0.5f;
            float smallCircle = 40.0f + s*0.2f;
            drawGraphCircles(w/2-bigCircle/2, h*0.1f, bigCircle, bigCircle, 10, t*2);
            drawGraphCircles(w*0.05, h*0.55f, smallCircle, smallCircle, 8, t*3);
            drawGraphCircles(w-smallCircle-w*0.05, h*0.55f, smallCircle, smallCircle, 3, t);
        }
        // Paint lines
        if (m_enabledTests & 4) {
            drawGraphLine(0, h, w, -h, 4, t);
            drawGraphLine(0, h, w, -h*0.8f, 6,  t+10);
            drawGraphLine(0, h, w, -h*0.6f, 12, t/2);
        }
        // Paint bars
        if (m_enabledTests & 8) {
            drawGraphBars(0, h, w, -h*0.8f, 6, t*3);
            drawGraphBars(0, h, w, -h*0.4f, 10, t+2);
            drawGraphBars(0, h, w, -h*0.3f, 20, t*2+2);
            drawGraphBars(0, h, w, -h*0.2f, 40, t*3+2);
        }
        // Paint icons
        int icons = 50;
        if (m_enabledTests & 16) {
            drawIcons(0, h*0.2f, w, h*0.2f, icons, t);
        }

        // Paint flower
        if (m_enabledTests & 32) {
            float flowerSize = 80.0f + s*0.6f;
            drawFlower(w/2-flowerSize/2, h-flowerSize, flowerSize, flowerSize, t);
        }

        // Increase animation time when m_testCount > 1
        t += 0.3f;
    }
}

void DemoQNanoItemPainter::drawGraphLine(float x, float y, float w, float h, int items, float t)
{
    QVarLengthArray<float, 1024> samples(items);
    QVarLengthArray<float, 1024> sx(items); QVarLengthArray<float, 1024> sy(items);
    float dx = w/(items-1);
    float dotSize = 4.0f + w*0.005;
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
    m_painter->setLineWidth(1.0f+dotSize*0.2f);
    m_painter->stroke();

    // Draw dots
    m_painter->beginPath();
    for (i = 0; i < items; i++)
        m_painter->circle(sx[i], sy[i], dotSize*0.8);
    m_painter->setLineWidth(dotSize*0.2);
    m_painter->setStrokeStyle(m_colorBlack);
    m_painter->setFillStyle(m_colorWhite);
    m_painter->fill();
    m_painter->stroke();
}

void DemoQNanoItemPainter::drawGraphBars(float x, float y, float w, float h, int items, float t) {

    QVarLengthArray<float, 1024> samples(items);
    QVarLengthArray<float, 1024> sx(items); QVarLengthArray<float, 1024> sy(items);
    float dx = w/items;
    float barWidth = dx * 0.8f;
    float margin = dx - barWidth;
    int i;

    // Generate positions
    for (i = 0; i<items; i++) {
        samples[i] = 0.5f + sinf(i*0.1f+t)*0.5f;
    }
    for (i = 0; i < items; i++) {
        sx[i] = x+i*dx + margin/2;
        sy[i] = h*samples[i];
    }

    // Draw graph bars
    m_painter->beginPath();
    for (i = 0; i < items; i++) {
        m_painter->rect((int)sx[i]+0.5f, (int)y+1.5f, (int)barWidth, (int)sy[i]);
    }
    int lineWidth = 1.0f;
    m_painter->setLineWidth(lineWidth);
    m_painter->setLineJoin(QNanoPainter::JOIN_MITER);
    m_painter->setFillStyle(m_color3);
    m_painter->setStrokeStyle(m_colorBlack);
    m_painter->fill();
    m_painter->stroke();
}

void DemoQNanoItemPainter::drawGraphCircles(float x, float y, float w, float h, int items, float t)
{
    float barWidth = 0.3f * w/items;
    float lineMargin = 0.2f * barWidth;
    float showAnimationProgress = 0.1f + 0.4f*sinf(t*0.8f)+0.5f;
    float lineWidth = barWidth*showAnimationProgress;

    float cx = x+w/2;
    float cy = y+h/2;
    float radius1 = w/2 - lineWidth;
    int i;

    // Setup values
    float a1 = -(float)M_PI/2;
    QVarLengthArray<float, 1024> a0(items);
    for (i=0; i<items; i++) {
        a0[i] = -M_PI/2 + 2*M_PI*(((float)items-i)/items)*showAnimationProgress;
    }

    m_painter->setLineWidth(lineWidth);
    m_painter->setLineJoin(QNanoPainter::JOIN_ROUND);
    m_painter->setLineCap(QNanoPainter::CAP_ROUND);

    // Draw cicle backgrounds
    float r = radius1;
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
    float s = std::min(width(), height());
    float size = 16.0f + s*0.05f;
    // Note: Adjust font size to match QPainter sizing
    float fontSize = size * 0.5f * 1.32f;
    m_testFont.setPixelSize(fontSize);
    m_painter->setFont(m_testFont);
    m_painter->setFillStyle("#FFFFFF");
    m_painter->setTextAlign(QNanoPainter::ALIGN_CENTER);
    m_painter->setTextBaseline(QNanoPainter::BASELINE_MIDDLE);
    for (int i=0 ; i<items ; i++) {
        float xp = x + (w-size)/items*i;
        float yp = y + h*0.5f + h * sinf((i+1) * t * 0.1) * 0.5f;
        m_painter->drawImage(m_circleImage, QRectF(xp, yp, size, size));
        m_painter->fillText(QString::number(i+1), QRectF(xp, yp+size/2, size, size));
    }
}

void DemoQNanoItemPainter::drawRuler(float x, float y, float w, float h, float t)
{
    float posX = x + w*0.05f;
    double space = w*0.03f + sinf(t)*w*0.02f;
    m_painter->setTextAlign(QNanoPainter::ALIGN_CENTER);
    m_painter->setTextBaseline(QNanoPainter::BASELINE_MIDDLE);
    // Note: Adjust font size to match QPainter sizing
    float fontSize = (10.0f + w*0.01f) * 1.32f;
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

float DemoQNanoItemPainter::_flowerPos(int i) {
    const int items = 12;
    return (2*M_PI) * (1 - (float)i/items) - M_PI/2;
}

void DemoQNanoItemPainter::drawFlower(float x, float y, float w, float h, float t)
{
    const float cx = x + w/2;
    const float cy = y + h/2;
    const float leafSize = w/2;
    m_painter->save(); // Required to prevent rotation multiply

    m_painter->translate(cx, cy);
    m_painter->rotate(sinf(t)*20 * M_PI/180);
    m_painter->translate(-cx, -cy);

    m_painter->setStrokeStyle("#40000000");
    m_painter->setLineWidth(4);
    QNanoRadialGradient gradient1(cx, cy, leafSize);
    QNanoColor startColor((0.5 + sinf(t*2)*0.5)*255, 0, (0.5 + sinf(t+M_PI)*0.5)*255);
    gradient1.setStartColor(startColor);
    gradient1.setEndColor("#ffffff");
    m_painter->setFillStyle(gradient1);

    m_painter->beginPath();
    m_painter->moveTo(cx, cy);
    const int items = 12;
    for (int i=0; i<items; i+=2) {
        m_painter->quadTo(cx + cosf(_flowerPos(i))*leafSize, cy + sinf(_flowerPos(i))*leafSize, cx + cosf(_flowerPos(i+1))*leafSize, cy + sinf(_flowerPos(i+1))*leafSize);
        m_painter->quadTo(cx + cosf(_flowerPos(i+2))*leafSize, cy + sinf(_flowerPos(i+2))*leafSize, cx, cy);
    }
    m_painter->fill();
    m_painter->stroke();

    m_painter->setFillStyle("#ffffff");
    m_painter->beginPath();
    m_painter->circle(cx, cy, 0.1*w);
    m_painter->fill();
    m_painter->restore();
}

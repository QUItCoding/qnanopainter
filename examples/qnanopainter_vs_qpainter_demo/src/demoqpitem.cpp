#include "demoqpitem.h"
#include <QPainter>
#include <QPainterPath>
#include <QPen>
#include <QPointF>
#include <QFontDatabase>
#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>

DemoQPItem::DemoQPItem(QQuickItem *parent)
    : QQuickPaintedItem(parent)
    , m_animationTime(0.0)
    , m_enabledTests(0)
    , m_testCount(1)
    , m_qpAntialiasing(true)
    , m_qpRenderTargetFBO(false)
{
    // Setup colors
    m_colorWhite = QColor(255,255,255,255);
    m_colorGray = QColor(180,180,180,255);
    m_colorBlack = QColor(0,0,0,255);
    m_color1 = QColor(180,190,40,20);
    m_color2 = QColor(255,255,255,150);
    m_color3 = QColor(255,255,255,80);

    m_circleImage = QImage(":/qml/images/circle.png");

    // Load custom font
    QStringList s = QFontDatabase::applicationFontFamilies(g_customFontId);
    if (!s.isEmpty()) {
        m_testFont = QFont(s.first());
    }

    // Set antialiasing and render target
    setAntialiasing(m_qpAntialiasing);
    setRenderTarget(m_qpRenderTargetFBO ? QQuickPaintedItem::FramebufferObject : QQuickPaintedItem::Image);
    // Enable this to try FastFBOResizing
    //setPerformanceHint(QQuickPaintedItem::FastFBOResizing, true);
}

void DemoQPItem::paint(QPainter *painter)
{
    m_painter = painter;

    float w = boundingRect().width();
    float h = boundingRect().height();
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

void DemoQPItem::drawGraphLine(float x, float y, float w, float h, int items, float t)
{
    QVarLengthArray<float, 1024> samples(items);
    QVarLengthArray<float, 1024> sx(items); QVarLengthArray<float, 1024> sy(items);
    float dx = w/(items-1);
    float dotSize = 4.0f + w*0.005;
    int i;

    // Generate positions
    for (i = 0; i<items; i++) {
        samples[i] = 0.5f + sinf((i+1) * t * 0.2f) * 0.1f;
    }
    for (i = 0; i < items; i++) {
        sx[i] = x+i*dx;
        sy[i] = y+h*samples[i]*0.8f;
    }

    // Draw graph background area
    QLinearGradient bg = QLinearGradient(x, y, x, y+h);
    bg.setColorAt(0.0, m_color1);
    bg.setColorAt(1.0, m_color2);
    QPainterPath path;
    path.moveTo(sx[0], sy[0]);
    for (i = 1; i < items; i++)
        path.cubicTo(sx[i-1]+dx*0.5f,sy[i-1], sx[i]-dx*0.5f,sy[i], sx[i],sy[i]);
    path.lineTo(x+w, y);
    path.lineTo(x, y);
    m_painter->fillPath(path, bg);

    // Draw graph line
    QPainterPath path2;
    path2.moveTo(sx[0], sy[0]);
    for (i = 1; i < items; i++)
            path2.cubicTo(sx[i-1]+dx*0.5f,sy[i-1], sx[i]-dx*0.5f,sy[i], sx[i],sy[i]);
    m_painter->strokePath(path2, QPen(m_colorGray, 1.0f+dotSize*0.2f));

    // Draw dots
    m_painter->setBrush(m_colorWhite);
    m_painter->setPen(QPen(m_colorBlack, dotSize*0.2));
    for (i = 0; i < items; i++)
        m_painter->drawEllipse(QPointF(sx[i], sy[i]), dotSize*0.8, dotSize*0.8);
}

void DemoQPItem::drawGraphBars(float x, float y, float w, float h, int items, float t) {
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
    m_painter->setBrush(m_color3);
    float lineWidth = 1.0f;
    QPen pen(m_colorBlack, lineWidth);
    pen.setJoinStyle(Qt::MiterJoin);
    m_painter->setPen(pen);
    // TODO: Consider drawRects() instead if it's faster?
    for (i = 0; i < items; i++) {
        m_painter->drawRect((int)sx[i]+0.5, (int)y+2.5, (int)barWidth, (int)sy[i]);
    }
}

void DemoQPItem::drawGraphCircles(float x, float y, float w, float h, int items, float t)
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
    int a1 = 90*16;
    QVarLengthArray<int, 1024> a0(items);
    for (i=0; i<items; i++) {
        a0[i] = -360*16*(((float)items-i)/items)*showAnimationProgress;
    }

    // Draw cicle backgrounds
    float r = radius1;
    QColor c_background = QColor(215,215,215,50);
    m_painter->setPen(QPen(c_background, lineWidth));
    m_painter->setBrush(Qt::NoBrush);
    for (int i=0 ; i<items ; i++) {
        m_painter->drawEllipse(QPointF(cx, cy), r, r);
        r -= (lineWidth + lineMargin);
    }

    // Draw circle bars
    r = radius1;
    float r2 = radius1;
    QPen pen(c_background, lineWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    float margin = 0;
    for (int i=0 ; i<items ; i++) {
        float s = (float)i/items;
        QColor c(200-150*s,200-50*s,100+50*s,255*showAnimationProgress);
        pen.setColor(c);
        m_painter->setPen(pen);
        QRectF area(cx-r, cy-r, cx-x+r2-lineWidth, cy-y+r2-lineWidth);
        m_painter->drawArc(area, a1, a0[i]);
        margin -= (lineWidth + lineMargin);
        r -= (lineWidth + lineMargin);
        r2 -= 2*(lineWidth + lineMargin);
    }
}

void DemoQPItem::drawIcons(float x, float y, float w, float h, int items, float t)
{
    float s = std::min(width(), height());
    float size = 16.0f + s*0.05f;
    float fontSize = size * 0.5f;
    m_testFont.setPixelSize(fontSize);
    m_painter->setFont(m_testFont);
    m_painter->setPen("#FFFFFF");
    m_painter->setBrush(Qt::NoBrush);
    for (int i=0 ; i<items ; i++) {
        float xp = x + (w-size)/items*i;
        float yp = y + h*0.5f + h * sinf((i+1) * t * 0.1f) * 0.5f;
        m_painter->drawImage(QRectF(xp, yp, size, size), m_circleImage);
        m_painter->drawText(QRectF(xp, yp, size, size), Qt::AlignHCenter|Qt::AlignCenter, QString::number(i+1));
    }
}

void DemoQPItem::drawRuler(float x, float y, float w, float h, float t)
{
    float posX = x + w*0.05f;
    double space = w*0.03f + sinf(t)*w*0.02f;
    float fontSize = 10.0f + w*0.01f;
    m_testFont.setPixelSize(fontSize);
    m_painter->setFont(m_testFont);
    m_painter->setPen("#E0E0B0");
    m_painter->setBrush(Qt::NoBrush);

    int i = 0;
    QVector<QPointF> pointPairs;
    while (posX < w) {
        pointPairs.append(QPointF(posX, y));
        float height = h*0.2f;
        float ts = w*0.05f;
        QRectF textRect(posX-ts, y+h-ts, ts*2, ts*2);
        if (i%10==0) {
            height = h*0.5f;
            m_painter->drawText(textRect, Qt::AlignHCenter|Qt::AlignCenter, QString::number(i));
        } else if (i%5==0) {
            height = h*0.3f;
            if (space > w*0.02f) m_painter->drawText(textRect, Qt::AlignHCenter|Qt::AlignCenter, QString::number(i));
        }
        pointPairs.append(QPointF(posX, y+height));
        posX += space;
        i++;
    }
    QPen pen(QColor("#E0E0E0"), 1);
    m_painter->setPen(pen);
    m_painter->drawLines(pointPairs);
}

float DemoQPItem::_flowerPos(int i) {
    const int items = 12;
    return (2*M_PI) * (1 - (float)i/items) - M_PI/2;
}


void DemoQPItem::drawFlower(float x, float y, float w, float h, float t)
{
    const float cx = x + w/2;
    const float cy = y + h/2;
    const float leafSize = w/2;

    m_painter->save(); // Required to prevent rotation multiply

    m_painter->translate(cx, cy);
    m_painter->rotate(sinf(t)*20);
    m_painter->translate(-cx, -cy);

    m_painter->setPen(QPen(QColor("#40000000"), 4));
    QRadialGradient gradient1(cx, cy, leafSize);
    QColor startColor((0.5f + sinf(t*2)*0.5f)*255, 0, (0.5f + sinf(t+M_PI)*0.5f)*255);
    gradient1.setColorAt(0, startColor);
    gradient1.setColorAt(1, QColor("#ffffff"));
    m_painter->setBrush(gradient1);

    QPainterPath path;
    path.moveTo(cx, cy);
    const int items = 12;
    for (int i=0; i<items; i+=2) {
        path.quadTo(cx + cosf(_flowerPos(i))*leafSize, cy + sinf(_flowerPos(i))*leafSize, cx + cosf(_flowerPos(i+1))*leafSize, cy + sinf(_flowerPos(i+1))*leafSize);
        path.quadTo(cx + cosf(_flowerPos(i+2))*leafSize, cy + sinf(_flowerPos(i+2))*leafSize, cx, cy);
    }
    m_painter->drawPath(path);

    m_painter->setPen(Qt::NoPen);
    m_painter->setBrush(QColor("#ffffff"));
    m_painter->drawEllipse(QPointF(cx, cy), 0.1f*w, 0.1f*w);
    m_painter->restore();

}

#include "demoqpitem.h"
#include <QPainter>
#include <QPen>
#include <QPointF>
#include <QFontDatabase>
#include <math.h>

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
    int id = QFontDatabase::addApplicationFont(":/qml/fonts/Roboto-Regular.ttf");
    QStringList s = QFontDatabase::applicationFontFamilies(id);
    m_testFont = QFont(s.at(0));

    // Set antialiasing and render target
    setAntialiasing(m_qpAntialiasing);
    setRenderTarget(m_qpRenderTargetFBO ? QQuickPaintedItem::FramebufferObject : QQuickPaintedItem::Image);
    // Enable this to try FastFBOResizing
    //setPerformanceHint(QQuickPaintedItem::FastFBOResizing, true);
}

void DemoQPItem::paint(QPainter *painter)
{
    m_painter = painter;

    qreal w = boundingRect().width();
    qreal h = boundingRect().height();
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

void DemoQPItem::drawGraphLine(float x, float y, float w, float h, int items, float t)
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
    m_painter->strokePath(path2, QPen(m_colorGray, dotSize*0.4));

    // Draw dot borders
    m_painter->setBrush(m_colorBlack);
    m_painter->setPen(Qt::NoPen);
    for (i = 0; i < items; i++)
        m_painter->drawEllipse(QPointF(sx[i], sy[i]), dotSize, dotSize);

    // Draw dot content
    m_painter->setBrush(m_colorWhite);
    for (i = 0; i < items; i++)
        m_painter->drawEllipse(QPointF(sx[i], sy[i]), dotSize*0.6, dotSize*0.6);
}

void DemoQPItem::drawGraphBars(float x, float y, float w, float h, int items, float t) {
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
    m_painter->setBrush(m_color3);
    qreal lineWidth = 0.5 + w * 0.002;
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
    qreal barWidth = 0.3 * w/items;
    qreal lineMargin = 0.2 * barWidth;
    qreal showAnimationProgress = 0.1 + 0.4*sinf(t*0.8)+0.5;
    qreal lineWidth = barWidth*showAnimationProgress;

    qreal cx = x+w/2;
    qreal cy = y+h/2;
    qreal radius1 = w/2 - lineWidth;
    int i;

    // Setup values
    qreal a1 = 90*16;
    QVarLengthArray<qreal, 1024> a0(items);
    for (i=0; i<items; i++) {
        a0[i] = -360*16*(((float)items-i)/items)*showAnimationProgress;
    }

    // Draw cicle backgrounds
    qreal r = radius1;
    QColor c_background = QColor(215,215,215,50);
    m_painter->setPen(QPen(c_background, lineWidth));
    m_painter->setBrush(Qt::NoBrush);
    for (int i=0 ; i<items ; i++) {
        m_painter->drawEllipse(QPointF(cx, cy), r, r);
        r -= (lineWidth + lineMargin);
    }

    // Draw circle bars
    r = radius1;
    qreal r2 = radius1;
    QPen pen(c_background, lineWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    qreal margin = 0;
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
    qreal fontSize = w/22.0;
    m_testFont.setPixelSize(fontSize);
    m_painter->setFont(m_testFont);
    m_painter->setPen("#FFFFFF");
    m_painter->setBrush(Qt::NoBrush);
    qreal size = w/12;
    for (int i=0 ; i<items ; i++) {
        qreal xp = x + (w-size)/items*i;
        qreal yp = y + h*0.5 + h * sinf((i+1) * t * 0.1) * 0.5;
        m_painter->drawImage(QRectF(xp, yp, size, size), m_circleImage);
        m_painter->drawText(QRectF(xp, yp, size, size), Qt::AlignHCenter|Qt::AlignCenter, QString::number(i+1));
    }
}

void DemoQPItem::drawRuler(float x, float y, float w, float h, float t)
{
    float posX = x + w*0.05;
    double space = w*0.03 + sinf(t)*w*0.02;
    m_testFont.setPixelSize(w/35.0);
    m_painter->setFont(m_testFont);
    m_painter->setPen("#E0E0B0");
    m_painter->setBrush(Qt::NoBrush);

    int i = 0;
    QVector<QPointF> pointPairs;
    while (posX < w) {
        pointPairs.append(QPointF(posX, y));
        float height = h*0.2;
        float ts = w*0.05;
        QRectF textRect(posX-ts, y+h-ts, ts*2, ts*2);
        if (i%10==0) {
            height = h*0.5;
            m_painter->drawText(textRect, Qt::AlignHCenter|Qt::AlignCenter, QString::number(i));
        } else if (i%5==0) {
            height = h*0.3;
            if (space > w*0.02) m_painter->drawText(textRect, Qt::AlignHCenter|Qt::AlignCenter, QString::number(i));
        }
        pointPairs.append(QPointF(posX, y+height));
        posX += space;
        i++;
    }
    QPen pen(QColor("#E0E0E0"), 1);
    m_painter->setPen(pen);
    m_painter->drawLines(pointPairs);
}

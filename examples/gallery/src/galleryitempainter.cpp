#include "galleryitempainter.h"
#include "galleryitem.h"
#include "qnanoradialgradient.h"
#include "qnanoboxgradient.h"
#include <math.h>

GalleryItemPainter::GalleryItemPainter()
    : m_viewIndex(0)
{
    m_patternImage = QNanoImage(":/images/pattern1.png", QNanoImage::REPEATX | QNanoImage::REPEATY);
    m_patternImage2 = QNanoImage(":/images/pattern2.png", QNanoImage::REPEATX | QNanoImage::REPEATY);
    m_testImage = QNanoImage(":/images/quit_logo2.png");
}

GalleryItemPainter::~GalleryItemPainter()
{
}

void GalleryItemPainter::synchronize(QNanoQuickItem *item)
{
    // Setting values here synchronized
    GalleryItem *realItem = static_cast<GalleryItem*>(item);
    if (realItem) {
        m_animationTime = realItem->animationTime();
        m_animationSine = realItem->animationSine();
        m_viewIndex = realItem->galleryView();
    }
}

void GalleryItemPainter::paint()
{
    // Draw similarly colored rectangles
    switch (m_viewIndex) {
    case 0:
        drawRectsWithSameColor();
        drawRectsWithMix();
        drawRectsWithLinearGradient();
        drawRectsWithRadialGradient();
        drawRectsWithBoxGradient();
        drawRectsWithImagePattern();
        break;
    case 1:
        drawPaths();
        break;
    case 2:
        drawTransforms();
        break;
    case 3:
        drawAntialiasing();
        break;
    case 4:
        drawTextsFonts();
        break;
    case 5:
        drawTextsAlignments();
        break;
    case 6:
        drawImages();
        break;
    default:
        break;
    }
}

void GalleryItemPainter::drawRectsWithSameColor() {

    int rects = 7;
    qreal margin = width()*0.02;
    qreal w = width() / (rects+1) - margin;
    qreal posX = w/2 + margin;
    qreal posY = w/2;

    painter()->setStrokeStyle(QNanoColor(0,0,0));
    painter()->setFillStyle(QNanoColor(204,102,51,102));
    drawRect(posX,posY,w,w);
    posX += w + margin;

    painter()->setStrokeStyle("#000000");
    painter()->setFillStyle(0x66CC6633);
    drawRect(posX,posY,w,w);
    posX += w + margin;

    QNanoColor c1; // Note: Using default
    QNanoColor c2;
    c2.setRed(204);
    c2.setGreen(102);
    c2.setBlue(51);
    c2.setAlpha(102);
    painter()->setStrokeStyle(c1);
    painter()->setFillStyle(c2);
    drawRect(posX,posY,w,w);
    posX += w + margin;

    QNanoColor cf1; // Note: Using default
    QNanoColor cf2;
    cf2.setRedF(0.8);
    cf2.setGreenF(0.4);
    cf2.setBlueF(0.2);
    cf2.setAlphaF(0.4);
    painter()->setStrokeStyle(cf1);
    painter()->setFillStyle(cf2);
    drawRect(posX,posY,w,w);
    posX += w + margin;

    QColor qc1(0,0,0,255);
    QColor qc2(204,102,51,102);
    painter()->setStrokeStyle(QNanoColor::fromQColor(qc1));
    painter()->setFillStyle(QNanoColor::fromQColor(qc2));
    drawRect(posX,posY,w,w);
    posX += w + margin;

    painter()->setStrokeStyle(QNanoColor::fromHSLA(0, 0, 0, 255));
    // HSL: 20° 60% 50%
    painter()->setFillStyle(QNanoColor::fromHSLA(20, 255*0.6, 255*0.5, 102));
    drawRect(posX,posY,w,w);
    posX += w + margin;

    QNanoColor cc1(c1);
    QNanoColor cc2(c2);
    painter()->setStrokeStyle(cc1);
    painter()->setFillStyle(cc2);
    drawRect(posX,posY,w,w);
}

void GalleryItemPainter::drawRectsWithMix() {
    int rects = 3;
    qreal margin = width()*0.02;
    qreal w = width() / (rects+2) - margin;
    qreal posX = w + margin;
    qreal posY = w+margin;

    QNanoColor c1 = QNanoColor::fromMix(QNanoColor(255,0,0,255), QNanoColor(0,0,0,255), m_animationSine);
    QNanoColor c2 = QNanoColor::fromMix(QNanoColor(255,0,0,100), QNanoColor(0,0,0,0), m_animationSine);
    painter()->setStrokeStyle(c1);
    painter()->setFillStyle(c2);
    drawRect(posX,posY,w,w);
    posX += w + margin;

    QNanoColor c12 = QNanoColor::fromMix(QNanoColor(0,255,0,255), QNanoColor(0,0,0,255), m_animationSine);
    QNanoColor c22 = QNanoColor::fromMix(QNanoColor(0,255,0,100), QNanoColor(0,0,0,0), m_animationSine);
    painter()->setStrokeStyle(c12);
    painter()->setFillStyle(c22);
    drawRect(posX,posY,w,w);
    posX += w + margin;

    QNanoColor c23 = QNanoColor::fromMix(QNanoColor(0,0,255,255), QNanoColor(0,0,255,0), m_animationSine);
    painter()->setStrokeStyle(QNanoColor(255,255,255));
    painter()->setFillStyle(c23);
    drawRect(posX,posY,w,w);
    posX += w + margin;
}

void GalleryItemPainter::drawRectsWithLinearGradient() {
    int rects = 3;
    qreal margin = width()*0.02;
    qreal w = width() / (rects+2) - margin;
    qreal posX = w + margin;
    qreal posY = 2*(w+margin);

    painter()->setStrokeStyle(QNanoColor(255,255,255));
    QRectF rect1(posX,posY,w,w);
    painter()->setFillStyle(QNanoLinearGradient(0, rect1.y(), 0, rect1.y()+rect1.height()));
    painter()->fillRect(rect1);
    posX += w + margin;

    QNanoLinearGradient g1(0, 0, 0, 0, QNanoColor(0,255,0,255), QNanoColor(255,0,0,255));
    g1.setStartPosition(posX + (w*0.4 * m_animationSine), 0);
    g1.setEndPosition(posX + w - (w*0.4 * m_animationSine), 0);

    painter()->setStrokeStyle(QNanoColor(255,255,255));
    painter()->setFillStyle(g1);
    painter()->fillRect(posX,posY,w,w);
    posX += w + margin;

    QRectF rect2(posX,2*(w+margin),w,w);
    QNanoLinearGradient g2(rect2.x(), rect2.y(), rect2.x()+rect2.width(), rect2.y()+rect2.height());
    g2.setStartColor(QNanoColor(m_animationSine*255, 255, 255));
    g2.setEndColor(QNanoColor(255, m_animationSine*255, 255, 255-m_animationSine*255));
    painter()->setFillStyle(g2);
    painter()->fillRect(rect2);
}

void GalleryItemPainter::drawRectsWithRadialGradient() {
    int rects = 3;
    qreal margin = width()*0.02;
    qreal w = width() / (rects+2) - margin;
    qreal posX = w + margin;
    qreal posY = 3*(w+margin);

    QRectF rect1(posX,posY,w,w);
    painter()->setFillStyle(QNanoRadialGradient(rect1.x(), rect1.y(), rect1.width()));
    painter()->fillRect(rect1);
    posX += w + margin;

    QRectF rect2(posX,posY,w,w);
    QNanoRadialGradient g1;
    g1.setStartColor(QNanoColor(0,255,0,255));
    g1.setEndColor(QNanoColor(255,0,0,255));
    g1.setCenterPosition(rect2.x() + rect2.width()/2, rect2.y() + rect2.height()/2);
    g1.setOuterRadius(0.5*w);
    g1.setInnerRadius(0.4*w*m_animationSine);

    painter()->setStrokeStyle(QNanoColor(255,255,255));
    painter()->setFillStyle(g1);
    painter()->fillRect(rect2);
    posX += w + margin;

    QRectF rect3(posX,3*(w+margin),w,w);
    QNanoRadialGradient g3(rect3.x() + rect3.width()/2, rect3.y() + rect3.height()/2, w/2, w/4);
    g3.setStartColor(QNanoColor(m_animationSine*255, 255, 255));
    g3.setEndColor(QNanoColor(255, m_animationSine*255, 255, 255-m_animationSine*255));
    painter()->setFillStyle(g3);
    painter()->fillRect(rect3);
}

void GalleryItemPainter::drawRectsWithBoxGradient() {
    int rects = 3;
    qreal margin = width()*0.02;
    qreal w = width() / (rects+2) - margin;
    qreal posX = w + margin;
    qreal posY = 4*(w+margin);

    QRectF rect1(posX,posY,w,w);
    painter()->setFillStyle(QNanoBoxGradient(rect1, w/2));
    painter()->fillRect(rect1);
    posX += w + margin;

    QRectF rect2(posX,posY,w,w);
    QNanoBoxGradient g1;
    g1.setStartColor(QNanoColor(0,255,0,255));
    g1.setEndColor(QNanoColor(255,0,0,255));
    g1.setRect(rect2);
    g1.setRadius(w/8);
    g1.setFeather(w/2 - m_animationSine*w/3);

    painter()->setStrokeStyle(QNanoColor(255,255,255));
    painter()->setFillStyle(g1);
    painter()->fillRect(rect2);
    posX += w + margin;

    QRectF rect3(posX,posY,w,w);
    QNanoBoxGradient g3(rect3, w/4, w/8);
    g3.setStartColor(QNanoColor(m_animationSine*255, 255, 255));
    g3.setEndColor(QNanoColor(255, m_animationSine*255, 255, 255-m_animationSine*255));
    painter()->setFillStyle(g3);
    painter()->fillRect(rect3);
}

void GalleryItemPainter::drawRectsWithImagePattern() {
    int rects = 3;
    qreal margin = width()*0.02;
    qreal w = width() / (rects+2) - margin;
    qreal posX = w + margin;
    qreal posY = 5*(w+margin);

    QRectF rect1(posX,posY,w,w);
    painter()->setFillStyle(QNanoImagePattern(m_patternImage2));
    painter()->fillRect(rect1);
    posX += w + margin;

    QRectF rect2(posX,posY,w,w);
    QNanoImagePattern g1;
    g1.setImage(m_patternImage);
    g1.setStartPosition(rect2.x()+w/2, rect2.y()+w/2);
    g1.setImageSize(w/5, w/3);
    g1.setRotation(m_animationTime);
    g1.setAlpha(0.5 + m_animationSine * 0.5);
    painter()->setFillStyle(g1);
    painter()->fillRect(rect2);
    posX += w + margin;

    QRectF rect3(posX,posY,w,w);
    QNanoImagePattern g3(m_patternImage);
    g3.setStartPosition(rect3.x(), rect3.y());
    g3.setImageSize(w*0.1 + m_animationSine * w*0.2, w*0.15 + m_animationSine * w*0.3);
    painter()->setFillStyle(g3);
    painter()->fillRect(rect3);
}

void GalleryItemPainter::drawRect(float x, float y, float w, float h) {
    painter()->beginPath();
    painter()->rect(x, y, w, h);
    painter()->setLineWidth(w*0.05);
    painter()->fill();
    painter()->stroke();
}

void GalleryItemPainter::drawPaths() {
    int rects = 3;
    qreal margin = width()*0.1;
    qreal w = width() / (rects) - margin;
    qreal posX = margin/2;
    qreal posY = margin;
    qreal lineWidth = width()*0.02;

    painter()->setFillStyle(0xFFE0E0E0);
    painter()->setStrokeStyle(0xFFE00000);
    painter()->setLineWidth(lineWidth);
    painter()->setLineCap(QNanoPainter::CAP_ROUND);
    painter()->setLineJoin(QNanoPainter::JOIN_ROUND);

    painter()->beginPath();
    painter()->rect(posX, posY, w, w);
    posX += w + margin;
    painter()->roundedRect(posX, posY, w, w, w/2*m_animationSine);
    posX += w + margin;
    painter()->circle(posX+w/2, posY+w/2, w/2);

    posY += w+margin;
    posX = margin/2;
    painter()->ellipse(posX+w/2, posY+w/2, w/3+w/4*m_animationSine, w/3+w/4*(1-m_animationSine));
    posX += w + margin;
    painter()->moveTo(posX+w/2, posY+w/2);
    painter()->arc(posX+w/2, posY+w/2, w/2, 0, 0.5 + m_animationSine*5);
    painter()->closePath();
    posX += w + margin;
    QRectF r(posX, posY, w, w);
    painter()->ellipse(r);
    qreal a = w*0.1*m_animationSine;
    qreal a2 = w*0.1*(1-m_animationSine);
    r.adjust(w*0.1+a, w*0.1+a2, -w*0.1-a, -w*0.1-a2);
    painter()->beginHoleSubPath();
    painter()->ellipse(r);
    r.adjust(w*0.2, w*0.2, -w*0.2, -w*0.2);
    painter()->beginSolidSubPath();
    painter()->rect(r);

    painter()->stroke();
    painter()->fill();

    posY += w+margin;
    posX = margin/2;
    painter()->beginPath();
    painter()->moveTo(posX, posY);
    painter()->bezierTo(posX+w*0.2*m_animationSine, posY+w*0.8*m_animationSine, posX+w*0.8*m_animationSine, posY+w*0.2*m_animationSine, posX+w, posY+w);
    posX += w + margin;
    painter()->moveTo(posX, posY);
    painter()->quadTo(posX+w*0.8*m_animationSine, posY+w*0.2*m_animationSine, posX+w, posY+w);
    posX += w + margin;
    painter()->moveTo(posX, posY);
    painter()->arcTo(posX, posY+w, posX+w, posY+w, w*m_animationSine);

    painter()->stroke();

    posY += w+margin;
    posX = margin/2;
    painter()->setLineCap(QNanoPainter::CAP_ROUND);
    painter()->setLineJoin(QNanoPainter::JOIN_ROUND);
    painter()->beginPath();
    painter()->moveTo(posX, posY);
    painter()->lineTo(posX, posY+w);
    painter()->lineTo(posX+w*m_animationSine, posY);
    painter()->stroke();
    posX += w + margin;
    painter()->setLineCap(QNanoPainter::CAP_BUTT);
    painter()->setLineJoin(QNanoPainter::JOIN_MITER);
    painter()->beginPath();
    painter()->moveTo(posX, posY);
    painter()->lineTo(posX, posY+w);
    painter()->lineTo(posX+w*m_animationSine, posY);
    painter()->stroke();
    posX += w + margin;
    painter()->setLineCap(QNanoPainter::CAP_SQUARE);
    painter()->setLineJoin(QNanoPainter::JOIN_BEVEL);
    painter()->beginPath();
    painter()->moveTo(posX, posY);
    painter()->lineTo(posX, posY+w);
    painter()->lineTo(posX+w*m_animationSine, posY);
    painter()->stroke();
}

void GalleryItemPainter::drawTransforms() {
    int rects = 3;
    qreal margin = width()*0.1;
    qreal w = width() / (rects) - margin;
    qreal posX = margin/2;
    qreal posY = margin;
    qreal lineWidth = width()*0.01;

    painter()->setLineWidth(lineWidth);
    painter()->fillRect(posX, posY, w, w);
    painter()->strokeRect(posX, posY, w, w);
    // Save the painter state
    painter()->save();

    // Modify painter state
    posX += w + margin;
    painter()->setFillStyle(0xFF900000);
    painter()->setStrokeStyle(0xFFE00000);
    painter()->setLineWidth(lineWidth*2);
    painter()->setLineCap(QNanoPainter::CAP_ROUND);
    painter()->setLineJoin(QNanoPainter::JOIN_ROUND);
    painter()->translate(posX+w/2, posY+w/2);
    painter()->rotate(m_animationSine);
    painter()->scale(0.5 + 0.5*m_animationSine);
    painter()->translate(-(posX+w/2), -(posY+w/2));
    painter()->fillRect(posX, posY, w, w);
    painter()->strokeRect(posX, posY, w, w);

    posX += w + margin;
    // And restore painter state back
    painter()->restore();
    painter()->fillRect(posX, posY, w, w);
    painter()->strokeRect(posX, posY, w, w);

    posY += w+margin;
    posX = margin/2;
    painter()->setStrokeStyle(0xFF40E040);
    painter()->save();
    painter()->translate(w/8 * m_animationSine, w/4 * m_animationSine);
    painter()->strokeRect(posX, posY, w/2, w/2);
    posX += w + margin;
    painter()->translate(w/8 * m_animationSine, w/4 * m_animationSine);
    painter()->strokeRect(posX, posY, w/2, w/2);
    posX += w + margin;
    painter()->translate(w/8 * m_animationSine, w/4 * m_animationSine);
    painter()->strokeRect(posX, posY, w/2, w/2);
    painter()->restore();

    posY += w+margin;
    posX = margin/2;
    painter()->save();
    painter()->translate(posX+w/2, posY+w/2);
    painter()->skewX(0.4 * m_animationSine);
    painter()->translate(-(posX+w/2), -(posY+w/2));
    painter()->strokeRect(posX, posY, w, w);
    painter()->restore();

    posX += w + margin;
    painter()->save();
    painter()->translate(posX+w/2, posY+w/2);
    painter()->skewY(0.4 * m_animationSine);
    painter()->translate(-(posX+w/2), -(posY+w/2));
    painter()->strokeRect(posX, posY, w, w);
    painter()->restore();

    posX += w + margin;
    QTransform t;
    t.translate(posX+w/2, posY+w/2);
    t.rotate(m_animationTime*20);
    float scale = 0.5 + 0.5*m_animationSine;
    t.scale(scale, scale);
    t.translate(-(posX+w/2), -(posY+w/2));
    painter()->transform(t);
    painter()->strokeRect(posX, posY, w, w);
    painter()->setClipRect(posX, posY, w, w);
    // Duplicate transformation
    QTransform t2 = painter()->currentTransform();
    painter()->transform(t2);

    painter()->setStrokeStyle(0xFF40E0E0);
    painter()->strokeRect(posX, posY, w, w);
    painter()->resetClipping();

    posY += w+margin;
    posX = margin/2;
    painter()->resetTransform();
    painter()->setLineWidth(lineWidth);
    QRectF r(posX, posY, w*2, w);
    painter()->strokeRect(r);
    r.adjust(lineWidth/2, lineWidth/2, -lineWidth/2, -lineWidth/2);
    painter()->setClipRect(r);
    painter()->beginPath();
    painter()->circle(posX + m_animationSine*w*2, posY + w/2, w/2);
    painter()->fill();
    painter()->resetClipping();
}

void GalleryItemPainter::drawAntialiasing() {
    int rects = 3;
    qreal margin = width()*0.1;
    qreal w = width() / (rects) - margin;
    qreal posX = margin/2;
    qreal posY = margin;
    qreal lineWidth = width()*0.01;

    painter()->setLineCap(QNanoPainter::CAP_ROUND);
    painter()->setStrokeStyle(0xFFFFFFFF);
    painter()->setLineWidth(lineWidth);
    painter()->setAntialias(0);
    painter()->beginPath();
    painter()->roundedRect(posX, posY, w, w, w/4);
    painter()->stroke();
    posX += w + margin;
    painter()->setAntialias(1.0);
    painter()->beginPath();
    painter()->roundedRect(posX, posY, w, w, w/4);
    painter()->stroke();
    posX += w + margin;
    painter()->setAntialias(1.0 + m_animationSine*5);
    painter()->beginPath();
    painter()->roundedRect(posX, posY, w, w, w/4);
    painter()->stroke();

    posY += w+margin;
    posX = margin/2;
    int lines = 12;
    painter()->setAntialias(0);
    // Align to improve thin lines painting
    painter()->setPixelAlign(QNanoPainter::PIXEL_ALIGN_HALF);
    for (int i=0 ; i<lines ; i++) {
        painter()->beginPath();
        painter()->setLineWidth(i*0.5);
        painter()->moveTo(posX, posY+i*w/lines);
        painter()->lineTo(posX+w, posY+i*w/lines);
        painter()->stroke();
    }
    posX += w + margin;
    painter()->setAntialias(1.0);
    for (int i=0 ; i<lines ; i++) {
        painter()->beginPath();
        painter()->setLineWidth(i*0.5);
        painter()->moveTo(posX, posY+i*w/lines);
        painter()->lineTo(posX+w, posY+i*w/lines);
        painter()->stroke();
    }
    posX += w + margin;
    painter()->setAntialias(1.0 + m_animationSine*5);
    for (int i=0 ; i<lines ; i++) {
        painter()->beginPath();
        painter()->setLineWidth(i*0.5);
        painter()->moveTo(posX, posY+i*w/lines);
        painter()->lineTo(posX+w, posY+i*w/lines);
        painter()->stroke();
    }

    posY += w+margin;
    posX = margin/2;
    painter()->setAntialias(0);
    for (int i=0 ; i<lines ; i++) {
        painter()->beginPath();
        painter()->setLineWidth(i*0.5);
        painter()->moveTo(posX, posY+i*w/lines);
        painter()->lineTo(posX+w, posY+i*w/lines+5);
        painter()->stroke();
    }
    posX += w + margin;
    painter()->setAntialias(1.0);
    for (int i=0 ; i<lines ; i++) {
        painter()->beginPath();
        painter()->setLineWidth(i*0.5);
        painter()->moveTo(posX, posY+i*w/lines);
        painter()->lineTo(posX+w, posY+i*w/lines+5);
        painter()->stroke();
    }
    posX += w + margin;
    painter()->setAntialias(1.0 + m_animationSine*5);
    for (int i=0 ; i<lines ; i++) {
        painter()->beginPath();
        painter()->setLineWidth(i*0.5);
        painter()->moveTo(posX, posY+i*w/lines);
        painter()->lineTo(posX+w, posY+i*w/lines+5);
        painter()->stroke();
    }

    posY += w+margin;
    posX = margin/2;
    posY += m_animationSine*4;
    painter()->setAntialias(0);
    // Stop aligning for smoother animation
    painter()->setPixelAlign(QNanoPainter::PIXEL_ALIGN_NONE);
    for (int i=0 ; i<lines ; i++) {
        painter()->beginPath();
        painter()->setLineWidth(i*0.5);
        painter()->moveTo(posX, posY+i*w/lines);
        painter()->lineTo(posX+w, posY+i*w/lines+5);
        painter()->stroke();
    }
    posX += w + margin;
    painter()->setAntialias(1.0);
    for (int i=0 ; i<lines ; i++) {
        painter()->beginPath();
        painter()->setLineWidth(i*0.5);
        painter()->moveTo(posX, posY+i*w/lines);
        painter()->lineTo(posX+w, posY+i*w/lines+5);
        painter()->stroke();
    }
    posX += w + margin;
    painter()->setAntialias(1.0 + m_animationSine*5);
    for (int i=0 ; i<lines ; i++) {
        painter()->beginPath();
        painter()->setLineWidth(i*0.5);
        painter()->moveTo(posX, posY+i*w/lines);
        painter()->lineTo(posX+w, posY+i*w/lines+5);
        painter()->stroke();
    }

}

void GalleryItemPainter::drawTextsFonts() {
    float margin = height() * 0.04;
    float posX = margin/2;
    float posY = margin;
    painter()->fillText("This is text without setting any font", posX, posY);

    posY += margin;
    QNanoFont f;
    painter()->setFont(f);
    painter()->fillText("This is text with default font", posX, posY);

    posY += margin;
    painter()->setPixelAlignText(QNanoPainter::PIXEL_ALIGN_FULL);
    painter()->fillText("This is text aligned to pixels", posX, posY);
    painter()->setPixelAlignText(QNanoPainter::PIXEL_ALIGN_NONE);

    posY += 2*margin;
    painter()->setFillStyle(0xFFD06060);
    f.setPixelSize(30);
    painter()->setFont(f);
    painter()->fillText("Size 30px", posX, posY);
    posY += 2*margin;
    f.setPointSize(30);
    painter()->setFont(f);
    painter()->fillText("Size 30pt", posX, posY);

    posY += 2*margin;
    painter()->setFillStyle(0xFFFFFFFF);
    QNanoFont f4 = QNanoFont(QNanoFont::DEFAULT_FONT_THIN);
    f4.setPixelSize(margin);
    painter()->setFont(f4);
    painter()->fillText("DEFAULT_FONT_THIN", posX, posY);

    f4.setFontId(QNanoFont::DEFAULT_FONT_THIN_ITALIC);
    painter()->setFont(f4);
    posY += margin;
    painter()->fillText("DEFAULT_FONT_THIN_ITALIC", posX, posY);
    f4.setFontId(QNanoFont::DEFAULT_FONT_LIGHT);
    painter()->setFont(f4);
    posY += margin;
    painter()->fillText("DEFAULT_FONT_LIGHT", posX, posY);
    f4.setFontId(QNanoFont::DEFAULT_FONT_LIGHT_ITALIC);
    painter()->setFont(f4);
    posY += margin;
    painter()->fillText("DEFAULT_FONT_LIGHT_ITALIC", posX, posY);
    f4.setFontId(QNanoFont::DEFAULT_FONT_NORMAL);
    painter()->setFont(f4);
    posY += margin;
    painter()->fillText("DEFAULT_FONT_NORMAL", posX, posY);
    f4.setFontId(QNanoFont::DEFAULT_FONT_NORMAL_ITALIC);
    painter()->setFont(f4);
    posY += margin;
    painter()->fillText("DEFAULT_FONT_NORMAL_ITALIC", posX, posY);
    f4.setFontId(QNanoFont::DEFAULT_FONT_BOLD);
    painter()->setFont(f4);
    posY += margin;
    painter()->fillText("DEFAULT_FONT_BOLD", posX, posY);
    f4.setFontId(QNanoFont::DEFAULT_FONT_BOLD_ITALIC);
    painter()->setFont(f4);
    posY += margin;
    painter()->fillText("DEFAULT_FONT_BOLD_ITALIC", posX, posY);

    posY += 2*margin;
    QNanoFont f2 = QNanoFont(":/fonts/Pacifico.ttf");
    f2.setPixelSize(margin*2);
    painter()->setFont(f2);
    painter()->fillText("This is text with custom font", posX, posY);

    posY += 2*margin;
    f2.setBlur(margin*0.2 + margin*0.4*m_animationSine);
    painter()->setFont(f2);
    painter()->fillText("This is text with some glowing", posX, posY);
    f2.setBlur(0);
    painter()->setFont(f2);
    painter()->setFillStyle(0xFF000000);
    painter()->fillText("This is text with some glowing", posX, posY);

    posY += 2*margin;
    QNanoFont f5 = QNanoFont();
    f5.setPixelSize(margin*0.8);
    f5.setLetterSpacing(6 * m_animationSine - 2);
    painter()->setFont(f5);
    painter()->setFillStyle(0xFFFFFFFF);
    painter()->fillText("This is text with varying letter spacing", posX, posY);

    /* TODO: Test how font size animation behaves
    posY += 2*margin;
    f5.setSize(margin*0.5 + margin*4*m_animationSine);
    painter()->setFont(f5);
    painter()->fillText("Font SIZES", posX, posY);
    */
}

void GalleryItemPainter::drawTextsAlignments() {
    float margin = height() * 0.2;
    float fontSize = height() * 0.04;
    float posX = margin/2;
    float posY = margin/2;

    QNanoFont f3;
    f3.setPixelSize(fontSize);
    painter()->setFont(f3);

    // Test horizontal alignments
    posX = width() * 0.5;
    painter()->setFillStyle(0xFF808080);
    painter()->fillRect(posX-1, posY-fontSize, 2, fontSize*3);
    painter()->setTextBaseline(QNanoPainter::BASELINE_BOTTOM);
    painter()->setFillStyle(0xFFFF9090);
    painter()->setTextAlign(QNanoPainter::ALIGN_LEFT);
    painter()->fillText("ALIGN_LEFT", posX, posY);
    painter()->setFillStyle(0xFF90FF90);
    painter()->setTextAlign(QNanoPainter::ALIGN_CENTER);
    painter()->fillText("ALIGN_CENTER", posX, posY+fontSize);
    painter()->setFillStyle(0xFF9090FF);
    painter()->setTextAlign(QNanoPainter::ALIGN_RIGHT);
    painter()->fillText("ALIGN_RIGHT", posX, posY+fontSize*2);

    // Test vertical (baseline) alignments
    posY += margin;
    posX = width() * 0.1;
    painter()->setFillStyle(0xFF808080);
    painter()->fillRect(width()*0.1, posY-1, width()*0.8, 2);
    painter()->setTextAlign(QNanoPainter::ALIGN_LEFT);
    painter()->setFillStyle(0xFFFF9090);
    painter()->setTextBaseline(QNanoPainter::BASELINE_BOTTOM);
    painter()->fillText("BASELINE_BOTTOM", posX, posY);
    painter()->setFillStyle(0xFF90FF90);
    painter()->setTextBaseline(QNanoPainter::BASELINE_ALPHABETIC);
    painter()->fillText("BASELINE_ALPHABETIC", posX+width()*0.15, posY);
    painter()->setFillStyle(0xFF9090FF);
    painter()->setTextBaseline(QNanoPainter::BASELINE_MIDDLE);
    painter()->fillText("BASELINE_MIDDLE", posX+width()*0.3, posY);
    painter()->setFillStyle(0xFFFF90FF);
    painter()->setTextBaseline(QNanoPainter::BASELINE_TOP);
    painter()->fillText("BASELINE_TOP", posX+width()*0.45, posY);

    // Test text alignment and line wrapping
    posY += margin;
    QString s("This is a test string which is slightly longer one. Yes, we want long string for this.");
    painter()->setFillStyle(0xFFFFFFFF);
    //painter()->setTextAlign(QNanoPainter::ALIGN_LEFT);
    //painter()->setTextAlign(QNanoPainter::ALIGN_RIGHT);
    painter()->setTextAlign(QNanoPainter::ALIGN_CENTER);
    QRectF rect1(width()*0.1, posY, width()*0.8 - m_animationSine*width()*0.4, 100);
    painter()->fillText(s, rect1);

    // Text bounding box
    posY += margin;
    //painter()->setTextAlign(QNanoPainter::ALIGN_LEFT);
    painter()->setTextAlign(QNanoPainter::ALIGN_RIGHT);
    //painter()->setTextAlign(QNanoPainter::ALIGN_CENTER);
    QRectF bounds = painter()->textBoundingBox(s, width()*0.1, posY, width()*0.8 - m_animationSine*width()*0.4);
    painter()->setFillStyle(0x80808080);
    painter()->fillRect(bounds);
    painter()->setFillStyle(0xFFFFFF80);
    painter()->fillText(s, bounds);
}

void GalleryItemPainter::drawImages() {

    qreal margin = width()*0.1;
    qreal posX = margin;
    qreal posY = margin;

    QRectF rect1(0, 0, width(), height());
    painter()->setFillStyle(QNanoImagePattern(m_patternImage2));
    painter()->fillRect(rect1);

    // Draw image at its native resolution
    QNanoImage image(":/images/quit_logo2.png");
    painter()->drawImage(image, posX, posY);

    // Draw just the "coding" part of the image
    posY += image.height() + margin;
    QRectF sourceArea(3,89,192-6,28);
    float cx = width()/2;
    float r = width()*0.1 + m_animationSine*width()*0.2;
    QRectF targetArea(cx-r, posY, r*2, width()*0.1);
    painter()->drawImage(m_testImage, sourceArea, targetArea);

    posY += targetArea.height() + height()*0.2;
    image.setFilename(":/images/face.png");
    QRectF rect(cx-r, posY-r, r*2, r*2);
    QPointF c(rect.x()+rect.width()/2, rect.y()+rect.height()/2);
    painter()->translate(c);
    painter()->rotate(m_animationTime);
    painter()->translate(-c);
    painter()->drawImage(image,rect);
}

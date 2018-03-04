#ifndef GALLERYITEMPAINTER_H
#define GALLERYITEMPAINTER_H

#include "qnanoquickitempainter.h"
#include "qnanopainter.h"
#include "qnanoimage.h"
#include "qnanofont.h"

#include <QQuickItem>

class GalleryItemPainter : public QNanoQuickItemPainter
{
public:
    explicit GalleryItemPainter();
    ~GalleryItemPainter();

    // Reimplement
    void synchronize(QNanoQuickItem *item);
    void paint(QNanoPainter *painter);

private:

    // View - Rectangles
    void drawRectsWithSameColor();
    void drawRectsWithMix();
    void drawRectsWithLinearGradient();
    void drawRectsWithRadialGradient();
    void drawRectsWithBoxGradient();
    void drawRectsWithImagePattern();
    void drawRectsWithBrushStroke();

    // View - Paths
    void drawPaths();

    // View - States and transforms
    void drawTransforms();

    // View - Antialiasing
    void drawAntialiasing();

    // View - Texts
    void drawTextsFonts();
    void drawTextsAlignments();

    void drawImages();

    void drawRect(float x, float y, float w, float h);

    //QNanoPainter *m_painter;
    float m_animationTime;
    float m_animationSine;
    int m_viewIndex;

    QNanoImage m_testImage;
    QNanoImage m_patternImage;
    QNanoImage m_patternImage2;
    QNanoImage m_patternImage3;
};

#endif // GALLERYITEMPAINTER_H

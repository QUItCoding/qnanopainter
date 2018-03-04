#ifndef DEMOQNANOITEMPAINTER_H
#define DEMOQNANOITEMPAINTER_H

#include "qnanoquickitempainter.h"
#include "qnanocolor.h"

class DemoQNanoItemPainter : public QNanoQuickItemPainter
{

public:
    explicit DemoQNanoItemPainter();

    // Reimplement
    void synchronize(QNanoQuickItem *item);
    void paint(QNanoPainter *painter);

private:
    void drawGraphLine(float x, float y, float w, float h, int items, float t);
    void drawGraphBars(float x, float y, float w, float h, int items, float t);
    void drawGraphCircles(float x, float y, float w, float h, int items, float t);
    void drawIcons(float x, float y, float w, float h, int items, float t);
    void drawRuler(float x, float y, float w, float h, float t);
    void drawFlower(float x, float y, float w, float h, float t);
    float _flowerPos(int i);

    QNanoPainter *m_painter;
    float m_animationTime;
    int m_enabledTests;
    int m_testCount;

    // Colors
    QNanoColor m_colorWhite, m_colorGray, m_colorBlack,
    m_color1, m_color2, m_color3;

    QNanoImage m_circleImage;
    QNanoFont m_testFont;

};

#endif // DEMOQNANOITEMPAINTER_H

#ifndef DEMOQPITEM_H
#define DEMOQPITEM_H

#include <QQuickPaintedItem>
#include <QColor>
#include <QImage>
#include <QFont>

extern int g_customFontId; // set in main.cpp

class DemoQPItem : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(float animationTime READ animationTime WRITE setAnimationTime NOTIFY animationTimeChanged)
    Q_PROPERTY(int enabledTests READ enabledTests WRITE setEnabledTests NOTIFY enabledTestsChanged)
    Q_PROPERTY(int testCount READ testCount WRITE setTestCount NOTIFY testCountChanged)
    Q_PROPERTY(bool qpAntialiasing READ qpAntialiasing WRITE setQpAntialiasing NOTIFY qpAntialiasingChanged)
    Q_PROPERTY(bool qpRenderTargetFBO READ qpRenderTargetFBO WRITE setQpRenderTargetFBO NOTIFY qpRenderTargetFBOChanged)

public:    
    DemoQPItem(QQuickItem *parent = nullptr);
    void paint(QPainter *painter);

    float animationTime() const { return m_animationTime; }
    int enabledTests() const { return m_enabledTests; }
    int testCount() const { return m_testCount; }
    bool qpAntialiasing() const { return m_qpAntialiasing; }
    bool qpRenderTargetFBO() const { return m_qpRenderTargetFBO; }

    void setAnimationTime(const float value) {
        if (qFuzzyCompare(value, m_animationTime))
            return;
        m_animationTime = value;
        Q_EMIT animationTimeChanged();
        update();
    }

    void setEnabledTests(const int value) {
        if (value == m_enabledTests)
            return;
        m_enabledTests = value;
        Q_EMIT enabledTestsChanged();
        update();
    }

    void setTestCount(const int value) {
        if (value == m_testCount)
            return;
        m_testCount = value;
        Q_EMIT testCountChanged();
        update();
    }

    void setQpAntialiasing(const bool value) {
        if (value == m_qpAntialiasing)
            return;
        m_qpAntialiasing = value;
        Q_EMIT qpAntialiasingChanged();
        setAntialiasing(value);
        update();
    }

    // Enable to use FBO rendering instead of default QQuickPaintedItem::Image
    // Increases performance with reduced rendering quality, no antialiasing
    void setQpRenderTargetFBO(const bool value) {
        if (value == m_qpRenderTargetFBO)
            return;
        m_qpRenderTargetFBO = value;
        setRenderTarget(m_qpRenderTargetFBO ? QQuickPaintedItem::FramebufferObject : QQuickPaintedItem::Image);
        Q_EMIT qpRenderTargetFBOChanged();
        update();
    }

Q_SIGNALS:
    void animationTimeChanged();
    void enabledTestsChanged();
    void testCountChanged();
    void qpAntialiasingChanged();
    void qpRenderTargetFBOChanged();

private:
    void drawGraphLine(float x, float y, float w, float h, int items, float t);
    void drawGraphBars(float x, float y, float w, float h, int items, float t);
    void drawGraphCircles(float x, float y, float w, float h, int items, float t);
    void drawIcons(float x, float y, float w, float h, int items, float t);
    void drawRuler(float x, float y, float w, float h, float t);
    void drawFlower(float x, float y, float w, float h, float t);
    float _flowerPos(int i);

    float m_animationTime;
    int m_enabledTests;
    int m_testCount;
    bool m_qpAntialiasing;
    bool m_qpRenderTargetFBO;

    // Colors
    QColor m_colorWhite, m_colorGray, m_colorBlack,
    m_color1, m_color2, m_color3;
    QPainter *m_painter;
    QImage m_circleImage;
    QFont m_testFont;

};

#endif // DEMOQPITEM_H

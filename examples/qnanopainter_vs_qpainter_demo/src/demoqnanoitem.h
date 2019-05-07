#ifndef DEMOQNANOITEM_H
#define DEMOQNANOITEM_H

#include "qnanoquickitem.h"
#include "demoqnanoitempainter.h"

class DemoQNanoItem : public QNanoQuickItem
{

    Q_OBJECT
    Q_PROPERTY(float animationTime READ animationTime WRITE setAnimationTime NOTIFY animationTimeChanged)
    Q_PROPERTY(int enabledTests READ enabledTests WRITE setEnabledTests NOTIFY enabledTestsChanged)
    Q_PROPERTY(int testCount READ testCount WRITE setTestCount NOTIFY testCountChanged)

public:
    DemoQNanoItem(QQuickItem *parent = nullptr);

    // Reimplement
    QNanoQuickItemPainter *createItemPainter() const;

    float animationTime() const { return m_animationTime; }
    int enabledTests() const { return m_enabledTests; }
    int testCount() const { return m_testCount; }

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

Q_SIGNALS:
    void animationTimeChanged();
    void enabledTestsChanged();
    void testCountChanged();

private:
    float m_animationTime;
    int m_enabledTests;
    int m_testCount;
};

#endif // DEMOQNANOITEM_H

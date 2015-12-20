#ifndef DEMOQNANOITEM_H
#define DEMOQNANOITEM_H

#include "qnanoquickitem.h"
#include "demoqnanoitempainter.h"

class DemoQNanoItem : public QNanoQuickItem
{

    Q_OBJECT
    Q_PROPERTY(qreal animationTime READ animationTime WRITE setAnimationTime NOTIFY animationTimeChanged)
    Q_PROPERTY(int enabledTests READ enabledTests WRITE setEnabledTests NOTIFY enabledTestsChanged)
    Q_PROPERTY(int testCount READ testCount WRITE setTestCount NOTIFY testCountChanged)

public:
    DemoQNanoItem(QQuickItem *parent = 0);

    // Reimplement
    QNanoQuickItemPainter *createItemPainter() const;

    qreal animationTime() const { return m_animationTime; }
    int enabledTests() const { return m_enabledTests; }
    int testCount() const { return m_testCount; }

    void setAnimationTime(const qreal value) {
        if (value == m_animationTime)
            return;
        m_animationTime = value;
        emit animationTimeChanged();
        update();
    }

    void setEnabledTests(const int value) {
        if (value == m_enabledTests)
            return;
        m_enabledTests = value;
        emit enabledTestsChanged();
        update();
    }

    void setTestCount(const int value) {
        if (value == m_testCount)
            return;
        m_testCount = value;
        emit testCountChanged();
        update();
    }

Q_SIGNALS:
    void animationTimeChanged();
    void enabledTestsChanged();
    void testCountChanged();

private:
    qreal m_animationTime;
    int m_enabledTests;
    int m_testCount;
};

#endif // DEMOQNANOITEM_H

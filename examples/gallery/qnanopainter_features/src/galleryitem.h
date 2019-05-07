#ifndef GALLERYITEM_H
#define GALLERYITEM_H

#include "qnanoquickitem.h"
#include "galleryitempainter.h"
#include <QQuickItem>

class GalleryItem : public QNanoQuickItem
{
    Q_OBJECT
    // Alternatively could just use:
    //QNANO_PROPERTY(float, m_animationTime, animationTime, setAnimationTime)
    //QNANO_PROPERTY(float, m_animationSine, animationSine, setAnimationSine)
    QNANO_PROPERTY(int, m_galleryView, galleryView, setGalleryView)

    Q_PROPERTY(float animationTime READ animationTime WRITE setAnimationTime NOTIFY animationTimeChanged)
    Q_PROPERTY(float animationSine READ animationSine WRITE setAnimationSine NOTIFY animationSineChanged)

public:

    GalleryItem(QQuickItem *parent = nullptr);

    // Reimplement
    QNanoQuickItemPainter *createItemPainter() const;

    float animationTime() const { return m_animationTime; }
    float animationSine() const { return m_animationSine; }

    void setAnimationTime(const float value) {
        if (qFuzzyCompare(value, m_animationTime))
            return;
        m_animationTime = value;
        Q_EMIT animationTimeChanged();
        update();
    }

    void setAnimationSine(const float value) {
        if (qFuzzyCompare(value, m_animationSine))
            return;
        m_animationSine = value;
        Q_EMIT animationSineChanged();
        update();
    }

Q_SIGNALS:
    void animationTimeChanged();
    void animationSineChanged();

private:
    float m_animationTime;
    float m_animationSine;
};

#endif // GALLERYITEM_H

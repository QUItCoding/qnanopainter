#ifndef PAINTINGITEMPAINTER_H
#define PAINTINGITEMPAINTER_H

#include "qnanoquickitempainter.h"
#include "qnanopainter.h"
#include <QQueue>
#include <QVector>
#include <QSequentialAnimationGroup>

class Path : public QObject
{
    Q_OBJECT
    Q_PROPERTY(float opacity READ opacity WRITE setOpacity NOTIFY opacityChanged)
public:
    Path();
    ~Path();

    float opacity() {
        return m_opacity;
    }
    void setOpacity(float opacity) {
        m_opacity = opacity;
        Q_EMIT opacityChanged();
    }


Q_SIGNALS:
    void opacityChanged();

private:
    friend class PaintingItemPainter;
    float m_opacity;
    QVector<QPoint> m_points;
    QSequentialAnimationGroup m_hideAnimation;
};

class PaintingItemPainter: public QObject, public QNanoQuickItemPainter
{
    Q_OBJECT
public:
    PaintingItemPainter();

    // Reimplement
    void synchronize(QNanoQuickItem *item);
    void paint(QNanoPainter *painter);

Q_SIGNALS:
    void update();

private:
    void drawPathLine(const QVector<QPoint> &points, const QPoint translate);
    QQueue<QSharedPointer<Path> > m_paths;

};

#endif // PAINTINGITEMPAINTER_H

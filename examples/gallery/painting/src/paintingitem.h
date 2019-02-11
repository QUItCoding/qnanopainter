#ifndef PAINTINGITEM_H
#define PAINTINGITEM_H

#include "qnanoquickitem.h"
#include <QVector>

class QNanoQuickItemPainter;

class PaintingItem: public QNanoQuickItem
{
    Q_OBJECT
public:
    PaintingItem(QQuickItem *parent = nullptr);

    // Reimplement
    QNanoQuickItemPainter *createItemPainter() const Q_DECL_OVERRIDE Q_DECL_FINAL;

protected:
    // Reimplement from QQuickItem
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE Q_DECL_FINAL;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE Q_DECL_FINAL;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE Q_DECL_FINAL;

private:
    friend class PaintingItemPainter;
    QVector<QPoint> m_points;
    int m_pointsSynced;
};

#endif // PAINTINGITEM_H

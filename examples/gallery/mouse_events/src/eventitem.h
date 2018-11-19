#ifndef EVENTITEM_H
#define EVENTITEM_H

#include "qnanoquickitem.h"

class EventItem : public QNanoQuickItem
{
    Q_OBJECT
public:
    EventItem(QQuickItem *parent = nullptr);

    // Reimplement
    QNanoQuickItemPainter *createItemPainter() const;

public Q_SLOTS:
    void generateRandomItems();

protected:
    // Reimplement from QQuickItem
    void geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void hoverEnterEvent(QHoverEvent *event);
    void hoverLeaveEvent(QHoverEvent *event);
    void hoverMoveEvent(QHoverEvent *event);

private:
    friend class EventItemPainter;
    int topItemAt(QPointF pos);
    int resizeItemAt(QPointF pos);

    QList<QRectF> m_items;
    int m_activeItem;
    int m_pressedItem;
    int m_resizableItem;
    int m_circleSize;
    bool m_resizing;
    bool m_hovered;
    QPointF m_pressPos;

};

#endif // EVENTITEM_H

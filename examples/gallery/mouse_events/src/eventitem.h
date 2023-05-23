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
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    void geometryChange(const QRectF &newGeometry, const QRectF &oldGeometry) Q_DECL_OVERRIDE;
#else
    void geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry) Q_DECL_OVERRIDE;
#endif
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseDoubleClickEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void hoverEnterEvent(QHoverEvent *event) Q_DECL_OVERRIDE;
    void hoverLeaveEvent(QHoverEvent *event) Q_DECL_OVERRIDE;
    void hoverMoveEvent(QHoverEvent *event) Q_DECL_OVERRIDE;

private:
    friend class EventItemPainter;
    int topItemAt(QPointF pos);
    int resizeItemAt(QPointF pos);

    QList<QRectF> m_items;
    int m_activeItem = -1;
    int m_pressedItem = -1;
    int m_resizableItem = -1;
    int m_circleSize = 10;
    bool m_resizing = false;
    bool m_hovered = false;
    QPointF m_pressPos;

};

#endif // EVENTITEM_H

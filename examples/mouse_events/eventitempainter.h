#ifndef EVENTITEMPAINTER_H
#define EVENTITEMPAINTER_H

#include "qnanoquickitempainter.h"

class EventItemPainter : public QNanoQuickItemPainter
{
public:
    EventItemPainter();
    // Reimplement
    void synchronize(QNanoQuickItem *item);
    void paint(QNanoPainter *p);

private:
    QList<QRectF> m_items;
    int m_activeItem;
    int m_pressedItem;
    int m_resizableItem;
    int m_circleSize;
    bool m_hoverEnabled;
    bool m_resizing;
    bool m_hovered;
};

#endif // EVENTITEMPAINTER_H

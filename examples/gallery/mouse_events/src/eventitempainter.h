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
    int m_activeItem = -1;
    int m_pressedItem = -1;
    int m_resizableItem = -1;
    int m_circleSize = 10;
    bool m_hoverEnabled = true;
    bool m_resizing = false;
    bool m_hovered = false;
};

#endif // EVENTITEMPAINTER_H

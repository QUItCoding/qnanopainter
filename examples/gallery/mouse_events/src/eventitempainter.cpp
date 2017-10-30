#include "eventitempainter.h"
#include "eventitem.h"

EventItemPainter::EventItemPainter()
{
    m_activeItem = -1;
    m_pressedItem = -1;
    m_resizableItem = -1;
    m_circleSize = 10;
    m_hoverEnabled = true;
    m_hovered = false;
    m_resizing = false;
}

void EventItemPainter::synchronize(QNanoQuickItem *item)
{
    // Setting values here synchronized
    EventItem *realItem = static_cast<EventItem*>(item);
    if (realItem) {
        m_items = realItem->m_items;
        m_activeItem = realItem->m_activeItem;
        m_pressedItem = realItem->m_pressedItem;
        m_resizableItem = realItem->m_resizableItem;
        m_circleSize = realItem->m_circleSize;
        m_hoverEnabled = realItem->mouseHoverEventsEnabled();
        m_hovered = realItem->m_hovered;
    }
}

void EventItemPainter::paint(QNanoPainter *p)
{
    p->setGlobalAlpha(m_hoverEnabled && !m_hovered ? 0.5 : 1.0);
    // Background
    p->beginPath();
    p->setFillStyle(0xFF408070);
    p->setStrokeStyle(0xFFD0D0D0);
    p->rect(0, 0, width(), height());
    p->setLineWidth(2);
    p->fill();
    p->stroke();

    // Draw boxes
    p->setLineWidth(1);
    for (int i=0 ; i<m_items.count() ; i++)
    {
        QRectF box = m_items.at(i);
        bool isActive = (m_activeItem == i);
        bool isPressed = (m_pressedItem == i);
        QNanoColor fillColor(isPressed ? 0xFFFF0000 : isActive ? 0xFFFFFFFF : 0xFF000000);
        fillColor.setAlphaF(0.2f);
        p->setFillStyle(fillColor);
        p->setStrokeStyle(0xFFD0D0D0);
        p->beginPath();
        p->rect(box);
        p->fill();
        p->stroke();
        // corner circle
        p->beginPath();
        bool isResizable = (m_resizableItem == i);
        p->setFillStyle(isResizable ? 0x80F0F0F0 : 0x00F0F0F0);
        p->circle(box.x()+box.width(), box.y()+box.height(), m_circleSize);
        p->stroke();
        p->fill();
        // Item number
        p->setFillStyle(0xFFE0E0E0);
        p->setTextAlign(QNanoPainter::ALIGN_CENTER);
        p->setTextBaseline(QNanoPainter::BASELINE_MIDDLE);
        QNanoFont font(QNanoFont::DEFAULT_FONT_BOLD);
        font.setPointSize(12);
        p->setFont(font);
        p->fillText(QString::number(i+1), box.center());
    }
}


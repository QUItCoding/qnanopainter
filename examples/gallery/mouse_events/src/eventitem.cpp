#include "eventitem.h"
#include "eventitempainter.h"
#include <QtMath>
#include <QCursor>

#if (QT_VERSION >= 0x050150)         // Qt5.15 -- qrand() deprecated ; Qt6 -- qrand() is gone.
#include <QRandomGenerator>
#endif /* (QT_VERSION >= 0x050150) */

EventItem::EventItem(QQuickItem *parent)
    :  QNanoQuickItem(parent)
{
    m_activeItem = -1;
    m_pressedItem = -1;
    m_resizableItem = -1;
    m_circleSize = 10;
    m_hovered = false;
    m_resizing = false;
}

QNanoQuickItemPainter* EventItem::createItemPainter() const
{
    return new EventItemPainter();
}

#if (QT_VERSION >= 0x060000)
void EventItem::geometryChange(const QRectF &newGeometry, const QRectF &oldGeometry)
#else
void EventItem::geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry)
#endif
{
#if (QT_VERSION >= 0x060000) //Qt6 -- renamed to geometryChange()
    QQuickItem::geometryChange(newGeometry, oldGeometry);
#else                        //Qt5 -- had geometryChanged()
    QQuickItem::geometryChanged(newGeometry, oldGeometry);
#endif /* (QT_VERSION >= 0x060000) */
    if (widthValid() && heightValid()) {
        m_circleSize = 2 + int(qMin(width(), height())*0.01);
        generateRandomItems();
    }
}

void EventItem::mousePressEvent(QMouseEvent *event)
{
    qDebug() << "LOG: mousePressEvent";
    // Resizing an item?
    int resizeItem = resizeItemAt(event->pos());
    if (resizeItem != -1) {
            m_resizing = true;
            int topIndex = m_items.count()-1;
            m_items.move(resizeItem, topIndex);
            m_pressedItem = topIndex;
            m_activeItem = -1;
            update();
            setCursor(QCursor(Qt::SizeFDiagCursor));
    }

    // Moving an item?
    if (!m_resizing) {
        int pressItem = topItemAt(event->pos());
        if (pressItem != -1) {
            int topIndex = m_items.count()-1;
            m_items.move(pressItem, topIndex);
            m_pressedItem = topIndex;
            m_activeItem = mouseHoverEventsEnabled() ? topIndex : -1;
            m_pressPos = event->pos() - m_items.last().topLeft();
            update();
            setCursor(QCursor(Qt::OpenHandCursor));
        }
    }

    if (m_pressedItem == -1) {
        // If press doesn't hit any item, don't accept (so let it through to QtQuick)
        event->setAccepted(false);
    }
}

void EventItem::mouseReleaseEvent(QMouseEvent *event)
{
    qDebug() << "LOG: mouseReleaseEvent";
    Q_UNUSED(event);
    if (!mouseHoverEventsEnabled()) {
        m_activeItem = -1;
    }
    m_pressedItem = -1;
    m_resizing = false;
    setCursor(QCursor(Qt::ArrowCursor));
    update();
}

void EventItem::mouseDoubleClickEvent(QMouseEvent *event)
{
    qDebug() << "LOG: mouseDoubleClickEvent";
    // Remove item
    int item = topItemAt(event->pos());
    m_items.removeAt(item);
    m_activeItem = m_pressedItem = m_resizableItem = -1;
    update();
}

void EventItem::mouseMoveEvent(QMouseEvent *event)
{
    qDebug() << "LOG: mouseMoveEvent";
    if (m_pressedItem != -1) {
        QRectF *item = &m_items[m_pressedItem];
        if (m_resizing) {
            item->setBottomRight(event->pos());
        } else {
            item->moveTo(event->pos() - m_pressPos);
        }
        update();
    }
}

void EventItem::hoverEnterEvent(QHoverEvent *event)
{
    qDebug() << "LOG: hoverEnterEvent";
    Q_UNUSED(event);
    m_hovered = true;
    update();
}

void EventItem::hoverLeaveEvent(QHoverEvent *event)
{
    qDebug() << "LOG: hoverLeaveEvent";
    Q_UNUSED(event);
    m_hovered = false;
    update();
}

void EventItem::hoverMoveEvent(QHoverEvent *event)
{
    qDebug() << "LOG: hoverMoveEvent";
    int resizeItem = resizeItemAt(event->pos());
    if (resizeItem != m_resizableItem) {
        m_resizableItem = resizeItem;
        update();
    }
    int hoverItem = topItemAt(event->pos());
    if (hoverItem != m_activeItem) {
        m_activeItem = hoverItem;
        update();
    }

    // If mouse is on top of some resize area, don't activate
    if (m_resizableItem != -1) {
        m_activeItem = -1;
    }

    if (m_resizableItem != -1) {
        setCursor(QCursor(Qt::SizeFDiagCursor));
    } else if (m_activeItem != -1) {
        setCursor(QCursor(Qt::OpenHandCursor));
    } else {
        setCursor(QCursor(Qt::ArrowCursor));
    }
}

int EventItem::topItemAt(QPointF pos)
{
    for (int i=m_items.count()-1 ; i>=0 ; i--)
    {
        if (m_items.at(i).contains(pos)) {
            return i;
        }
    }
    return -1;
}

int EventItem::resizeItemAt(QPointF pos)
{
    for (int i=m_items.count()-1 ; i>=0 ; i--)
    {
        QRectF item = m_items.at(i);
        QPointF p = item.bottomRight() - pos;
        if (p.manhattanLength() <= m_circleSize*2) {
            return i;
        } else if (item.contains(pos)) {
            // This item is on top of other possible resize corners, so stop seaching
            break;
        }
    }
    return -1;
}

#if (QT_VERSION >= 0x050150)         // Qt5.15 -- qrand() deprecated ; Qt6 -- qrand() is gone.
#define QRAND() QRandomGenerator::global()->generate()
#else
#define QRAND() qrand()
#endif /* (QT_VERSION >= 0x050150) */

void EventItem::generateRandomItems()
{
    m_items.clear();
    int margin = int(width()*0.05);
    int items = 2 + QRAND()%150;
    for (int i=0; i<items ; i++) {
        double w = QRAND() % int(width()*0.2) + margin;
        double h = QRAND() % int(height()*0.2) + margin;
        double x = QRAND() % int(width()-w-margin*2) + margin;
        double y = QRAND() % int(height()-h-margin*2) + margin;
        m_items << QRectF(x, y, w, h);
    }
    update();
}

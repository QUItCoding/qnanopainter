#include "paintingitem.h"
#include "paintingitempainter.h"

PaintingItem::PaintingItem(QQuickItem *parent)
    : QNanoQuickItem(parent)
    , m_pointsSynced(0)
{
    setHighQualityRendering(true);
}

QNanoQuickItemPainter* PaintingItem::createItemPainter() const
{
    PaintingItemPainter *itemPainter = new PaintingItemPainter();
    QObject::connect(itemPainter, &PaintingItemPainter::update, this, &PaintingItem::update);
    return itemPainter;
}

void PaintingItem::mousePressEvent(QMouseEvent *event)
{
    // Start a new line
    m_pointsSynced = 0;
    m_points.clear();
    m_points.append(event->pos());
    update();
}

void PaintingItem::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    // Use INT_MAX as a mark that drawing the line has ended
    m_points.append(QPoint(INT_MAX, 0));
    update();
}

void PaintingItem::mouseMoveEvent(QMouseEvent *event)
{
    m_points.append(event->pos());
    update();
}

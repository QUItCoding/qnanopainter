#include "demoqnanoitem.h"

DemoQNanoItem::DemoQNanoItem(QQuickItem *parent)
:  QNanoQuickItem(parent)
{
}

QNanoQuickItemPainter* DemoQNanoItem::createItemPainter() const
{
    return new DemoQNanoItemPainter();
}

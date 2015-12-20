#include "galleryitem.h"

GalleryItem::GalleryItem(QQuickItem *parent)
:  QNanoQuickItem(parent)
{
}

QNanoQuickItemPainter* GalleryItem::createItemPainter() const
{
    return new GalleryItemPainter();
}


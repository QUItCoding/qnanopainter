#ifndef HELLOITEM
#define HELLOITEM

#include "qnanoquickitem.h"
#include "qnanoquickitempainter.h"
#include "painthelper.h"

// HelloItemPainter contains the painting code
class HelloItemPainter : public QNanoQuickItemPainter
{

public:
    HelloItemPainter()
    {
    }

    void paint(QNanoPainter *p)
    {
        // Painting code is shared as it's identical in all hello* examples
        paintHelloItem(p, width(), height());
    }
};

// HelloItem provides the API towards QML
class HelloItem : public QNanoQuickItem
{
    Q_OBJECT

public:
    HelloItem(QQuickItem *parent = nullptr)
        :  QNanoQuickItem(parent)
    {
    }

    // Reimplement
    QNanoQuickItemPainter *createItemPainter() const
    {
        // Create painter for this item
        return new HelloItemPainter();
    }
};

#endif // HELLOITEM


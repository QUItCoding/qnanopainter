#ifndef HELLOITEM
#define HELLOITEM

#include "qnanoquickitem.h"
#include "qnanoquickitempainter.h"

// HelloItemPainter contains the painting code
class HelloItemPainter : public QNanoQuickItemPainter
{
    Q_OBJECT

public:
    HelloItemPainter()
    {
    }

    void paint()
    {
        // Paint the background circle
        painter()->beginPath();
        painter()->circle(width()*0.5, height()*0.5, width()*0.46);
        QNanoRadialGradient gradient1(width()*0.5, height()*0.4, width()*0.6, width()*0.2);
        gradient1.setStartColor("#808080");
        gradient1.setEndColor("#404040");
        painter()->setFillStyle(gradient1);
        painter()->fill();
        painter()->setStrokeStyle("#202020");
        painter()->setLineWidth(width()*0.02);
        painter()->stroke();
        // Hello text
        painter()->setTextAlign(QNanoPainter::ALIGN_CENTER);
        painter()->setTextBaseline(QNanoPainter::BASELINE_MIDDLE);
        QNanoFont font1(QNanoFont::DEFAULT_FONT_BOLD_ITALIC);
        font1.setPixelSize(width()*0.08);
        painter()->setFont(font1);
        painter()->setFillStyle("#B0D040");
        painter()->fillText("HELLO", width()*0.5, height()*0.4);
        // QNanoPainter text
        QNanoFont font2(QNanoFont::DEFAULT_FONT_THIN);
        font2.setPixelSize(width()*0.18);
        painter()->setFont(font2);
        painter()->fillText("QNanoPainter", width()*0.5, height()*0.5);
    }
};

// HelloItem provides the API towards QML
class HelloItem : public QNanoQuickItem
{
    Q_OBJECT

public:
    HelloItem(QQuickItem *parent = 0)
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


#include "piechart.h"
#include <QtMath>
void PieChartPainter::synchronize(QNanoQuickItem *item)
{
    // Setting values here synchronized
    PieChart *realItem = static_cast<PieChart*>(item);
    if (realItem) {
        m_animation = realItem->animation();
        m_animationProgress = realItem->animationProgress()/100.0;
        m_animateScale = realItem->animateScale();
        m_animateRotate = realItem->animateRotate();
        m_segmentShowStroke = realItem->segmentShowStroke();
        m_segmentStrokeColor = QNanoColor::fromQColor(realItem->segmentStrokeColor());
        m_segmentStrokeWidth = realItem->segmentStrokeWidth();
        m_percentageInnerCutout = realItem->percentageInnerCutout();
    }
}


void PieChartPainter::paint(QNanoPainter *p)
{

    qreal segmentTotal = 0.0;
    for (auto i=0; i<m_data.size(); i++) {
        segmentTotal += m_data[i].m_value;
    }


    qreal pieRadius = qMin(height()/2,width()/2) - 5;

    qreal cumulativeAngle = -M_PI_2;

    qreal scaleAnimation = 1;
    qreal rotateAnimation = 1;

    if (m_animation) {
        if (m_animateScale) {
            scaleAnimation = m_animationProgress;
        }
        if (m_animateRotate) {
            rotateAnimation = m_animationProgress;
        }
    }

   for (auto it = m_data.cbegin(); it != m_data.cend(); ++it) {
       qreal segmentAngle = rotateAnimation * ((it->m_value/segmentTotal) * (M_PI*2));
       p->beginPath();
       p->arc(width()/2,height()/2,scaleAnimation * pieRadius,cumulativeAngle,cumulativeAngle + segmentAngle);
       p->lineTo(width()/2,height()/2);
       p->closePath();
       p->setFillStyle(it->m_color);
       p->fill();
       if(m_segmentShowStroke) {
                p->setLineWidth(m_segmentStrokeWidth);
                p->setStrokeStyle(m_segmentStrokeColor);
                p->stroke();
        }
        cumulativeAngle += segmentAngle;
    }

    QLatin1String text("PieChart");
    QRectF box = p->textBoundingBox(text,0,0);
    p->setTextAlign(QNanoPainter::ALIGN_CENTER);
    box.setWidth(width());
    box.setY(box.height());
    QNanoFont f;
    f.setPixelSize(25);
    f.setBlur(2);
    p->setFont(f);
    p->setFillStyle(0xFFf8f8f8);
    p->fillText(text,box);
    f.setBlur(0);
    p->setFont(f);
    p->setFillStyle(0xFF000000);
    p->fillText(text,box);


}


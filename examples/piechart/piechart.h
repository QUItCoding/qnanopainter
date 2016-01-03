#ifndef PIECHART
#define PIECHART

#include "qnanoquickitem.h"
#include "qnanoquickitempainter.h"

// PieChartPainter contains the painting code
class PieChartPainter : public QNanoQuickItemPainter
{
    Q_OBJECT

public:
    PieChartPainter()
    {
        m_data.push_back(Data(10,QNanoColor::fromQColor(Qt::green)));
        m_data.push_back(Data(20,QNanoColor::fromQColor(Qt::darkBlue)));
        m_data.push_back(Data(40,QNanoColor::fromQColor(Qt::magenta)));
        m_data.push_back(Data(50,QNanoColor::fromQColor(Qt::darkGray)));
        m_data.push_back(Data(30,QNanoColor::fromQColor(Qt::red)));
        m_data.push_back(Data(90,QNanoColor::fromQColor(Qt::blue)));
        m_data.push_back(Data(70,QNanoColor::fromQColor(Qt::darkYellow)));

    }
    void synchronize(QNanoQuickItem *item);
    void paint(QNanoPainter *p);

    class Data {
      public:
        Data()
            :m_value(0)
        {}
        Data(qreal v,const QNanoColor & c)
            :m_value(v)
            ,m_color(c)
        {}

      public:
      qreal m_value;
      QNanoColor m_color;
    };
private:
    qreal       m_animationProgress;
    bool        m_animation;
    bool        m_animateScale;
    bool        m_animateRotate;
    bool        m_segmentShowStroke;
    QNanoColor  m_segmentStrokeColor;
    qreal       m_segmentStrokeWidth;
    qreal       m_percentageInnerCutout;
    QVector<Data>     m_data;
};
#define PIE_PROPERTY(type,name) QNANO_PROPERTY(type, m_##name, name, set##name)

// PieChart provides the API towards QML
class PieChart : public QNanoQuickItem
{
    Q_OBJECT
    PIE_PROPERTY(bool,segmentShowStroke)
    PIE_PROPERTY(bool,animation)
    PIE_PROPERTY(bool,animateScale)
    PIE_PROPERTY(bool,animateRotate)
    PIE_PROPERTY(int,animationProgress)
    PIE_PROPERTY(QColor,segmentStrokeColor)
    PIE_PROPERTY(qreal,segmentStrokeWidth)
    PIE_PROPERTY(qreal,percentageInnerCutout)
public:
    PieChart(QQuickItem *parent = 0)
        :  QNanoQuickItem(parent)
        ,m_segmentShowStroke(true)
        ,m_animation(false)
        ,m_animateScale(false)
        ,m_animateRotate(false)
        ,m_animationProgress(0)
        ,m_segmentStrokeColor("#fff")
        ,m_segmentStrokeWidth(2.0)
        ,m_percentageInnerCutout(50.0)
    {
    }

    // Reimplement
    QNanoQuickItemPainter *createItemPainter() const
    {
        // Create painter for this item
        return new PieChartPainter();
    }
};

#endif // PIECHART


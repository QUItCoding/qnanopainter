#ifndef PIECHART
#define PIECHART

#include "qnanoquickitem.h"
#include "qnanoquickitempainter.h"
#include <QPointer>
#include <QAbstractListModel>

// PieChartPainter contains the painting code
class PieChartPainter : public QNanoQuickItemPainter
{
    Q_OBJECT

public:
    PieChartPainter()
    {

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
    qreal       m_totalValue;
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

    Q_PROPERTY(QVariant model READ model WRITE setModel NOTIFY modelChanged)

    QVector<PieChartPainter::Data> m_data;
    qreal       m_totalValue;
    QPointer<QAbstractListModel> m_model;
    QVariant m_dataSource;
    bool m_dataSourceIsObject : 1;

public:
    PieChart(QQuickItem *parent = 0);
    ~PieChart();
    // Reimplement
    QNanoQuickItemPainter *createItemPainter() const
    {
        // Create painter for this item
        return new PieChartPainter();
    }

    QVariant model() const;
    void setModel(const QVariant &);

    QVector<PieChartPainter::Data> data(qreal * ptotalValue) {
        *ptotalValue = m_totalValue;
        return m_data;
    }
protected:

    void updateData();
    void updateTotalValue() {
        m_totalValue = 0.0;
        for (auto it = m_data.cbegin(); it != m_data.cend(); ++it) {
            m_totalValue += it->m_value;
        }

    }

Q_SIGNALS:
    void modelChanged();

private Q_SLOTS:
    void dataChanged(const QModelIndex & topLeft, const QModelIndex & bottomRight, const QVector<int> & roles );


};

#endif // PIECHART


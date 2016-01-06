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
        m_data = realItem->data(&m_totalValue);

    }
}


void PieChartPainter::paint(QNanoPainter *p)
{
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
       qreal segmentAngle = rotateAnimation * ((it->m_value/m_totalValue) * (M_PI*2));
       p->beginPath();
       p->arc(width()/2,height()/2,scaleAnimation * pieRadius,cumulativeAngle,cumulativeAngle + segmentAngle);
       p->lineTo(width()/2,height()/2);
       p->closePath();
       p->setFillStyle(it->m_color);
       p->fill();
       if(m_segmentShowStroke) {
                p->setLineJoin(QNanoPainter::JOIN_ROUND);
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


PieChart::PieChart(QQuickItem *parent)
    :  QNanoQuickItem(parent)
    ,m_segmentShowStroke(true)
    ,m_animation(false)
    ,m_animateScale(false)
    ,m_animateRotate(false)
    ,m_animationProgress(0)
    ,m_segmentStrokeColor("#fff")
    ,m_segmentStrokeWidth(2.0)
    ,m_percentageInnerCutout(50.0)
    ,m_model(0)
    ,m_dataSourceIsObject(false)

{
    m_data.push_back(PieChartPainter::Data(10,QNanoColor::fromQColor(Qt::green)));
    m_data.push_back(PieChartPainter::Data(20,QNanoColor::fromQColor(Qt::darkBlue)));
    m_data.push_back(PieChartPainter::Data(40,QNanoColor::fromQColor(Qt::magenta)));
    m_data.push_back(PieChartPainter::Data(50,QNanoColor::fromQColor(Qt::darkGray)));
    m_data.push_back(PieChartPainter::Data(30,QNanoColor::fromQColor(Qt::red)));
    m_data.push_back(PieChartPainter::Data(90,QNanoColor::fromQColor(Qt::blue)));
    m_data.push_back(PieChartPainter::Data(70,QNanoColor::fromQColor(Qt::darkYellow)));
    updateTotalValue();
}

PieChart::~PieChart()
{
}
QVariant PieChart::model() const
{

    if (m_dataSourceIsObject) {
        QObject *o = m_model;
        return QVariant::fromValue(o);
    }

    return m_dataSource;
}

void PieChart::setModel(const QVariant &m)
{
    QVariant model = m;
    if (model.userType() == qMetaTypeId<QJSValue>())
        model = model.value<QJSValue>().toVariant();

    if (m_dataSource == model)
        return;

    if (m_model) {
        disconnect(m_model, &QAbstractListModel::dataChanged,
                this, &PieChart::dataChanged);
    }
    m_dataSource = model;
    QObject *object = qvariant_cast<QObject*>(model);
    m_dataSourceIsObject = object != 0;
    QAbstractListModel *alm = 0;
    if (object && (alm = qobject_cast<QAbstractListModel *>(object))) {
        m_model = alm;
    }
    if (m_model) {
        connect(m_model, &QAbstractListModel::dataChanged,
                this, &PieChart::dataChanged);
        updateData();
    }
    emit modelChanged();
}

void PieChart::updateData() {
    QVariant m = model();
    QObject *object = qvariant_cast<QObject*>(m);
    QAbstractListModel *alm = 0;
    if (object && (alm = qobject_cast<QAbstractListModel *>(object))) {
        m_data.clear();
        int roleValue = alm->roleNames().key(QByteArray("value"));
        int roleColor = alm->roleNames().key(QByteArray("color"));

        for (int row = 0; row < alm->rowCount(); ++row) {
            QModelIndex index = alm->index(row);
            QString color = alm->data(index,roleColor).value<QString>();
            double value = alm->data(index,roleValue).toDouble();
            m_data.push_back(PieChartPainter::Data(value,QNanoColor::fromQColor(QColor(color))));
        }
    }
    updateTotalValue();
    update();
}

void PieChart::dataChanged(const QModelIndex & /*topLeft*/, const QModelIndex & /*bottomRight*/, const QVector<int> & /*roles*/)
{
    updateData();
}

#ifndef TPCHART_H
#define TPCHART_H

#include <QtCharts/QChart>
#include <QTimer>
#include <QChart>
#include <QLineSeries>

QT_CHARTS_BEGIN_NAMESPACE
//class QSplineSeries;
class QLineSeries;
class QValueAxis;
QT_CHARTS_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE


class TPChart : public QChart
{
    Q_OBJECT
public:
    TPChart(QGraphicsItem *parent = 0, Qt::WindowFlags wFlags = 0);
    virtual ~TPChart();

public slots:
    void handleTimeout();

private:
    QTimer m_timer;
    QLineSeries *m_series;
    QStringList m_titles;
    QValueAxis *m_axis;
    qreal m_step;
    qreal m_x;
    qreal m_y;
};

#endif // TPCHART_H

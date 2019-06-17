#include "tpchart.h"
#include <QtCharts/QAbstractAxis>
#include <QtCharts/QValueAxis>
#include <QtCore/QTime>
#include <QtCore/QDebug>

TPChart::TPChart(QGraphicsItem *parent, Qt::WindowFlags wFlags):
    QChart(QChart::ChartTypeCartesian, parent, wFlags),
    m_series(0),
    m_axis(new QValueAxis),
    m_step(0),
    m_x(5),
    m_y(1)
{
    //setTitle("Throughput");
    setAnimationOptions(QChart::AllAnimations);
    //legend
    legend()->setAlignment(Qt::AlignRight);
    //qDebug() << "legend:" << legend()->geometry();


    //test
    qsrand((uint) QTime::currentTime().msec());

    QObject::connect(&m_timer, &QTimer::timeout, this, &TPChart::handleTimeout);
    m_timer.setInterval(1000);

    m_series = new QLineSeries(this);

    QPen green(Qt::red);
    green.setWidth(3);
    m_series->setPen(green);
    m_series->append(m_x, m_y);

    addSeries(m_series);
    createDefaultAxes();
    setAxisX(m_axis, m_series);
    m_axis->setTickCount(5);
    axisX()->setRange(0, 10);
    axisY()->setRange(-5, 10);

    m_timer.start();

}


TPChart::~TPChart()
{

}

void TPChart::handleTimeout()
{
    qreal x = plotArea().width() / m_axis->tickCount();
    qreal y = (m_axis->max() - m_axis->min()) / m_axis->tickCount();
    m_x += y;
    m_y = qrand() % 5 - 2.5;
    m_series->append(m_x, m_y);
    scroll(x, 0);
    if (m_x == 100)
        m_timer.stop();
}

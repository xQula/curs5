#ifndef HISTOGRAMSAIRPORTCONGESTION_H
#define HISTOGRAMSAIRPORTCONGESTION_H

#include <QObject>
#include <QChart>
#include <QBarSet>
#include <QBarSeries>
#include <QBarCategoryAxis>
#include <QValueAxis>
#include <QChartView>

class HistogramsAirportCongestion : public QObject
{
    Q_OBJECT
public:
    explicit HistogramsAirportCongestion(QObject *parent = nullptr);
    ~HistogramsAirportCongestion();
    void addHistograms(const QVector<uint32_t> &departure_data,const  QVector<uint32_t> &arrival_data );
    void clear_histogram();
signals:
    void sig_send_histogram_view(QChartView *chart_view);
private:
    QVector<QBarSet *> bar_set;
    QBarSeries *series;
    QChart *chart;
    QBarCategoryAxis *category_axis;
    QValueAxis *value_axis;
    QChartView *chart_view;
};

#endif // HISTOGRAMSAIRPORTCONGESTION_H

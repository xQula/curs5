#ifndef AIRPORTCONGESTIONSHEDULE_H
#define AIRPORTCONGESTIONSHEDULE_H

#include <QObject>
#include <QChart>
#include <QLineSeries>
#include <QChartView>
#include <QCategoryAxis>
#include <QValueAxis>

class AirportCongestionShedule : public QObject
{
    Q_OBJECT
public:
    explicit AirportCongestionShedule(QObject *parent = nullptr);
    ~AirportCongestionShedule();
    void add_data_graph(const QVector<uint32_t> &data_departure, const QVector<uint32_t> &data_arrival, const int num_month);
    void clear_graph();
signals:
    void sig_send_graph_view(QChartView * view);
private:
    QLineSeries* graph_departure;
    QLineSeries* graph_arrival;
    QChart *chart;
    QChartView *chart_view;
    QCategoryAxis *categoty_axis;
    QValueAxis *value_axis;
};

#endif // AIRPORTCONGESTIONSHEDULE_H

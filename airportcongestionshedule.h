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
    void add_data_graph(const int num_month, const int num_days);
    void add_data_shipments( QVector<QVector<uint32_t>> &departure, QVector<QVector<uint32_t>> &arrival);
    void clear_graph();
signals:
    void sig_send_graph_view(QChartView * view);
private:
    QLineSeries* graph_departure;
    QLineSeries* graph_arrival;
    QChart *chart;
    QChartView *chart_view;
    QValueAxis *value_axis_y;
    QValueAxis *value_axis_x;
    QVector<QVector<uint32_t>> departure_data;
    QVector<QVector<uint32_t>> arrival_data;
};

#endif // AIRPORTCONGESTIONSHEDULE_H

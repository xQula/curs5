#include "airportcongestionshedule.h"


AirportCongestionShedule::AirportCongestionShedule(QObject *parent)
{
    graph_departure = new QLineSeries();
    graph_arrival = new QLineSeries();
    chart = new QChart();
    chart_view = new QChartView();
    value_axis = new QValueAxis();
    categoty_axis = new QCategoryAxis();
}

AirportCongestionShedule::~AirportCongestionShedule()
{
    delete graph_departure;
    delete graph_arrival;
    delete chart;
    delete chart_view;
    delete value_axis;
    delete categoty_axis;
}

void AirportCongestionShedule::add_data_graph(const QVector<uint32_t> &data_departure, const QVector<uint32_t> &data_arrival, const int num_month)
{
    int max = 0;
    {
        auto max_one = *std::max_element(data_departure.begin(), data_departure.end());
        auto max_two = *std::max_element(data_arrival.begin(), data_arrival.end());
        max = max_one > max_two ? max_one : max_two;
    }
    {
        int var_max = 25;
        while(true){
            if(var_max < max){
                var_max += 50;
            }else{
                max = var_max;
                break;
            }
        }
    }
    for(QVector<uint32_t>::size_type i = 0; i < data_departure.size(); ++i){
        graph_departure->append(i, data_departure[i]);

    }
    for(QVector<uint32_t>::size_type i = 0; i < data_arrival.size(); ++i){
        graph_arrival->append(i, data_arrival[i]);
    }
    graph_arrival->setName("Прилеты");
    graph_departure->setName("Вылеты");
    chart->setTitle("Месячная загруженность аэропорта");
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->addSeries(graph_arrival);
    chart->addSeries(graph_departure);
    for(auto i = 1; i <= num_month+1; ++i){
        categoty_axis->append(QString::number(i), i);
    }
    chart->addAxis(categoty_axis, Qt::AlignBottom);
    graph_arrival->attachAxis(categoty_axis);
    graph_departure->attachAxis(categoty_axis);
    value_axis->setRange(0, max);
    chart->addAxis(value_axis, Qt::AlignLeft);
    graph_arrival->attachAxis(value_axis);
    graph_departure->attachAxis(value_axis);
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignRight);
    chart_view->setChart(chart);
    chart_view->setRenderHint(QPainter::Antialiasing);
    emit sig_send_graph_view(chart_view);
}

void AirportCongestionShedule::clear_graph()
{
    if(!chart->series().empty()){
        graph_arrival->clear();
        graph_departure->clear();
        chart->removeSeries(graph_arrival);
        chart->removeSeries(graph_departure);
        delete categoty_axis;
        delete value_axis;
        categoty_axis = new QCategoryAxis();
        value_axis = new QValueAxis();
    }
}

#include "airportcongestionshedule.h"


AirportCongestionShedule::AirportCongestionShedule(QObject *parent)
{
    graph_departure = new QLineSeries();
    graph_arrival = new QLineSeries();
    chart = new QChart();
    chart_view = new QChartView();
    value_axis_y = new QValueAxis();
    value_axis_x = new QValueAxis();
}

AirportCongestionShedule::~AirportCongestionShedule()
{
    delete graph_departure;
    delete graph_arrival;
    delete chart;
    delete chart_view;
    delete value_axis_y;
    delete value_axis_x;
}

void AirportCongestionShedule::add_data_graph(const int num_month, const int num_days)
{
    int max = 0;
    {
        auto max_one = *std::max_element(departure_data[num_month].begin(), departure_data[num_month].end());
        auto max_two = *std::max_element(arrival_data[num_month].begin(), arrival_data[num_month].end());
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
    for(QVector<uint32_t>::size_type i = 0; i < departure_data[num_month].size(); ++i){
        graph_departure->append(i+1, departure_data[num_month][i]);

    }
    for(QVector<uint32_t>::size_type i = 0; i < arrival_data[num_month].size(); ++i){
        graph_arrival->append(i+1, arrival_data[num_month][i]);
    }
    graph_arrival->setName("Прилеты");
    graph_departure->setName("Вылеты");
    chart->setTitle("Месячная загруженность аэропорта");
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->addSeries(graph_arrival);
    chart->addSeries(graph_departure);
    value_axis_x->setRange(1, num_days);
    value_axis_x->setTickCount(num_days);
    value_axis_x->setLabelFormat("%.i");
    value_axis_x->setTitleText("Дни месяца");
    chart->addAxis(value_axis_x, Qt::AlignBottom);
    graph_arrival->attachAxis(value_axis_x);
    graph_departure->attachAxis(value_axis_x);
    value_axis_y->setRange(0, max);
    chart->addAxis(value_axis_y, Qt::AlignLeft);
    graph_arrival->attachAxis(value_axis_y);
    graph_departure->attachAxis(value_axis_y);
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignRight);
    chart_view->setChart(chart);
    chart_view->setRenderHint(QPainter::Antialiasing);
    emit sig_send_graph_view(chart_view);
}

void AirportCongestionShedule::add_data_shipments(QVector<QVector<uint32_t> > &departure, QVector<QVector<uint32_t> > &arrival)
{
    departure_data = std::move(departure);
    arrival_data = std::move(arrival);
}

void AirportCongestionShedule::clear_graph()
{
    if(!chart->series().empty()){
        graph_arrival->clear();
        graph_departure->clear();
        chart->removeSeries(graph_arrival);
        chart->removeSeries(graph_departure);
        delete value_axis_y;
        delete value_axis_x;
        value_axis_y = new QValueAxis();
        value_axis_x = new QValueAxis();
    }
}

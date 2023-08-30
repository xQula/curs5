#include "histogramsairportcongestion.h"



HistogramsAirportCongestion::HistogramsAirportCongestion(QObject *parent)
{
    bar_set.resize(2);
    bar_set[0] = new QBarSet("Прилеты");
    bar_set[1] = new QBarSet("Вылеты");
    series = new QBarSeries();
    chart = new QChart();
    category_axis = new QBarCategoryAxis();
    value_axis = new QValueAxis();
    chart_view = new QChartView();
}

HistogramsAirportCongestion::~HistogramsAirportCongestion()
{
    for(auto &i: bar_set){
        delete i;
    }
    delete series;
    delete chart;
    delete category_axis;
    delete value_axis;
    delete chart_view;
}

void HistogramsAirportCongestion::addHistograms(const QVector<uint32_t> &departure_data, const QVector<uint32_t> &arrival_data)
{
    int max = 0;
    {
        auto max_one = *std::max_element(departure_data.begin(), departure_data.end());
        auto max_two = *std::max_element(arrival_data.begin(), arrival_data.end());
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
    for(QVector<uint32_t>::size_type i = 0; i < departure_data.size(); ++i){
        bar_set[0]->append(departure_data[i]);
        bar_set[1]->append(arrival_data[i]);
    }
    for(const auto &i: bar_set){
        series->append(i);
    }
    chart->addSeries(series);
    chart->setTitle("Годовая загруженность аэропорта");
    chart->setAnimationOptions(QChart::SeriesAnimations);
    QStringList categoris;
    categoris << "Сентябрь 2016" << "Октябрь 2016" << "Ноябрь 2016"
              << "Декабрь 2016" << "Январь 2017" << "Февраль 2017"
              << "Март 2017" << "Апрель 2017" << "Май 2017"
              << "Июнь 2017" << "Июль 2017" << "Агуст 2017";
    category_axis->append(categoris);
    chart->addAxis(category_axis, Qt::AlignBottom);
    series->attachAxis(category_axis);
    value_axis->setRange(0, max);
    chart->addAxis(value_axis, Qt::AlignLeft);
    series->attachAxis(value_axis);
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignRight);
    chart_view->setChart(chart);
    chart_view->setRenderHint(QPainter::Antialiasing);
    emit sig_send_histogram_view(chart_view);
}

void HistogramsAirportCongestion::clear_histogram()
{
    if(!chart->series().empty()){
        series->clear();
        chart->removeSeries(series);
        bar_set[0] = new QBarSet("Прилеты");
        bar_set[1] = new QBarSet("Вылеты");
        delete category_axis;
        delete value_axis;
        category_axis = new QBarCategoryAxis();
        value_axis = new QValueAxis();
    }
}

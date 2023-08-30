#ifndef STATISTICSAIRPORT_H
#define STATISTICSAIRPORT_H

#include <QDialog>
#include <QChartView>
#include <QGridLayout>

namespace Ui {
class StatisticsAirport;
}

class StatisticsAirport : public QDialog
{
    Q_OBJECT

public:
    explicit StatisticsAirport(QWidget *parent = nullptr);
    ~StatisticsAirport();
    void add_wd_histogram(QChartView *chart_view);
    void add_wd_graph(QChartView *chart_view);
    int getting_index_month();
    void change_month();
private slots:
    void on_tabWidget_tabBarClicked(int index);


    void on_cb_month_currentIndexChanged(int index);
    void on_pb_closeStatusticAirport_clicked();

signals:
    void sig_send_change_month(int month);

private:
    Ui::StatisticsAirport *ui;
    QGridLayout* gridLayotHistogram;
    QGridLayout* gridLayotGraphic;
};

#endif // STATISTICSAIRPORT_H

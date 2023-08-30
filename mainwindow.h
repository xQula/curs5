#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QMessageBox>
#include <QTimer>
#include "airportdatabase.h"
#include "dataforconnectingdatabase.h"
#include "statisticsairport.h"
#include "histogramsairportcongestion.h"
#include "airportcongestionshedule.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void stat_bar_uncon();
    void stat_bar_con();
    void turning_off_models();
    void turning_on_models();
    void send_reqest_db_for_histogram();
    int getting_days_from_month(int month);
    bool date_limit(QDate *date);

private slots:
    void sl_getting_the_status_db(bool status);
    void sl_getting_the_error_db(QSqlError err);
    void sl_getting_the_query_model_db_cb_airoport(QSqlQueryModel *model);
    void sl_getting_the_query_model_db_tw_airoport(QSqlQueryModel *model);
    void sl_getting_the_query_model_from_graph(QSqlQueryModel *query_model_departure, QSqlQueryModel *query_model_arrival);
    void sl_getting_the_query_model_from_histogram(QSqlQueryModel *query_model_departure, QSqlQueryModel *query_model_arrival);
    void sl_add_wd_histogram_airport(QChartView *wd);
    void sl_add_wd_graph_airport(QChartView *wd);
    void sl_send_reqest_db_for_graph(int month);
    void on_act_connect_triggered();
    void on_pb_findBD_clicked();
    void on_pb_statistics_clicked();

private:
    Ui::MainWindow *ui;
    QLabel *label;
    AirportDatabase *data_base;
    DataForConnectingDataBase *data_connecting_db;
    QMessageBox *msg;
    StatisticsAirport *ui_stat_airport;
    QTimer *timer;
    HistogramsAirportCongestion *histograms;
    AirportCongestionShedule *shedule;
};
#endif // MAINWINDOW_H

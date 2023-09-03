#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , ui_stat_airport(new StatisticsAirport)
{
    ui->setupUi(this);
    timer = new QTimer(this);
    histograms = new HistogramsAirportCongestion(this);
    shedule = new AirportCongestionShedule(this);
    timer->setInterval(5000);
    ui->cb_bdAirport->setFixedWidth(150);
    ui->de_dataDeparture->setFixedWidth(150);
    ui->cb_where->setFixedWidth(150);
    label = new QLabel(this);
    stat_bar_uncon();
    turning_off_models();
    data_base = new AirportDatabase(this);
    data_connecting_db = new DataForConnectingDataBase();
    msg = new QMessageBox(this);
    data_base->add_data_base(POSTGRE_DRIVER, DB_NAME);
    ui->de_dataDeparture->setMinimumDate({2016,8,15});
    ui->de_dataDeparture->setMaximumDate({2017,9,14});

    connect(data_base, &AirportDatabase::sig_send_status_connection, this, &MainWindow::sl_getting_the_status_db);
    connect(data_base, &AirportDatabase::sig_send_err_db, this, &MainWindow::sl_getting_the_error_db);
    connect(data_base, &AirportDatabase::sig_send_data_from_db_cb_airoport, this, &MainWindow::sl_getting_the_query_model_db_cb_airoport);
    connect(data_base, &AirportDatabase::sig_send_data_from_db_tw_airoport, this, &MainWindow::sl_getting_the_query_model_db_tw_airoport);
    connect(timer, &QTimer::timeout, this, &MainWindow::on_act_connect_triggered);
    connect(histograms, &HistogramsAirportCongestion::sig_send_histogram_view, this, &MainWindow::sl_add_wd_histogram_airport);
    connect(shedule, &AirportCongestionShedule::sig_send_graph_view, this, &MainWindow::sl_add_wd_graph_airport);
    connect(ui_stat_airport, &StatisticsAirport::sig_send_change_month, this, [this]{shedule->clear_graph() ;shedule->add_data_graph(ui_stat_airport->getting_index_month(), getting_days_from_month(ui_stat_airport->getting_index_month()));});
    connect(data_base, &AirportDatabase::sig_send_data_from_db_from_graph, this, &MainWindow::sl_getting_the_query_model_from_graph);
    connect(data_base, &AirportDatabase::sig_send_data_from_db_from_histogram, this, &MainWindow::sl_getting_the_query_model_from_histogram);

    on_act_connect_triggered();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete ui_stat_airport;
    delete data_connecting_db;
}

void MainWindow::stat_bar_uncon()
{
    label->setText("Не подкюченно");
    label->setStyleSheet("QLabel { color : red; }");
    ui->statusbar->addWidget(label);
}

void MainWindow::stat_bar_con()
{
    label->setText("Подкюченно");
    label->setStyleSheet("QLabel { color : green; }");
    ui->statusbar->addWidget(label);
}

void MainWindow::turning_off_models()
{
    ui->cb_bdAirport->setEnabled(false);
    ui->de_dataDeparture->setEnabled(false);
    ui->pb_findBD->setEnabled(false);
    ui->pb_statistics->setEnabled(false);
    ui->cb_where->setEnabled(false);
}

void MainWindow::turning_on_models()
{
    ui->cb_bdAirport->setEnabled(true);
    ui->de_dataDeparture->setEnabled(true);
    ui->pb_findBD->setEnabled(true);
    ui->pb_statistics->setEnabled(true);
    ui->cb_where->setEnabled(true);
}

void MainWindow::send_reqest_db_for_histogram()
{
    histograms->clear_histogram();
    QString code_airport = data_base->get_code_airport_from_model_bd(ui->cb_bdAirport->currentText());
    data_base->request_to_db_from_histogram("SELECT count(flight_no), date_trunc('month', scheduled_departure) as \"Month\" from bookings.flights f "
                                            "WHERE (scheduled_departure::date > date('2016-08-31') and scheduled_departure::date <= date('2017-08-31')) and ( departure_airport = '"+ code_airport + "') "
                                            "group by \"Month\"",
                                            "SELECT count(flight_no), date_trunc('month', scheduled_departure) as \"Month\" from bookings.flights f "
                                            "WHERE (scheduled_arrival::date > date('2016-08-31') and scheduled_arrival::date <= date('2017-08-31')) and ( arrival_airport = '"+ code_airport + "') "
                                            "group by \"Month\""
);
}

int MainWindow::getting_days_from_month(int month)
{
    ++month;

    switch(month){
        case 1: return 31;
        case 2: return 28;
        case 3: return 31;
        case 4: return 30;
        case 5: return 31;
        case 6: return 30;
        case 7: return 31;
        case 8: return 31;
        case 9: return 30;
        case 10: return 31;
        case 11: return 30;
        case 12: return 31;
    }
    return{};
}

void MainWindow::sl_getting_the_status_db(bool status)
{
    if(status){
        timer->stop();
        stat_bar_con();
        turning_on_models();
        data_base->request_to_db_cb_airport("SELECT airport_name->>'ru' as \"airportName\", airport_code FROM bookings.airports_data");
    }else{
        data_base->disconnect_from_db(DB_NAME);
        msg->setIcon(QMessageBox::Critical);
        msg->setText(data_base->get_last_error_db().text());
        msg->show();
        stat_bar_uncon();
        ui->act_connect->setText("Подключтить к БД");
        timer->start();
    }

}

void MainWindow::sl_getting_the_error_db(QSqlError err)
{
    msg->setText(err.text());
    msg->show();
}

void MainWindow::sl_getting_the_query_model_db_cb_airoport(QSqlQueryModel *model)
{
    ui->cb_bdAirport->setModel(model);
    ui->cb_bdAirport->show();
    ui->cb_bdAirport->update();
}

void MainWindow::sl_getting_the_query_model_db_tw_airoport(QSqlQueryModel *model)
{
    ui->tw_bd->setModel(model);
    ui->tw_bd->show();
    ui->tw_bd->update();
}

void MainWindow::sl_getting_the_query_model_from_graph(QSqlQueryModel *query_model_departure, QSqlQueryModel *query_model_arrival)
{
    QVector<QVector<uint32_t>> departure, arrival;
    departure.resize(12);
    arrival.resize(12);
    for(int i = 0; i < query_model_departure->rowCount(); ++i){
        departure[query_model_departure->record(i).value(1).toDate().toString("M").toInt()-1].push_back(query_model_departure->record(i).value(0).toUInt());
    }
    for(int i = 0; i < query_model_departure->rowCount(); ++i){
        arrival[query_model_arrival->record(i).value(1).toDate().toString("M").toInt()-1].push_back(query_model_arrival->record(i).value(0).toUInt());
    }
    shedule->add_data_shipments(departure, arrival);
}

void MainWindow::sl_getting_the_query_model_from_histogram(QSqlQueryModel *query_model_departure, QSqlQueryModel *query_model_arrival)
{
    QVector<uint32_t> departure, arrival;
    for(int i = 0;  i < query_model_departure->rowCount(); ++i){
        departure.push_back(query_model_departure->record(i).value(0).toUInt());
    }
    for(int i = 0;  i < query_model_arrival->rowCount(); ++i){
        arrival.push_back(query_model_arrival->record(i).value(0).toUInt());
    }
    histograms->addHistograms(departure, arrival);
}

void MainWindow::sl_add_wd_histogram_airport(QChartView *wd)
{
    ui_stat_airport->add_wd_histogram(wd);
}

void MainWindow::sl_add_wd_graph_airport(QChartView *wd)
{
    ui_stat_airport->add_wd_graph(wd);
}

void MainWindow::sl_send_reqest_db_for_graph()
{
    shedule->clear_graph();
    QString code_airport = data_base->get_code_airport_from_model_bd(ui->cb_bdAirport->currentText());
    data_base->request_to_db_from_graph("SELECT count(flight_no), date_trunc('day', scheduled_departure) as \"Day\" from bookings.flights f "
                                        "WHERE(scheduled_departure::date > date('2016-08-31') and scheduled_departure::date <= date('2017-08-31')) and ( departure_airport = '"+ code_airport +"') "
                                        "GROUP BY \"Day\"",
                                        "SELECT count(flight_no), date_trunc('day', scheduled_departure) as \"Day\" from bookings.flights f "
                                        "WHERE(scheduled_arrival::date > date('2016-08-31') and scheduled_arrival::date <= date('2017-08-31')) and ( arrival_airport = '"+ code_airport +"') "
                                        "GROUP BY \"Day\""
                                        );
}


void MainWindow::on_act_connect_triggered()
{
    if(ui->act_connect->text() == "Подключтить к БД"){
        data_base->connect_to_db({
                                data_connecting_db->hostName,
                                data_connecting_db->dbName,
                                data_connecting_db->login,
                                data_connecting_db->pass,
                                data_connecting_db->port});
        ui->act_connect->setText("Отключиться от БД");
    }else{
        data_base->disconnect_from_db(DB_NAME);
        stat_bar_uncon();
        turning_off_models();
        ui->act_connect->setText("Подключтить к БД");
    }
}



void MainWindow::on_pb_findBD_clicked()
{
    QDate date = ui->de_dataDeparture->date();
    QString code_airport = data_base->get_code_airport_from_model_bd(ui->cb_bdAirport->currentText());
    if(ui->cb_where->currentText() == "Вылет"){
        data_base->request_to_db_tw_airport("SELECT flight_no, scheduled_arrival, ad.airport_name->>'ru' as \"Name\" from bookings.flights f "
                                             "JOIN bookings.airports_data ad on ad.airport_code = f.departure_airport "
                                             "where f.arrival_airport  = '" + code_airport +
                                             "' AND TO_CHAR(f.scheduled_arrival, 'DD.MM.YYYY') LIKE '" + date.toString("dd.MM.yyyy") + "%'");
    }else{
        data_base->request_to_db_tw_airport("SELECT flight_no, scheduled_departure, ad.airport_name->>'ru' as \"Name\" from bookings.flights f "
                                            "JOIN bookings.airports_data ad on ad.airport_code = f.arrival_airport "
                                            "WHERE f.departure_airport  = '" + code_airport +
                                            "' AND TO_CHAR(f.scheduled_arrival, 'DD.MM.YYYY') LIKE '" + date.toString("dd.MM.yyyy") + "%'");
    }
}


void MainWindow::on_pb_statistics_clicked()
{
    ui_stat_airport->setModal(true);
    ui_stat_airport->show();
    ui_stat_airport->change_month();
    sl_send_reqest_db_for_graph();
    shedule->add_data_graph(ui_stat_airport->getting_index_month(), getting_days_from_month(ui_stat_airport->getting_index_month()));
    send_reqest_db_for_histogram();
}



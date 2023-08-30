#include "airportdatabase.h"

AirportDatabase::AirportDatabase(QObject *parent) : QObject{parent}
{
    data_base_ = new QSqlDatabase();
    query_model_cb_airport = new QSqlQueryModel();
    query_model_tw_airport = new QSqlQueryModel();
    query_model_graph_departue = new QSqlQueryModel();
    query_model_graph_arrival = new QSqlQueryModel();
    query_model_histogram_departue = new QSqlQueryModel();
    query_model_histogram_arrival = new QSqlQueryModel();
}
AirportDatabase::~AirportDatabase()
{
    delete data_base_;
    delete query_model_cb_airport;
    delete query_model_graph_departue;
    delete query_model_graph_arrival;
    delete query_model_histogram_departue;
    delete query_model_histogram_arrival;
    delete query_model_tw_airport;
}

void AirportDatabase::add_data_base(const QString &driver, const QString &name_db)
{
    *data_base_ = QSqlDatabase::addDatabase(driver, name_db);
}

void AirportDatabase::connect_to_db(const QVector<QString> &data_for_connect)
{
    data_base_->setHostName(data_for_connect[static_cast<int>(fieldsForConnect::hostName)]);
    data_base_->setDatabaseName(data_for_connect[static_cast<int>(fieldsForConnect::dbName)]);
    data_base_->setUserName(data_for_connect[static_cast<int>(fieldsForConnect::login)]);
    data_base_->setPassword(data_for_connect[static_cast<int>(fieldsForConnect::pass)]);
    data_base_->setPort(data_for_connect[static_cast<int>(fieldsForConnect::port)].toInt());
    bool status = data_base_->open();
    emit sig_send_status_connection(status);
}

void AirportDatabase::disconnect_from_db(const QString &namedb)
{
    *data_base_ = QSqlDatabase::database(namedb);
    data_base_->close();
}

void AirportDatabase::request_to_db_cb_airport(const QString &request)
{
    query_model_cb_airport->setQuery(request, *data_base_);
    if(!query_model_cb_airport->query().isActive()){
        emit sig_send_err_db(query_model_cb_airport->lastError());
    }
    emit sig_send_data_from_db_cb_airoport(query_model_cb_airport);
}

void AirportDatabase::request_to_db_tw_airport(const QString &request)
{
    query_model_tw_airport->setQuery(request, *data_base_);
    if(!query_model_tw_airport->query().isActive()){
        emit sig_send_err_db(query_model_tw_airport->lastError());
    }
    emit sig_send_data_from_db_tw_airoport(query_model_tw_airport);
}

void AirportDatabase::request_to_db_from_graph(const QString &request_departure, const QString &request_arrival)
{
    query_model_graph_departue->setQuery(request_departure, *data_base_);
    if(!query_model_graph_departue->query().isActive()){
        emit sig_send_err_db(query_model_graph_departue->lastError());
    }
    query_model_graph_arrival->setQuery(request_arrival, *data_base_);
    if(!query_model_graph_arrival->query().isActive()){
        emit sig_send_err_db(query_model_graph_arrival->lastError());
    }
    emit sig_send_data_from_db_from_graph( query_model_graph_departue, query_model_graph_arrival);
}

void AirportDatabase::request_to_db_from_histogram(const QString &request_departure, const QString &request_arrival)
{
    query_model_histogram_departue->setQuery(request_departure, *data_base_);
    if(!query_model_histogram_departue->query().isActive()){
        emit sig_send_err_db(query_model_histogram_departue->lastError());
    }
    query_model_histogram_arrival->setQuery(request_arrival, *data_base_);
    if(!query_model_histogram_arrival->query().isActive()){
        emit sig_send_err_db(query_model_histogram_arrival->lastError());
    }
    emit sig_send_data_from_db_from_histogram( query_model_histogram_departue, query_model_histogram_arrival);
}

QString AirportDatabase::get_code_airport_from_model_bd(const QString &name)
{
    for(uint32_t i = 0; i < query_model_cb_airport->rowCount(); ++i){
        if(query_model_cb_airport->record(i).value(0).toString() == name){
            return query_model_cb_airport->record(i).value(1).toString();
        }
    }
    return {};
}

QSqlError AirportDatabase::get_last_error_db()
{
    return data_base_->lastError();
}

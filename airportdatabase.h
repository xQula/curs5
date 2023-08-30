#ifndef AIRPORTDATABASE_H
#define AIRPORTDATABASE_H

#include <QObject>
#include <QTableWidget>
#include <QObject>
#include <QSqlError>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QSqlQueryModel>
#include <QSqlRecord>

#define POSTGRE_DRIVER "QPSQL"
#define DB_NAME "MyDB"
#define NUM_DATA_FOR_CONNECT_TO_DB 5

enum class fieldsForConnect{
    hostName = 0,
    dbName = 1,
    login = 2,
    pass = 3,
    port = 4
};

class AirportDatabase : public QObject
{
    Q_OBJECT
public:
    explicit AirportDatabase(QObject *parent = nullptr);
    ~AirportDatabase();
    void add_data_base(const QString &driver, const QString &name_db);
    void connect_to_db(const QVector<QString> &data_for_connect);
    void disconnect_from_db(const QString &nabedb);
    void request_to_db_cb_airport(const QString &request);
    void request_to_db_tw_airport(const QString &request);
    void request_to_db_from_graph(const QString &request_departure, const QString &request_arrival);
    void request_to_db_from_histogram(const QString &request_departure, const QString &request_arrival);
    QString get_code_airport_from_model_bd(const QString &name);
    QSqlError get_last_error_db();

signals:
    void sig_send_data_from_db_cb_airoport(QSqlQueryModel *query_model);
    void sig_send_data_from_db_tw_airoport(QSqlQueryModel *query_model);
    void sig_send_data_from_db_from_graph(QSqlQueryModel *query_model_departure, QSqlQueryModel *query_model_arrival);
    void sig_send_data_from_db_from_histogram(QSqlQueryModel *query_model_departure, QSqlQueryModel *query_model_arrival);
    void sig_send_status_connection(bool);
    void sig_send_err_db(QSqlError err);
private:
    QSqlDatabase *data_base_;
    QSqlQueryModel *query_model_cb_airport;
    QSqlQueryModel *query_model_tw_airport;
    QSqlQueryModel *query_model_graph_departue;
    QSqlQueryModel *query_model_graph_arrival;
    QSqlQueryModel *query_model_histogram_departue;
    QSqlQueryModel *query_model_histogram_arrival;
};

#endif // AirportDatabase_H

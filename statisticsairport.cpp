#include "statisticsairport.h"
#include "ui_statisticsairport.h"

StatisticsAirport::StatisticsAirport(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StatisticsAirport)
{
    ui->setupUi(this);
    gridLayotHistogram = new QGridLayout(ui->wd_histogram);
    gridLayotGraphic = new QGridLayout(ui->wd_graphic);
    ui->cb_month->setCurrentIndex(0);
}

StatisticsAirport::~StatisticsAirport()
{
    delete ui;
}

void StatisticsAirport::add_wd_histogram(QChartView *chart_view)
{
    gridLayotHistogram->addWidget(chart_view);
}

void StatisticsAirport::add_wd_graph(QChartView *chart_view)
{
    gridLayotGraphic->addWidget(chart_view);
}

int StatisticsAirport::getting_index_month()
{
    return ui->cb_month->currentIndex();
}

void StatisticsAirport::change_month()
{
    ui->cb_month->setCurrentIndex(0);
}

void StatisticsAirport::on_tabWidget_tabBarClicked(int index)
{
    if(index){
        ui->cb_month->show();
    }else
        ui->cb_month->hide();
}


void StatisticsAirport::on_cb_month_currentIndexChanged(int index)
{
    emit sig_send_change_month(ui->cb_month->currentIndex());
}


void StatisticsAirport::on_pb_closeStatusticAirport_clicked()
{
    QDialog::close();
}


#include "cihm.h"
#include "ui_cihm.h"

cihm::cihm(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::cihm)
{
    ui->setupUi(this);
}

cihm::~cihm()
{
    delete ui;
}


void cihm::on_pbPresence_clicked()
{
    _zdc = new CZdc();

    bool presence;

    _zdc->setPresence(true);

    presence = _zdc->getPresence();

    if(presence == true){

        ui->lePresence->setText("Présence = 1");

    }else{
        ui->lePresence->setText("Présence = 0");
    }
}


void cihm::on_pbAbsence_clicked()
{
    bool presence;

    _zdc->setPresence(false);

    presence = _zdc->getPresence();

    if(presence == true){

        ui->lePresence->setText("Présence = 1");

    }else{
        ui->lePresence->setText("Présence = 0");
    }
}

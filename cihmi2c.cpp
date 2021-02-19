#include "cihmi2c.h"
#include "ui_cihmi2c.h"

CIhmI2c::CIhmI2c(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CIhmI2c)
{
    ui->setupUi(this);
    _app = new CApp(this);
}

CIhmI2c::~CIhmI2c()
{
    delete _app;
    delete ui;
}

void CIhmI2c::on_pb0_clicked()
{

}

void CIhmI2c::on_pb50_clicked()
{

}

void CIhmI2c::on_pb100_clicked()
{

}

void CIhmI2c::on_pbJour_clicked()
{

}

void CIhmI2c::on_pbNuit_clicked()
{

}

void CIhmI2c::on_pbPr1_clicked()
{

}

void CIhmI2c::on_pbPr0_clicked()
{

}

void CIhmI2c::on_pbRouge_clicked()
{

}

void CIhmI2c::on_pbOrange_clicked()
{

}

void CIhmI2c::on_pbVert_clicked()
{

}

void CIhmI2c::on_pbModAuto_clicked()
{

}

void CIhmI2c::on_pbModOC_clicked()
{

}

void CIhmI2c::on_pbModManu_clicked()
{

}

void CIhmI2c::on_pbBEM_clicked()
{

}

void CIhmI2c::on_pbBED_clicked()
{

}

void CIhmI2c::on_pbBSM_clicked()
{

}

void CIhmI2c::on_pbBSD_clicked()
{

}

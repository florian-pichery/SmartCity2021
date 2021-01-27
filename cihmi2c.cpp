#include "cihmi2c.h"
#include "ui_cihmi2c.h"

CIhmI2c::CIhmI2c(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CIhmI2c)
{
    ui->setupUi(this);
}

CIhmI2c::~CIhmI2c()
{
    delete ui;
}

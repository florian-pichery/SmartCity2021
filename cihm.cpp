#include "cihm.h"
#include "ui_cihm.h"

CIhm::CIhm(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CIhm)
{
    ui->setupUi(this);


    // composition dynamique
    _app = new CApp();
    connect(_app, &CApp::sig_erreur, this, &CIhm::on_erreur);
    connect(_app, &CApp::sig_info, this, &CIhm::on_info);
}

CIhm::~CIhm()
{
    delete _app;
    delete ui;
}

void CIhm::on_erreur(QString mess)
{
    ui->teErreurs->append(mess);
}

void CIhm::on_info(QString mess)
{
    ui->teSuivi->append(mess);
}


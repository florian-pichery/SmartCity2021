#ifndef CIHMI2C_H
#define CIHMI2C_H

#include <QMainWindow>

#include "capp.h"
#include "czdc.h"


namespace Ui { class CIhmI2c; }

class CIhmI2c : public QMainWindow
{
    Q_OBJECT

public:
    explicit CIhmI2c(QWidget *parent = nullptr);
    ~CIhmI2c();

private slots:
//Eclairage
    void on_pb0_clicked();
    void on_pb50_clicked();
    void on_pb100_clicked();
    void on_pbJour_clicked();
    void on_pbNuit_clicked();
    void on_pbPr1_clicked();
    void on_pbPr0_clicked();
//Fin Eclairage
//Intersection
    void on_pbRouge_clicked();
    void on_pbOrange_clicked();
    void on_pbVert_clicked();
    void on_pbModAuto_clicked();
    void on_pbModOC_clicked();
    void on_pbModManu_clicked();
//Fin Intersection
//Parking
    void on_pbBEM_clicked();
    void on_pbBED_clicked();
    void on_pbBSM_clicked();
    void on_pbBSD_clicked();
//Fin Parking

private:
    Ui::CIhmI2c *ui;
    CApp *_app;
};

#endif // CIHMI2C_H

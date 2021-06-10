#ifndef CIHMI2C_H
#define CIHMI2C_H

#include <QMainWindow>
#include <QTextBlock>
#include <QTextCursor>
#include <QTimer>

#include "capp.h"
#include "czdc.h"


namespace Ui { class CIhm; }

class CIhm : public QMainWindow
{
    Q_OBJECT

public:
    explicit CIhm(QWidget *parent = nullptr);
    ~CIhm();

private:
    Ui::CIhm *ui;
    CApp *_app;
    int _compteur = 0;

public slots:
    void on_sigMsgConsigne(QString msg_consigne);
    void on_sigMsginterOrdre1(QString msg_interOrdre1);
    void on_sigMsginterOrdre2(QString msg_interOrdre2);
    void on_sigMsgMode(QString msg_mode);
    void on_sigMsgParkOrdreE(QString msg_parkOrdre);
    void on_sigMsgParkOrdreS(QString msg_parkOrdre);


private slots:
//Définition des adresses I2C de test
    void on_leAddrEclair_returnPressed();//Eclairage
    void on_leAddrInter_returnPressed();//Intersection
    void on_leAddrPark_returnPressed();//Parking
//Eclairage
    void on_cb_0_clicked(bool checked);
    void on_cb_50_clicked(bool checked);
    void on_cb_100_clicked(bool checked);
//Fin Eclairage
//Intersection
    void on_rbAuto_clicked(bool checked);
    void on_rbCligno_clicked(bool checked);
    void on_rbManu_clicked(bool checked);
    void on_comboBoxVoie1_currentIndexChanged(int index);
    void on_comboBoxVoie2_currentIndexChanged(int index);
//Fin Intersection
//Parking
    void on_cbMontEnt_clicked(bool checked);
    void on_cbDescEnt_clicked(bool checked);
    void on_cbMontSor_clicked(bool checked);
    void on_cbDescSor_clicked(bool checked);
    void on_leSup_returnPressed();
    void on_leInf_returnPressed();
    void on_pbPlus_clicked();
    void on_pbMoins_clicked();
//Fin Parking

    void on_erreur(QString mess);
    void on_info(QString mess);
};

#endif // CIHMI2C_H

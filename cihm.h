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

public slots:
    void on_sigMsgConsigne(QString msg_consigne);
    void on_sigMsgCellule(QString msg_cellule);
    void on_sigMsgPresence(QString msg_presence);
    void on_sigMsginterOrdre1(QString msg_interOrdre1);
    void on_sigMsginterOrdre2(QString msg_interOrdre2);
    void on_sigMsgMode(QString msg_mode);


private slots:
//Définition des adresses I2C de test
    void on_leAddrEclair_returnPressed();//Eclairage
    void on_leAddrInter_returnPressed();//Intersection
    void on_leAddrPark_returnPressed();//Parking
//Eclairage
    void on_cb_0_clicked(bool checked);
    void on_cb_50_clicked(bool checked);
    void on_cb_100_clicked(bool checked);
    void on_cb_jour_clicked(bool checked);
    void on_cb_nuit_clicked(bool checked);
    void on_cb_presence_clicked(bool checked);
    void on_cb_absence_clicked(bool checked);
//Fin Eclairage
//Intersection
    void on_cb_red_clicked(bool checked);
    void on_cb_red2_clicked(bool checked);
    void on_cb_orange_clicked(bool checked);
    void on_cb_orange2_clicked(bool checked);
    void on_cb_green_clicked(bool checked);
    void on_cb_green2_clicked(bool checked);
    void on_cb_auto_clicked(bool checked);
//Fin Intersection
//Parking
//Fin Parking

    void on_erreur(QString mess);
    void on_info(QString mess);

    void on_cb_cligno_clicked(bool checked);
    void on_cb_manuel_clicked(bool checked);
};

#endif // CIHMI2C_H

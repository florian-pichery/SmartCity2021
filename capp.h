#ifndef CAPP_H
#define CAPP_H

#include <QObject>
#include <QThread>

#include "cgestionmaquette.h"
#include "czdc.h"
#include "cconfig.h"
#include "cgererserveur.h"

#define PORT 2222//confs

class CApp : public QObject
{
    Q_OBJECT
public:
    explicit CApp(QObject *parent = nullptr);
    ~CApp();

//Pour l'IHM

//Adressage
    void setAddrEclair(uint8_t addrEclair);
    uint8_t getAddrEclair();
    void setAddrInter(uint8_t addrInter);
    uint8_t getAddrInter();
    void setAddrPark(uint8_t addrPark);
    uint8_t getAddrPark();
//Fin Adressage

//Eclairage
    void setConsigne(uint8_t consigne);
    void setCellule(bool cellule);
    void setPresence(bool presence);
//Fin Eclairage

//Intersection
     void setInterOrdre1(uint8_t interOrdre);
     void setInterOrdre2(uint8_t interOrdre);
     void setMode(uint8_t mode);
//Fin Intersection

//Parking
    void setParkOrdre(uint8_t parkOrdre);
    void setRFID(QByteArray rfid);
//Fin Parking

private:
    CZdc *_zdc;
    CGestionMaquette *_maquette;
    CConfig *_config;
    QThread *_gthm;
    CGererServeur *_serv;

signals:
    void sig_go();
    /*POUR L'IHM*/
    //Éclairage
    void sig_msgConsigne(QString msg_consigne);
    void sig_msgCellule(QString msg_cellule);
    void sig_msgPresence(QString msg_presence);
    //Fin éclairage
    //Intersection
    void sig_msgInterOrdre1(QString msg_interOrdre1);
    void sig_msgInterOrdre2(QString msg_interOrdre2);
    void sig_msgMode(QString msg_mode);
    //Fin intersection
    //Parking
    void sig_msgParkOrdreE(QString msg_parkOrdreE);
    void sig_msgParkOrdreS(QString msg_parkOrdreS);
    //Fin parking

    void sig_erreur(QString mess);
    void sig_info(QString mess);

private slots:

    void on_erreur(QString mess);
    void on_info(QString mess);

};

#endif // CAPP_H

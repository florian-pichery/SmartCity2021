#ifndef CAPP_H
#define CAPP_H

#include <QObject>
#include <QThread>

#include "cgestionmaquette.h"
#include "czdc.h"
#include "cconfig.h"

class CApp : public QObject
{
    Q_OBJECT
public:
    explicit CApp(QObject *parent = nullptr);
    ~CApp();

//Pour l'IHM

//Adressage
    void setAddrEclair(uint8_t addrEclair);
    void setAddrInter(uint8_t addrInter);
    void setAddrPark(uint8_t addrPark);
//Fin Adressage

//Eclairage
    void setConsigne(uint8_t consigne);
    void setCellule(bool cellule);
    void setPresence(bool presence);
//Fin Eclairage

//Intersection
     void setInterOrdre(uint8_t interOrdre);
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

signals:
    void sig_go();
    /*POUR L'IHM*/
    //Éclairage
    void sig_msg_consigne(QString msg_consigne);
    void sig_msg_cellule(QString msg_cellule);
    void sig_msg_presence(QString msg_presence);
    //Fin éclairage
    //Intersection
    void sig_msg_interOrdre1(QString msg_interOrdre1);
    void sig_msg_interOrdre2(QString msg_interOrdre2);
    void sig_msg_mode(QString msg_mode);
    //Fin intersection
    //Parking
    void sig_msg_parkOrdreI(QString msg_parkOrdreI);
    void sig_msg_parkOrdreO(QString msg_parkOrdreO);
    void sig_msg_RFIDI(QString msg_RFIDI);
    void sig_msg_RFIDO(QString msg_RFIDO);
    //Fin parking
};

#endif // CAPP_H

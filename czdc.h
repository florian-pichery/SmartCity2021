#ifndef CZDC_H
#define CZDC_H

#include <QObject>
#include <QSharedMemory>
#include <QDebug>
#include <QByteArray>
#include "cconfig.h"

#define KEY "SmartCity 2021"

//masque pour les états barrières

#define BED 1
#define BEM 2
#define BSD 4
#define BSM 8
#define QQE 16
#define QQS 32
#define BAE 64
#define BAS 128


//Structuration des données

typedef enum couleurs {
  ETEINT,
  ROUGE,
  VERT,
  BLEU
} T_COULEURS;

typedef struct parking{
    uint8_t addr;// Adresse i²c de l'esclave
    char affLigneSup[17];
    char affLigneInf[17];
    T_COULEURS couleurs;// Couleurs de l'écran
    uint8_t parkOrdre;// 7:ACK_BSD 6:ACK_BSM 5:ACK_BED 4:ACK_BEM
                      // 3:OrdreBSD 2:OrdreBSM 1:OrdreBED 0:OrdreBEM
    uint8_t etats;// Barrière montée, descendue, en cours
    uint8_t cptPlaces;
    uint8_t rfidE[5];// RFID des clients
    uint8_t rfidS[5];// RFID des clients
} T_PARKING;

typedef struct eclairage{
    uint8_t addr;// Adresse i²c de l'esclave
    uint8_t consigne; // 0% / 50% / 100%
    bool presence;// Soit présent soit absent
    bool cellule;// Soit jour soit nuit
} T_ECLAIRAGE;

typedef struct intersection{
    uint8_t addr;// Adresse i²c de l'esclave
    uint8_t boutonPieton; // 8 appels piétons // 4 par trame de voie (bit 7,6,5 en lecture)
    uint8_t mode;// normal, orange clignotant, manuel (bit 2, LSB écriture)
    uint8_t interOrdre;// MSB à 0 : voie 1 / MSB à 1 : voie 2 (0 : éteint / 1 : Vert / 2 : Orange / 3 : Rouge [bit 7,6,5 en écriture])
} T_INTERSECTION;

typedef struct zdc {
    T_PARKING parking;
    T_ECLAIRAGE *eclairage;//possibilité d'ajouter un grand nombre de cartes d'éclairage
    T_INTERSECTION intersection;
} T_ZDC;

// ZDC : Zone De Données Communes

class CZdc : public QSharedMemory
{
        Q_OBJECT
public:
    CZdc();
    ~CZdc();

    void setAddrPark(uint8_t addrP);
    void setAddrInter(uint8_t addrI);
    void setAddrEclair(uint8_t addrE);

//Barrières
    void setBarriersState(bool state, int msk);
    void setBarriersOrder(uint8_t parkOrder);
    void setRfidE(QByteArray rfid);
    void setRfidS(QByteArray rfid);
    void setLigneSup(QByteArray liSup);
    void setLigneInf(QByteArray liInf);
    void setCpt(uint8_t places);
    void setCptPlus(uint8_t places);
    void setCptMoins(uint8_t places);
    QByteArray getRfidE(QByteArray rfid);
    QByteArray getRfidS(QByteArray rfid);
signals:
    void sig_OrderBarrier(uint8_t parkOrder);
    void sig_ligneSup(QByteArray liSup);
    void sig_ligneInf(QByteArray liInf);
    void sig_Cpt(uint8_t places);
    void sig_setRFIDe(QByteArray rfid);
    void sig_setRFIDs(QByteArray rfid);
//Fin barrières
//Eclairage
    void setConsigne(uint8_t consigne);
    bool getPresence(bool presence);
    bool getCellule(bool cellule);
signals:
    void sig_Consigne(uint8_t consigne);
//Fin éclairage
//Intersection
    uint8_t getBoutonPieton(uint8_t boutonPieton);
    void setMode(uint8_t mode);
    void setOrdres(uint8_t interOrdre);
signals:
    void sig_Mode(uint8_t mode);
    void sig_OrderInter(uint8_t order);
//Fin intersection
private:
    CConfig config;
    T_ZDC *_adrZdc;
    void clear();

public slots:
    //void on_sigErreur(QString mess);

signals:
    //void sig_erreur(QString mess);
};

#endif // CZDC_H

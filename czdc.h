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
  VERT
} T_COULEURS;

typedef struct parking{
    int addr;// Adresse i²c de l'esclave
    char affLigneSup[17];
    char affLigneInf[17];
    uint8_t cptPlaces;
    T_COULEURS couleurs;// Couleurs de l'écran
    uint8_t parkOrdre;// 7:ACK_BSD 6:ACK_BSM 5:ACK_BED 4:ACK_BEM
                      // 3:OrdreBSD 2:OrdreBSM 1:OrdreBED 0:OrdreBEM
    uint8_t etats;// 7:ButtUrgExit 6:ButtUrgEntr 5:PersEntr 4: PersExit
                  // 3:BSM 2:BSD 1:BEM 0: BED
    uint8_t rfidE[5];// RFID des clients
    uint8_t rfidS[5];// RFID des clients
} T_PARKING;

typedef struct eclairage{
    int addr;// Adresse i²c de l'esclave
    uint8_t nbEclair;
    uint8_t consigne; // 0% / 50% / 100%
    bool presence;// Soit présent soit absent
    bool cellule;// Soit jour soit nuit
    uint8_t lampFonct; //Indique lampadaires fonctionnels
} T_ECLAIRAGE;

typedef struct intersection{
    int addr;// Adresse i²c de l'esclave
    uint8_t mode;// normal, orange clignotant, manuel (bit 2, LSB écriture)
    //Voie 1
    uint8_t boutonPieton1; // 8 appels piétons // 4 par trame de voie (bit 7,6,5 en lecture)
    uint8_t interOrdre1;// MSB à 0 : voie 1 / MSB à 1 : voie 2 (0 : éteint / 1 : Vert / 2 : Orange / 3 : Rouge [bit 7,6,5 en écriture])
    //Voie 2
    uint8_t boutonPieton2; // 8 appels piétons // 4 par trame de voie (bit 7,6,5 en lecture)
    uint8_t interOrdre2;// MSB à 0 : voie 1 / MSB à 1 : voie 2 (0 : éteint / 1 : Vert / 2 : Orange / 3 : Rouge [bit 7,6,5 en écriture])
} T_INTERSECTION;

typedef struct zdc {
    T_PARKING parking;
    T_INTERSECTION intersection;
    T_ECLAIRAGE *eclairage;//possibilité d'ajouter un grand nombre de cartes d'éclairage
} T_ZDC;

// ZDC : Zone De Données Communes

class CZdc : public QSharedMemory
{
        Q_OBJECT
public:
    CZdc();
    ~CZdc();

    void setNbEclairage(uint8_t nb);
    uint8_t getNbEclairage();

    void setAddrPark(int addrP);
    void setAddrInter(int addrI);
    void setAddrEclair(int addrE);
    int getAddrPark();
    int getAddrInter();
    int getAddrEclair();

//Barrières
    void setBarriersState(uint8_t parkState);
    void setBarriersOrder(uint8_t parkOrder);
    uint8_t getBarriersOrder();
    void setRfidE(QByteArray rfid);
    void setRfidS(QByteArray rfid);
    void setLigneSup(QByteArray liSup);
    void setLigneInf(QByteArray liInf);
    void setCpt(uint8_t places);
    uint8_t getCpt();
    void setCptPlus();
    void setCptMoins();
    QByteArray getRfidE();
    QByteArray getRfidS();
signals:
    void sig_OrderBarrier(uint8_t parkOrder);
    void sig_ligneSup(QByteArray liSup);
    void sig_ligneInf(QByteArray liInf);
    void sig_Cpt(uint8_t places);
    void sig_setRFIDe(QByteArray rfid);
    void sig_setRFIDs(QByteArray rfid);
//Fin barrières
//Eclairage
public:
    void setConsigneEclair(uint8_t consigne);//0 = 0% // 1 = 50% // 2 = 100%
    uint8_t getConsigneEclair();
    void setLampFonct(uint8_t nb);
    uint8_t getLampFonct();
    void setPresence(bool presence);
    bool getPresence();
    void setCellule(bool cellule);
    bool getCellule();
signals:
    void sig_ConsigneEclair(uint8_t consigne);
    void sig_LampFonct(uint8_t nb);
//Fin éclairage
//Intersection
public:
    void setModeVoies(uint8_t mode);
    uint8_t getModeVoies();
    void setBoutonPietonVoie1(uint8_t bp);
    uint8_t getBoutonPietonVoie1();
    void setOrdresFeu1(uint8_t interOrdre);
    uint8_t getOrdresFeu1();

    void setBoutonPietonVoie2(uint8_t bp);
    uint8_t getBoutonPietonVoie2();
    void setOrdresFeu2(uint8_t interOrdre);
    uint8_t getOrdresFeu2();
signals:
    void sig_ModeVoies(uint8_t mode);
    void sig_OrderInter1(uint8_t order);
    void sig_OrderInter2(uint8_t order);
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

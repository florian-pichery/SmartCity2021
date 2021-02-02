#ifndef CZDC_H
#define CZDC_H

#include <QObject>
#include <QSharedMemory>
#include <QDebug>
#include <QByteArray>
#include "cconfig.cpp"

#define KEY "SmartCity 2021"

//masque pour les états barrières

#define BEM 1
#define BED 2
#define BSM 4
#define BSD 8
#define BAE 16
#define BAS 32


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
    uint8_t rfid[5];// RFID des clients
} T_PARKING;

typedef struct eclairage{
    uint8_t addr;// Adresse i²c de l'esclave
    uint8_t consigne; // 0% / 50% / 100%
    bool presence;// Soit présent soit absent
    bool cellule;// Soit jour soit nuit
} T_ECLAIRAGE;

typedef struct intersection{
    uint8_t addr;// Adresse i²c de l'esclave
    uint8_t boutonPieton; // 8 appels piétons
    uint8_t mode;// normal, orange clignotant, manuel
    uint8_t interOrdre;// bit 01 : voie 1 / bit 23 : voie 2 (00 : éteint / 01 : Vert / 02 : Orange / 03 : Rouge)
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

//Barrières
    void setBarriersState(bool state, int msk);
    void setBarriersOrder(uint8_t parkOrder);
    void setCpt(uint8_t places);
    void setRfid(uint8_t rfid[5]);
    void getRfid(uint8_t rfid[5]);
signals:
    void sig_OrderBarrier(uint8_t parkOrder);
    void sig_RFID(uint8_t rfid[5]);
//Fin barrières
//Eclairage
    void setConsigne(uint8_t consigne);
    void getPresence(bool presence);
    void getCellule(bool cellule);
signals:
    void sig_Consigne(uint8_t consigne);
//Fin éclairage
//Intersection
    void getBoutonPieton(uint8_t boutonPieton);
    void setMode(uint8_t mode);
    void setOrdres(uint8_t interOrdre);
signals:
    void sig_Mode(uint8_t mode);
    void sig_OrderInter(uint8_t order);
//Fin intersection
private:
    T_ZDC *_adrZdc;
    void clear();

public slots:
    void on_sigErreur();
    void on_newData();

signals:
    void sig_erreur(QString mess);
    void sig_update();
};

#endif // CZDC_H

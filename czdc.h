#ifndef CZDC_H
#define CZDC_H

#include <QObject>
#include <QSharedMemory>
#include <QDebug>
#include <QByteArray>
#include "cconfig.cpp"

#define KEY "SmartCity 2021"

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
    uint8_t addr[Addr_park];//adresse I2C
    char affLigneSup[17];
    char affLigneInf[17];
    T_COULEURS couleurs;// Couleurs de l'écran
    uint8_t ordres;// Monter, descendre
    uint8_t etats;// Barrière montée, descendue, en cours
    uint8_t cptPlaces;
    uint8_t rfid[5];// RFID des clients
} T_PARKING;

typedef struct eclairage{
    uint8_t addr;
    uint8_t consigne; // 0% / 50% / 100%
    bool presence;// Soit présent soit absent
    bool cellule;// Soit jour soit nuit
} T_ECLAIRAGE;

typedef struct intersection{
    uint8_t addr;
    uint8_t boutonPieton; // 8 appels piétons
    uint8_t mode;// normal, orange clignotant, manuel
    uint8_t ordres;// bit 01 : voie 1 / bit 23 : voie 2 (00 : éteint / 01 : Vert / 02 : Orange / 03 : Rouge)
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
    void setEtatBarriers(bool state, int msk);
    void setOrdreBarriers(QString ordre);
    void setCpt(uint8_t places);
    void setRfid(uint8_t rfid[5]);
    void getRfid(uint8_t rfid[5]);

//Eclairage
    void setConsigne();
    void getPresence();
    void getCellule();

//Intersection
    void getBoutonPieton();
    void setMode();
    void setOrdres();

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

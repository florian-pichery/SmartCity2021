#ifndef CZDC_H
#define CZDC_H

#include <QObject>
#include <QSharedMemory>
#include <QDebug>
#include <QByteArray>

#include "cconfig.h"

#define KEY "SmartCity 2021"

//Structuration des données

/* Raccourcis des états barrières
  BSM = Barrière de Sortie Montée
  BSD = Barrière de Sortie Descente ou Descendue (état)
  BEM = Barrière de l'Entrée Montée
  BED = Barrière de l'Entrée Descente ou Descendue (état)
*/

typedef struct parking{
    int addr;// Adresse i²c de l'esclave
    char affLigneSup[16]; //Ligne supérieure de l'écran
    char affLigneInf[16]; //Ligne inférieure de l'écran
    uint8_t cptPlaces; //Compteur de places (interne à la maquette)
    uint8_t parkOrdre;// Bit 7:ACK_ORDRE / Bits 6,5,4:INUTILISES
                      // Bit 3:OrdreBSD / Bit 2:OrdreBSM / Bit 1:OrdreBED / Bit 0:OrdreBEM
    uint8_t etats;// Bit 7:ButtUrgExit / Bit 6:ButtUrgEntr / Bit 5:PersEntr / Bit 4: PersExit
                  // Bit 3:BSM / Bit 2:BSD / Bit 1:BEM / Bit 0:BED
    uint8_t rfidE[5];// RFID du client entrant
    uint8_t rfidS[5];// RFID du client sortant
} T_PARKING;

typedef struct eclairage{
    int addr;// Adresse i²c de l'esclave
    uint8_t nbEclair; //Nombre de cartes d'éclairages
    uint8_t consigne; // Sur les deux bits de poids faible : 00 = 0% / 01 = 50% / 10 = 100% //
    bool presence; // Soit présent (0) soit absent (1) pour détecter les pb de capteurs
    bool cellule; // Soit jour (0) soit nuit (1) pour détecter les pb de capteurs
    uint8_t lampFonct; //Indique lampadaires fonctionnels (De 0 à 6)
} T_ECLAIRAGE;

typedef struct intersection{
    int addr;// Adresse i²c de l'esclave
    uint8_t etatMode;// Sur les deux bits de poids faible (1 et 0) : 00 = Orange clignotant / 01 = Automatique / 10 = Manuel //
    uint8_t ordreMode;// Sur les deux bits de poids faible (1 et 0) : 00 = Orange clignotant / 01 = Automatique / 10 = Manuel //
    //Voie 1
    uint8_t boutonPieton1; /* 4 par trame de voie (bit 6,5,4 en lecture)
                           =>100 : 4 boutons appuyés / 011 : 3 boutons appuyés
                             010 : 2 boutons appuyés / 001 : 1 bouton appuyé / 000 : Rien */
    uint8_t interEtat1; // 00 : éteint / 01 : Vert / 10 : Orange / 11 : Rouge [bits 3 et 2 en lecture])
    uint8_t interOrdre1; //(00 : éteint / 01 : Vert / 10 : Orange / 11 : Rouge [bits 3 et 2 en écriture])
    //Voie 2
    uint8_t boutonPieton2; /* 4 par trame de voie (bit 6,5,4 en lecture)
                           =>100 : 4 boutons appuyés / 011 : 3 boutons appuyés
                             010 : 2 boutons appuyés / 001 : 1 bouton appuyé / 000 : Rien */
    uint8_t interEtat2;// 00 : éteint / 01 : Vert / 10 : Orange / 11 : Rouge [bits 3 et 2 en lecture])
    uint8_t interOrdre2;//(00 : éteint / 01 : Vert / 10 : Orange / 11 : Rouge [bits 5 et 4 en écriture])
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

    void init();
//INIT I²C
    void setNbEclairage(uint8_t nb);
    uint8_t getNbEclairage();

    void setAddrPark(int addrP);
    void setAddrInter(int addrI);
    void setAddrEclair(int addrE);
    int getAddrPark();
    int getAddrInter();
    int getAddrEclair(); 
//Fin INIT I²C

//Parking
    void setEtatsBarrieres(uint8_t parkState);/* Définition de l'état des barrières par CParking:
                                             Bit 7:ButtUrgExit / Bit 6:ButtUrgEntr / Bit 5:PersEntr / Bit 4: PersExit
                                             Bit 3:BSM / Bit 2:BSD / Bit 1:BEM / Bit 0:BED */
    uint8_t getEtatsBarrieres();
    void setOrdreBarrieres(uint8_t parkOrder);/* Définition de l'ordre : Quand un ordre des clients est reçu le bit ACK_ORDRE passe à 1
                                                -> 128 + Bit 3:BSM / Bit 2:BSD / Bit 1:BEM / Bit 0:BED */
    uint8_t getOrdreBarrieres();
    void setRfidE(QByteArray rfid);// Ecriture du RFID entrant par CParking
    void setRfidS(QByteArray rfid);// Ecriture du RFID sortant par CParking
    QByteArray getRfidE();
    QByteArray getRfidS();
    void setLigneSup(QString &liSup);// Ecriture de la première ligne de l'écran par CParking
    void setLigneInf(QString &liInf);// Ecriture de la deuxième ligne de l'écran par CParking
    QString getLigneSup();
    QString getLigneInf();
    void setCpt(uint8_t places);// Définition du nombre de places dispo par CParking (Par défaut 8)
    uint8_t getCpt();
    void setCptPlus();// Incrémentation du nombre de places par CParking
    void setCptMoins();// Décrémentation du nombre de places par CParking
//Fin parking
//Eclairage
public:
    void setConsigneEclair(uint8_t noCarte, uint8_t consigne);/* Définition de la consigne de l'éclairage sur les deux bits de poids faible : 00 = 0% / 01 = 50% / 10 = 100%
                                                              en mentionnant le numéro de la carte dans la mémoire de la ZDC : première carte = 0 // deuxième carte = 1...*/
    uint8_t getConsigneEclair(uint8_t noCarte);
    void setLampFonct(uint8_t noCarte, uint8_t nb);// Définition du nombre d'éclairages sur une carte d'éclairage par CEclairage
    uint8_t getLampFonct(uint8_t noCarte);
    void setPresence(uint8_t noCarte, bool presence);// Présence définie par CEclairage
    bool getPresence(uint8_t noCarte);
    void setCellule(uint8_t noCarte, bool cellule);// Jour/Nuit défini par CEclairage
    bool getCellule(uint8_t noCarte);
//Fin éclairage
//Intersection
public:
    void setEtatMode(uint8_t mode);/* Définition du mode de l'éclairage
        sur les deux bits de poids faible (1 et 0) : 00 = Orange clignotant / 01 = Automatique / 10 = Manuel*/
    uint8_t getEtatMode(); //LECTURE
    void setModeVoies(uint8_t mode);// ECRITURE //Définition du mode de l'éclairage sur les deux bits de poids faible (1 et 0) : 00 = Orange clignotant / 01 = Automatique / 10 = Manuel*/
    uint8_t getModeVoies();
    /* Voie 1 */
    void setBoutonPietonVoie1(uint8_t bp);// Définition du nombre de boutons piétons cliqués défini par CIntersection
    uint8_t getBoutonPietonVoie1();
    void setOrdresFeu1(uint8_t interOrdre);// Définition de l'ordre (en mode manuel) (00 : éteint / 01 : Vert / 10 : Orange / 11 : Rouge)
    uint8_t getOrdresFeu1();
    void setInterEtat1(uint8_t etat);
    uint8_t getInterEtat1();
    /* Voie 2 */
    void setBoutonPietonVoie2(uint8_t bp);// Définition du nombre de boutons piétons cliqués défini par CIntersection
    uint8_t getBoutonPietonVoie2();
    void setOrdresFeu2(uint8_t interOrdre);// Définition de l'ordre (en mode manuel) (00 : éteint / 01 : Vert / 10 : Orange / 11 : Rouge)
    uint8_t getOrdresFeu2();
    void setInterEtat2(uint8_t etat);
    uint8_t getInterEtat2();

//Fin intersection
private:
    CConfig config;
    T_ZDC *_adrZdc;
    void clear();
};

#endif // CZDC_H

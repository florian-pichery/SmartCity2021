#ifndef CZDC_H
#define CZDC_H

#include <QObject>
#include <QSharedMemory>
#include <QDebug>

#define KEY "SmartCity 2021"

#define BEM 1
#define BED 2
#define BSM 4
#define BSD 8
#define BAE 16
#define BAS 32

//Structuration des données

typedef struct parking{
    char affLigneSup[17];
    char affLigneInf[17];
    uint8_t ordres;
    uint8_t etats;

    uint8_t cptPlaces;
} T_PARKING;

typedef struct eclairage{
    bool secteur;
    bool pourcentage;
    bool eco;
    bool pieton;
    bool detect;
    bool defect;
} T_ECLAIRAGE;

typedef struct intersection{

} T_INTERSECTION;

typedef enum couleurs {
    ETEINT,
    ROUGE,
    VERT,
    BLEU
} T_COULEURS;


typedef struct zdc {
    T_PARKING parking;
    T_ECLAIRAGE eclairage;
    T_INTERSECTION intersection;
    T_COULEURS couleurs;
} T_ZDC;

// ZDC : Zone De Données Communes

class CZdc : public QSharedMemory
{
        Q_OBJECT
public:
    CZdc();
    ~CZdc();
    void setEtatBarriers(bool state, int msk);
    void getDetect();

private:
    T_ZDC *_adrZdc;
    void clear();

public slots:
    void on_sigErreur();
    void on_sigUpdate();

signals:
    //void sig_erreur(QString mess);
    void sig_update();
};

#endif // CZDC_H


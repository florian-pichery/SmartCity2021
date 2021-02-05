#ifndef CGESTIONMAQUETTE_H
#define CGESTIONMAQUETTE_H

#include <QObject>
#include <QThread>
#include <QString>

#include "cparking.h"
#include "cintersection.h"
#include "ceclairage.h"
#include "cecran.h"
#include "czdc.h"

class CGestionMaquette : public QObject
{
    Q_OBJECT

public:
    CGestionMaquette(CZdc *zdc);
    ~CGestionMaquette();

    void cyclePark();
    void cycleEcran();
    void cycleEclair();
    void cycleInter();

private:

    CZdc *_zdc;
    CParking *_park;
    CIntersection *_inter;
    CEclairage *_eclair;
    CEcran *_ecran;
    QThread *_gthm;

public slots:
//Parking
    void onOrderBarriers();
    void onRFIDe();
    void onRFIDs();
//Fin parking
//Eclairage
    void onConsigne();
//Fin eclairage
//Intersection
    void onMode();
    void onOrderInter();
//Fin inter

signals:

};

#endif // CGESTIONMAQUETTE_H

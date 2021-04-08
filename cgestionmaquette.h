#ifndef CGESTIONMAQUETTE_H
#define CGESTIONMAQUETTE_H

#include <QObject>
#include <QThread>
#include <QString>

#include "czdc.h"
#include "ceclairage.h"
#include "cintersection.h"
#include "cecran.h"
#include "cparking.h"
#include "cconfig.h"

class CGestionMaquette : public QObject
{
    Q_OBJECT

public:
    CGestionMaquette(CZdc *zdc);
    ~CGestionMaquette();

    void on_go();

private:
    CEclairage *_eclair;
    CIntersection *_inter;
    CEcran *_ecran;
    CParking *_park;
    CZdc *_zdc;

signals:
    void sigEclair(int addr, int nb, int addr_base);
    void sigInter();
    void sigPark();
    void sigEcran(QString nb);
};

#endif // CGESTIONMAQUETTE_H

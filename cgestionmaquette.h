#ifndef CGESTIONMAQUETTE_H
#define CGESTIONMAQUETTE_H

#include <QObject>
#include <QThread>
#include <QString>


#include "cecran.h"
#include "czdc.h"
#include "capp.h"

class CGestionMaquette : public QObject
{
    Q_OBJECT

public:
    CGestionMaquette();
    ~CGestionMaquette();

private:
    CZdc *_zdc;
//    CParking *_park;
//    CIntersection *_inter;
//    CEclairage *_eclair;
//    CEcran *_ecran;
    QThread *_gthm;

public slots:



signals:


};

#endif // CGESTIONMAQUETTE_H

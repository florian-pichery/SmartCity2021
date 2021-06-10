#include "cparking.h"

CParking::CParking(CZdc *zdc, QObject *parent) : QObject(parent)
{
    _zdc = zdc;
    _i2c = CI2c::getInstance(nullptr, 1);
    //Init
    _zdc->setOrdreBarrieres(0);//0 = BED et BSD / 1 = BEM et BSD
    //2 = BED ET BSM / 3 = BEM ET BSM
    _zdc->setEtatsBarrieres(0);
    _zdc->setCpt(8);//8 places par défaut
    //Fin Init
}

CParking::~CParking()
{
    CI2c::freeInstance();
}

void CParking::onPark()
{
    unsigned char addr = static_cast<unsigned char>(_zdc->getAddrPark());

    unsigned char parking[11];
    QByteArray RFIDe;
    QByteArray RFIDs;

    _i2c->lire(addr, parking, 11); // Lecture
    T_PARK_STATE *parkState;
    parkState = reinterpret_cast<T_PARK_STATE *>(&parking[0]);
    _zdc->setEtatsBarrieres(parkState->bitsStates);

    RFIDe = QByteArray(reinterpret_cast<char *>(parking+1), 5); //Conversion de unsigned char * vers QByteArray
    //qDebug() << RFIDe;
    _zdc->setRfidE(RFIDe);

    RFIDs = QByteArray(reinterpret_cast<char *>(parking+6), 5);
    //qDebug() << RFIDs;
    _zdc->setRfidS(RFIDs);

    usleep(200);
    emit sigRestart();

    //emit sigEcran(static_cast<QString>(_zdc->getCpt()));

    uint8_t order = _zdc->getOrdreBarrieres();
    if(order >= 128){
        order = order - 128;
        switch(order){
        case BAR_SORTIE_DES:
            _i2c->ecrire(static_cast<unsigned char>(_zdc->getAddrPark()), &order, 1);
            _zdc->setOrdreBarrieres(order);
            _zdc->setCptPlus();
            usleep(100);
            emit sigEcran(static_cast<QString>(_zdc->getCpt()));
            break;

        case BAR_ENTREE_DES:
            _i2c->ecrire(static_cast<unsigned char>(_zdc->getAddrPark()), &order, 1);
            _zdc->setOrdreBarrieres(order);
            _zdc->setCptMoins();
            usleep(100);
            emit sigEcran(static_cast<QString>(_zdc->getCpt()));
            break;

        default:
            order = _zdc->getOrdreBarrieres();
            _i2c->ecrire(static_cast<unsigned char>(_zdc->getAddrPark()), &order, 1);
            _zdc->setOrdreBarrieres(order);
            usleep(100);
            emit sigEcran(static_cast<QString>(_zdc->getCpt()));
            break;
        }//SW
    }//IF
}//onPark()

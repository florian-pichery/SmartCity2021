#include "cintersection.h"

CIntersection::CIntersection(CZdc *zdc, QObject *parent) :  QObject(parent)
{
    _zdc = zdc;
    _i2c = CI2c::getInstance(this, '1');
    //Init
    _zdc->setModeVoies(1);//Mode auto par défaut
    _zdc->setOrdresFeu1(0);//Éteint
    _zdc->setOrdresFeu2(0);//Éteint
    //Fin Init
}

CIntersection::~CIntersection()
{
    CI2c::freeInstance();
}

void CIntersection::onInter()
{
    unsigned char addr = static_cast<unsigned char>(_zdc->getAddrInter());

    //READ
    unsigned char inter[2];
    _i2c->lire(addr, inter, 2); // Lecture

    U_READ octet;
    octet.octet = inter[0];//1er octet
    _zdc->setModeVoies(octet.partie.bitMode);
    _zdc->setOrdresFeu1(octet.partie.bitCouleurs);
    _zdc->setBoutonPietonVoie1(octet.partie.bitBoutons);

    octet.octet = inter[1];//2ème octet
    _zdc->setOrdresFeu2(octet.partie.bitCouleurs);
    _zdc->setBoutonPietonVoie2(octet.partie.bitBoutons);

    //WRITE

    uint8_t mode = _zdc->getModeVoies();

    if(mode > ORDRE_RECU){
        unsigned char ordre;
        T_WRITE *write;
        write = reinterpret_cast<T_WRITE *>(&ordre);
        write->bitMode = _zdc->getModeVoies() - 128;

        _i2c->ecrire(addr, &ordre, 1);
        _zdc->setModeVoies(ordre);
    }//IF ordre reçu

    if(mode == MODE_MANUEL){
        if(_zdc->getOrdresFeu1() > 128){ //getOrdres1 : pour différencier les ordres pouvant être envoyés
        unsigned char ordre;
            T_WRITE *write;
            write = reinterpret_cast<T_WRITE *>(&ordre);
            write->bitMode = _zdc->getModeVoies() - 128;
            write->bitCouleurs1 = _zdc->getOrdresFeu1() - 128;

            _i2c->ecrire(addr, &ordre, 1);

            _zdc->setModeVoies(ordre - write->bitCouleurs1);
            _zdc->setOrdresFeu1(ordre - write->bitMode);
        }//IF manu voie1
        if(_zdc->getOrdresFeu2() > 128){
            unsigned char ordre;
            T_WRITE *write;
            write = reinterpret_cast<T_WRITE *>(&ordre);
            write->bitMode = _zdc->getModeVoies() - 128;
            write->bitCouleurs2 = _zdc->getOrdresFeu2() - 128;

            _i2c->ecrire(addr, &ordre, 1);

            _zdc->setModeVoies(ordre - write->bitCouleurs2);
            _zdc->setOrdresFeu2(ordre - write->bitMode);
        }//IF  manu voie2

    }//IF Mode manuel

    emit sigRestart();
}//OnInter

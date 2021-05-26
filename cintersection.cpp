#include "cintersection.h"

CIntersection::CIntersection(CZdc *zdc, QObject *parent) :  QObject(parent)
{
    _zdc = zdc;
    _i2c = CI2c::getInstance(this, '1');
    //Init
    _zdc->setModeVoies(1);//Mode auto par défaut
    _zdc->setOrdresFeu1(3);//Rouge
    _zdc->setOrdresFeu2(1);//Vert
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
    _zdc->setInterEtat1(octet.partie.bitCouleurs);
    _zdc->setBoutonPietonVoie1(octet.partie.bitBoutons);

    octet.octet = inter[1];//2ème octet
    _zdc->setInterEtat2(octet.partie.bitCouleurs);
    _zdc->setBoutonPietonVoie2(octet.partie.bitBoutons);

    //WRITE
    uint8_t mode = _zdc->getModeVoies();
    uint8_t couleur1 = _zdc->getOrdresFeu1();
    uint8_t couleur2 = _zdc->getOrdresFeu2();
    mode |= ACK; // Version de test
    couleur1 |= ACK; //Test
    couleur2 |= ACK; //Test
    U_WRITE uw;
    uw.octet = 0;

    if(mode != MODE_MANUEL){
        uw.partie.bitMode = mode - ACK;
        _i2c->ecrire(addr, &uw.octet, 1);
        _zdc->setModeVoies(uw.octet);
    }//IF mode =/ manuel

    else{
        uw.partie.bitMode = mode - ACK;
        if(couleur1 >= ACK){
            uw.partie.bitCouleurs1 = couleur1 - ACK;
        }//IF changement couleur voie1
        if(couleur2 > ACK){
            uw.partie.bitCouleurs2 = couleur2 - ACK;
        }//IF changement couleur voie2
        _i2c->ecrire(addr, &uw.octet, 1);
        _zdc->setModeVoies(uw.octet - uw.partie.bitCouleurs2);
        _zdc->setOrdresFeu1(uw.octet - (uw.partie.bitMode + uw.partie.bitCouleurs2));
        _zdc->setOrdresFeu2(uw.octet - (uw.partie.bitMode + uw.partie.bitCouleurs1));
    }//IF Mode manuel

    emit sigRestart();
}//OnInter

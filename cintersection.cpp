#include "cintersection.h"

CIntersection::CIntersection(CZdc *zdc, QObject *parent) :  QObject(parent)
{
    _zdc = zdc;
    _i2c = CI2c::getInstance(this, '1');
    _zdc->setModeVoies(1);
    uw.octet = 1;
    ancienOctet = 128;
    //    //TEST
    //    _zdc->setOrdresFeu1(3);
    //    _zdc->setOrdresFeu2(3);
    //    //Fin TEST
}

CIntersection::~CIntersection()
{
    CI2c::freeInstance();
    _bdd->del_i2cIntersection(QString(_zdc->getAddrInter()));
}

void CIntersection::onInter()
{
    unsigned char addr = static_cast<unsigned char>(_zdc->getAddrInter());

    //READ
    unsigned char inter[2];
    _i2c->lire(addr, inter, 2); // Lecture

    _bdd->set_i2cIntersection(QString(addr), "0", "0", "0", "0", "0");

    U_READ octet;
    octet.octet = inter[0];//1er octet
    _zdc->setEtatMode(octet.partie.bitMode);
    _zdc->setInterEtat1(octet.partie.bitCouleurs);
    _zdc->setBoutonPietonVoie1(octet.partie.bitBoutons);

    octet.octet = inter[1];//2ème octet
    _zdc->setInterEtat2(octet.partie.bitCouleurs);
    _zdc->setBoutonPietonVoie2(octet.partie.bitBoutons);

    _bdd->mod_i2cIntersection(QString(addr), QString(_zdc->getEtatMode()), QString(_zdc->getBoutonPietonVoie1()), QString(_zdc->getBoutonPietonVoie2()), QString(_zdc->getInterEtat1()), QString(_zdc->getInterEtat2()));

    //WRITE
    uint8_t mode = _zdc->getModeVoies();
    //    mode += ACK; // Version de test
    //    couleur1 += ACK; //Test
    //    couleur2 += ACK; //Test

    if(mode >= ACK){
        mode &= ~ACK;
        uint8_t couleur1 = _zdc->getOrdresFeu1();
        uint8_t couleur2 = _zdc->getOrdresFeu2();
        uw.partie.bitMode = mode;
        if(couleur1 >= ACK){
            uw.partie.bitCouleurs1 = couleur1 - ACK;
        }//IF changement couleur voie1
        if(couleur2 >= ACK){
            uw.partie.bitCouleurs2 = couleur2 - ACK;
        }//IF changement couleur voie2

        if(mode != MODE_MANUEL){
            uw.partie.bitMode = mode;

            if(uw.octet != ancienOctet){
                _i2c->ecrire(addr, &uw.octet, 1);
                qDebug() << uw.octet;
                _zdc->setModeVoies(uw.octet);
                ancienOctet = uw.octet;
            }//IF nouvel octet != ancien octet
        }//IF mode != manuel
        else{
            if(uw.octet != ancienOctet){
                _i2c->ecrire(addr, &uw.octet, 1);
                _zdc->setModeVoies(uw.partie.bitMode);
                _zdc->setOrdresFeu1(uw.partie.bitCouleurs1);
                _zdc->setOrdresFeu2(uw.partie.bitCouleurs2);
                ancienOctet = uw.octet;
            }//IF nouvel octet != ancien octet
        }//IF Mode manuel
    }//IF Ordre
    sleep(1);
    emit sigRestart();
}//OnInter

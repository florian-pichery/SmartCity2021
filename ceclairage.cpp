#include "ceclairage.h"

CEclairage::CEclairage(CZdc *zdc, QObject *parent) : QObject(parent)
{
    _zdc = zdc;
    _i2c = CI2c::getInstance(this, '1');
    connect(this, &CEclairage::sigEclair, this, &CEclairage::on_sigEclair);
    //Initialisation des variables concernant l'éclairage
    int nb = _zdc->getNbEclairage();
    for(uint8_t i = 0; i < nb; i++){
        _zdc->setPresence(i,false);
        _zdc->setCellule(i,false);
        _zdc->setLampFonct(i,63);//6 lampadaires fonctionnants
        _zdc->setConsigneEclair(i,0); //Éteint par défaut
    }//for
   //Fin Init
}

CEclairage::~CEclairage()
{
    CI2c::freeInstance();
}

void CEclairage::on_sigEclair(int addr, int nb, int addr_base)
{
    if (addr > (addr_base+nb-1)){
        sleep(3);
        emit sigRestart();
        //emit sigParking();
        return;
    }//if addr

    U_LAMP octet;
    _i2c->lire(static_cast<unsigned char>(addr), &octet.octet, 1); // Lecture

    uint8_t indice = static_cast<uint8_t>(addr-addr_base);

    _zdc->setCellule(indice, octet.partie.bitJN);
    _zdc->setPresence(indice, octet.partie.bitPresence);
    _zdc->setLampFonct(indice, octet.partie.bitLamps);

//    //Test I2C avec Ninnin à voir

//    unsigned char ordre;
//    ordre = 1;

//    _i2c->ecrire(addr, &ordre, 1);
//    _zdc->setConsigne(ordre);

    unsigned char ordre = _zdc->getConsigneEclair(indice);

    if(ordre >= ORDRE_RECU){
        ordre -= ORDRE_RECU;
        _i2c->ecrire(static_cast<unsigned char>(addr), &ordre, 1);
        _zdc->setConsigneEclair(indice, ordre);
    }

    addr = addr + 1;

    emit sigEclair(addr, nb, addr_base);
}

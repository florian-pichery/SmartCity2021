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
        _zdc->setConsigneEclair(i,4); //Éteint par défaut
    }//for
    //Fin Init
}

CEclairage::~CEclairage()
{
    CI2c::freeInstance();
    for(int i = 0; i < _zdc->getNbEclairage(); i++){
        _bdd->del_i2cEclairage(QString(_zdc->getAddrEclair()+i));
    }
}

void CEclairage::on_sigEclair(int addr, int nb, int addr_base)
{
    if (addr > (addr_base+nb-1)){
        sleep(2);
        emit sigRestart();
        //emit sigParking();
        return;
    }//if addr

    _bdd->set_i2cEclairage(QString(addr), "0", "0", "0", "6");

    U_LAMP octet;
    _i2c->lire(static_cast<unsigned char>(addr), &octet.octet, 1); // Lecture

    uint8_t indice = static_cast<uint8_t>(addr-addr_base);

    _zdc->setCellule(indice, octet.partie.bitJN);
    _zdc->setPresence(indice, octet.partie.bitPresence);
    _zdc->setLampFonct(indice, octet.partie.bitLamps);


    unsigned char ordre = _zdc->getConsigneEclair(indice);
    ordre |= ORDRE_RECU;

    if(ordre >= ORDRE_RECU){
        ordre -= ORDRE_RECU;
        _i2c->ecrire(static_cast<unsigned char>(addr), &ordre, 1);
        _zdc->setConsigneEclair(indice, ordre);
    }

    _bdd->mod_i2cEclairage(QString(addr), QString(ordre), QString(octet.partie.bitPresence), QString(octet.partie.bitPresence), QString(octet.partie.bitLamps));

    addr = addr + 1;

    emit sigEclair(addr, nb, addr_base);
}

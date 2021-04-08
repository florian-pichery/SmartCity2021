#include "ceclairage.h"

CEclairage::CEclairage(CZdc *zdc) : QObject(zdc)
{
    _zdc = zdc;
    _i2c = CI2c::getInstance(this, '1');
    connect(this, &CEclairage::sigEclair, this, &CEclairage::on_sigEclair);
    //Initialisation des variables concernant l'éclairage
    _zdc->setPresence(false);
    _zdc->setCellule(false);
    _zdc->setLampFonct(63);//6 lampadaires fonctionnants
    _zdc->setConsigneEclair(0); //Éteint par défaut
    //Fin Init
}

CEclairage::~CEclairage()
{
    CI2c::freeInstance();
}

void CEclairage::on_sigEclair(int addr, int nb, int addr_base)
{
    if (addr > addr_base+nb-1){
        emit sigParking();
        return;
    }//if addr


    unsigned char eclairage[1];
    _i2c->lire(static_cast<unsigned char>(addr), eclairage, 1); // Lecture
    T_LAMP *lamp;
    lamp = reinterpret_cast<T_LAMP *>(&eclairage[0]);

    emit sigInter();

    _zdc->setCellule(lamp->bitJN);
    _zdc->setPresence(lamp->bitPresence);
    _zdc->setLampFonct(lamp->bitLamps);

//    //Test I2C avec Ninnin à voir

//    unsigned char ordre;
//    ordre = 1;

//    _i2c->ecrire(addr, &ordre, 1);
//    _zdc->setConsigne(ordre);

    if(_zdc->getConsigneEclair() >= 128){
        unsigned char ordre;
        ordre = _zdc->getConsigneEclair();

        _i2c->ecrire(static_cast<unsigned char>(addr), &ordre, 1);
        _zdc->setConsigneEclair(0);
    }
    emit sigEclair(addr, nb, addr_base);
}

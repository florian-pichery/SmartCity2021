#include "czdc.h"


CZdc::CZdc()
{
    setKey(KEY);
    if (!attach())
        if (!create(sizeof(T_ZDC))) { // RW
            //emit sig_erreur("CZdc::CZdc Erreur de création de la SHM");
            qDebug() << "CZdc::CZdc Erreur de création de la SHM";
        } // if erreur
    _adrZdc = static_cast<T_ZDC *>(data());
    //clear();  // init de toute la ZDC
}

CZdc::~CZdc()
{
    detach();
}

void CZdc::clear()
{
    // RAZ de toutes les informations
    lock();
        bzero(_adrZdc, sizeof(T_ZDC));
    unlock();
}

void CZdc::setBarriersState(bool state, int msk)
{
    lock();
        (state ? _adrZdc->parking.etats|=msk : _adrZdc->parking.etats &=~msk);
    unlock();
}

void CZdc::setBarriersOrder(uint8_t parkOrder)
{
    lock();

    unlock();

    emit sig_OrderBarrier(parkOrder & 0x0F);// Masque les acquittements des ordres
}

void CZdc::setCpt(uint8_t places)
{
    lock();
        places = 8;
    unlock();
}

void CZdc::setRfid(uint8_t rfid[5])
{
    lock();

    unlock();

    emit sig_RFID(rfid);
}

void CZdc::getRfid(uint8_t rfid[5])
{
    lock();

    unlock();
}

void CZdc::setConsigne(uint8_t consigne)
{
    lock();

    unlock();

    emit sig_Consigne(consigne);
}

void CZdc::getPresence(bool presence)
{
    lock();

    unlock();

}

void CZdc::getCellule(bool cellule)
{
    lock();

    unlock();
}

void CZdc::getBoutonPieton(uint8_t boutonPieton)
{
    lock();

    unlock();
}

void CZdc::setMode(uint8_t mode)
{
    lock();

    unlock();

    emit sig_Mode(mode);
}

void CZdc::setOrdres(uint8_t interOrdre)
{
    lock();

    unlock();

    emit sig_OrderInter(interOrdre);
}

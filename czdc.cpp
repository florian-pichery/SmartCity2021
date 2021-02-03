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
        parkOrder = _adrZdc->parking.parkOrdre;
    unlock();

    emit sig_OrderBarrier(parkOrder & 0x0F);// Masque les acquittements des ordres
}

uint8_t CZdc::setCpt()
{
    uint8_t places;
    lock();
        places = _adrZdc->parking.cptPlaces;
    unlock();
    return places;
}

void CZdc::setRfid(uint8_t rfid[5])
{
    lock();
        rfid = _adrZdc->parking.rfid;
    unlock();

    emit sig_RFID(rfid);
}

void CZdc::setLigneSup(char liSup[17])
{
    lock();
        liSup = _adrZdc->parking.affLigneSup;
    unlock();
}

void CZdc::setLigneInf(char liInf[17])
{
    lock();
        liInf = _adrZdc->parking.affLigneInf;
    unlock();
}

uint8_t CZdc::getRfid()
{
    uint8_t rfid[5];
    lock();

    unlock();
    return *rfid;// à faire checker par le prof pour cette méthode
}

void CZdc::setConsigne(uint8_t consigne)
{
    lock();
        consigne = _adrZdc->eclairage->consigne;
    unlock();

    emit sig_Consigne(consigne);
}

bool CZdc::getPresence()
{
    bool presence;
    lock();
        presence = _adrZdc->eclairage->presence;
    unlock();
    return presence;
}

bool CZdc::getCellule()
{
    bool cellule;
    lock();
        cellule = _adrZdc->eclairage->cellule;
    unlock();
    return cellule;
}

uint8_t CZdc::getBoutonPieton()
{
    uint8_t boutonPieton;
    lock();
        boutonPieton = _adrZdc->intersection.boutonPieton;
    unlock();
    return boutonPieton;
}

void CZdc::setMode(bool mode)
{
    lock();
        mode = _adrZdc->intersection.mode;
    unlock();

    emit sig_Mode(mode);
}

//void CZdc::setOrdres(uint8_t interOrdre)
//{
//    lock();

//    unlock();

//    emit sig_OrderInter(interOrdre);
//}

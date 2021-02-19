#include "czdc.h"


CZdc::CZdc()
{
    CConfig cfg;
    int nb;
    nb =sizeof(T_ZDC)+sizeof(T_ECLAIRAGE)*static_cast<uint8_t>(cfg._nbEclair.toUInt());
    setKey(KEY);
    if (!attach())
        if (!create(nb)) { // RW
            //emit sig_erreur("CZdc::CZdc Erreur de création de la SHM");
            qDebug() << "CZdc::CZdc Erreur de création de la SHM";
        } // if erreur
    _adrZdc = static_cast<T_ZDC *>(data());
    bzero(_adrZdc, nb);
    _adrZdc->eclairage = (T_ECLAIRAGE *)(_adrZdc + 1);

    setAddrPark(static_cast<uint8_t>(cfg._addrPark.toUInt(nullptr,16)));
    setAddrInter(static_cast<uint8_t>(cfg._addrInter.toUInt(nullptr,16)));
    setAddrEclair(static_cast<uint8_t>(cfg._addrEclair.toUInt(nullptr,16)));

    //clear();  // init de toute la ZDC
}

CZdc::~CZdc()
{
    detach();
}

void CZdc::setAddrPark(uint8_t addrP)
{
    lock();
        _adrZdc->parking.addr = addrP;
    unlock();
}

void CZdc::setAddrInter(uint8_t addrI)
{
    lock();
        _adrZdc->intersection.addr = addrI;
    unlock();
}

void CZdc::setAddrEclair(uint8_t addrE)
{
    lock();
        _adrZdc->eclairage = new T_ECLAIRAGE;
        _adrZdc->eclairage->addr = addrE;
    unlock();
}

void CZdc::clear()
{
    // RAZ de toutes les informations
    lock();
        CConfig cfg;
        bzero(_adrZdc, sizeof(T_ZDC)+sizeof(T_ECLAIRAGE)*static_cast<uint8_t>(cfg._nbEclair.toUInt()));// ràz de la bdd en fonction du nombre de cartes éclairage
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
        _adrZdc->parking.parkOrdre = parkOrder;
    unlock();

    emit sig_OrderBarrier(parkOrder & 0x0F);// Masque les acquittements des ordres
}

void CZdc::setCpt(uint8_t places)
{
    lock();
        _adrZdc->parking.cptPlaces = places;
    unlock();
}

void CZdc::setCptPlus(uint8_t places)
{
    lock();
        places++;
        _adrZdc->parking.cptPlaces = places;
    unlock();

    emit sig_Cpt(places);
}

void CZdc::setCptMoins(uint8_t places)
{
    lock();
        places--;
        _adrZdc->parking.cptPlaces = places;
    unlock();

    emit sig_Cpt(places);
}

void CZdc::setRfidE(QByteArray rfid)
{
    lock();
       for(int i = 0; i < 5; i++)
         _adrZdc->parking.rfidE[i] = rfid[i];
    unlock();

    emit sig_setRFIDe(rfid);
}

void CZdc::setRfidS(QByteArray rfid)
{
    lock();
       for(int i = 0; i < 5; i++)
         _adrZdc->parking.rfidS[i] = rfid[i];
    unlock();

    emit sig_setRFIDs(rfid);
}

void CZdc::setLigneSup(QByteArray liSup)
{
    lock();
      for(int i = 0; i < 17; i++)
        _adrZdc->parking.affLigneSup[i] = liSup[i];
    unlock();

    emit sig_ligneSup(liSup);
}

void CZdc::setLigneInf(QByteArray liInf)
{
    lock();
      for(int i = 0; i < 17; i++)
        _adrZdc->parking.affLigneInf[i] = liInf[i];
    unlock();

    emit sig_ligneInf(liInf);
}

QByteArray CZdc::getRfidE(QByteArray rfid)
{
    lock();
       for(int i = 0; i < 5; i++)
            rfid[i] = _adrZdc->parking.rfidE[i];
    unlock();
    return rfid;
}

QByteArray CZdc::getRfidS(QByteArray rfid)
{
    lock();
       for(int i = 0; i < 5; i++)
            rfid[i] = _adrZdc->parking.rfidS[i];
    unlock();
    return rfid;
}

void CZdc::setConsigne(uint8_t consigne)
{
    lock();
        _adrZdc->eclairage->consigne = consigne;
    unlock();

    emit sig_Consigne(consigne);
}

bool CZdc::getPresence(bool presence)
{
    lock();
        presence = _adrZdc->eclairage->presence;
    unlock();
    return presence;
}

bool CZdc::getCellule(bool cellule)
{
    lock();
        cellule = _adrZdc->eclairage->cellule;
    unlock();
    return cellule;
}

uint8_t CZdc::getBoutonPieton(uint8_t boutonPieton)
{
    lock();
        boutonPieton = _adrZdc->intersection.boutonPieton;
    unlock();
    return boutonPieton;
}

void CZdc::setMode(uint8_t mode)
{
    lock();
        _adrZdc->intersection.mode = mode;
    unlock();

    emit sig_Mode(mode);
}

void CZdc::setOrdres(uint8_t interOrdre)
{
    lock();
        _adrZdc->intersection.interOrdre = interOrdre;
    unlock();

    emit sig_OrderInter(interOrdre);
}

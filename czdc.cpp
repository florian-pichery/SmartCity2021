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
    bzero(_adrZdc, static_cast<size_t>(nb));
    _adrZdc->eclairage = reinterpret_cast<T_ECLAIRAGE *>(_adrZdc + 1);

    setAddrPark(cfg._addrPark.toInt(nullptr,16));
    setAddrInter(cfg._addrInter.toInt(nullptr,16));
    setAddrEclair(cfg._addrEclair.toInt(nullptr,16));

    setNbEclairage(static_cast<unsigned char>(cfg._nbEclair.toUInt(nullptr, 10)));

    //clear();  // init de toute la ZDC
}

CZdc::~CZdc()
{
    detach();
}

void CZdc::setNbEclairage(uint8_t nb)
{
    lock();
        _adrZdc->eclairage->nbEclair = nb;
    unlock();
}

uint8_t CZdc::getNbEclairage()
{
    uint8_t nb;
    lock();
        nb = uint8_t(config._nbEclair.toUInt());
    unlock();
    return nb;
}

void CZdc::setAddrPark(int addrP)
{
    lock();
        _adrZdc->parking.addr = addrP;
    unlock();
}

void CZdc::setAddrInter(int addrI)
{
    lock();
        _adrZdc->intersection.addr = addrI;
    unlock();
}

void CZdc::setAddrEclair(int addrE)
{
    lock();
        _adrZdc->eclairage = new T_ECLAIRAGE;
        _adrZdc->eclairage->addr = addrE;
    unlock();
}

int CZdc::getAddrPark()
{
    int addr;
    lock();
        addr = _adrZdc->parking.addr;
    unlock();
    return addr;
}

int CZdc::getAddrInter()
{
    int addr;
    lock();
        addr = _adrZdc->intersection.addr;
    unlock();
    return addr;
}

int CZdc::getAddrEclair()
{
    int addr;
    lock();
        addr = _adrZdc->eclairage->addr;
    unlock();
    return addr;
}//à voir

void CZdc::clear()
{
    // RAZ de toutes les informations
    lock();
        CConfig cfg;
        bzero(_adrZdc, sizeof(T_ZDC)+sizeof(T_ECLAIRAGE)*static_cast<uint8_t>(cfg._nbEclair.toUInt()));// ràz de la bdd en fonction du nombre de cartes éclairage
    unlock();
}

void CZdc::setBarriersState(uint8_t parkState)
{
    lock();
        _adrZdc->parking.etats = parkState;
    unlock();
}

void CZdc::setBarriersOrder(uint8_t parkOrder)
{
    lock();
        _adrZdc->parking.parkOrdre = parkOrder;
    unlock();

    emit sig_OrderBarrier(parkOrder & 0x0F);// Masque les acquittements des ordres
}

uint8_t CZdc::getBarriersOrder()
{
    uint8_t order;
    lock();
        order = _adrZdc->parking.parkOrdre;
    unlock();
    return order;
}

void CZdc::setCpt(uint8_t places)
{
    lock();
        _adrZdc->parking.cptPlaces = places;
    unlock();
}

uint8_t CZdc::getCpt()
{
    uint8_t places;
    lock();
        places = _adrZdc->parking.cptPlaces;
    unlock();
    return places;
}

void CZdc::setCptPlus()
{
    lock();
         uint8_t places = getCpt();
         places++;
        _adrZdc->parking.cptPlaces = places;
    unlock();

    emit sig_Cpt(places);
}

void CZdc::setCptMoins()
{
    lock();
        uint8_t places = getCpt();
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

QByteArray CZdc::getRfidE()
{
    QByteArray rfid;
    lock();
       for(int i = 0; i < 5; i++)
            rfid[i] = _adrZdc->parking.rfidE[i];
    unlock();
    return rfid;
}

QByteArray CZdc::getRfidS()
{
    QByteArray rfid;
    lock();
       for(int i = 0; i < 5; i++)
            rfid[i] = _adrZdc->parking.rfidS[i];
    unlock();
    return rfid;
}

void CZdc::setConsigneEclair(uint8_t consigne)
{
    lock();
        _adrZdc->eclairage->consigne = consigne;
    unlock();

    emit sig_ConsigneEclair(consigne);
}

void CZdc::setLampFonct(uint8_t nb)
{
    lock();
        _adrZdc->eclairage->lampFonct = nb;
    unlock();

    emit sig_ConsigneEclair(nb);
}

uint8_t CZdc::getLampFonct()
{
    uint8_t nb;
    lock();
        nb = _adrZdc->eclairage->lampFonct;
    unlock();
    return nb;
}

uint8_t CZdc::getConsigneEclair()
{
    uint8_t consigne;
    lock();
        consigne = _adrZdc->eclairage->consigne;
    unlock();
    return consigne;
}

void CZdc::setPresence(bool presence)
{
    lock();
        _adrZdc->eclairage->presence = presence;
    unlock();
}

bool CZdc::getPresence()
{
    bool presence;
    lock();
        presence = _adrZdc->eclairage->presence;
    unlock();
    return presence;
}

void CZdc::setCellule(bool cellule)
{
    lock();
        _adrZdc->eclairage->cellule = cellule;
    unlock();
}

bool CZdc::getCellule()
{
    bool cellule;
    lock();
        cellule = _adrZdc->eclairage->cellule;
    unlock();
    return cellule;
}

void CZdc::setBoutonPietonVoie1(uint8_t bp)
{
    lock();
        _adrZdc->intersection.boutonPieton1 = bp;
    unlock();
}

uint8_t CZdc::getBoutonPietonVoie1()
{
    uint8_t boutonPieton;
    lock();
        boutonPieton = _adrZdc->intersection.boutonPieton1;
    unlock();
    return boutonPieton;
}

void CZdc::setModeVoies(uint8_t mode)
{
    lock();
        _adrZdc->intersection.mode = mode;
    unlock();

    emit sig_ModeVoies(mode);
}

uint8_t CZdc::getModeVoies()
{
    uint8_t mode;
    lock();
        mode = _adrZdc->intersection.mode;
    unlock();
    return mode;
}

void CZdc::setOrdresFeu1(uint8_t interOrdre)
{
    lock();
        _adrZdc->intersection.interOrdre1 = interOrdre;
    unlock();

    emit sig_OrderInter1(interOrdre);
}

uint8_t CZdc::getOrdresFeu1()
{
    uint8_t interOrdre;
    lock();
        interOrdre = _adrZdc->intersection.interOrdre1;
    unlock();
    return interOrdre;
}

void CZdc::setBoutonPietonVoie2(uint8_t bp)
{
    lock();
        _adrZdc->intersection.boutonPieton2 = bp;
    unlock();
}

uint8_t CZdc::getBoutonPietonVoie2()
{
    uint8_t boutonPieton;
    lock();
        boutonPieton = _adrZdc->intersection.boutonPieton2;
    unlock();
    return boutonPieton;
}

void CZdc::setOrdresFeu2(uint8_t interOrdre)
{
    lock();
        _adrZdc->intersection.interOrdre2 = interOrdre;
    unlock();

    emit sig_OrderInter2(interOrdre);
}

uint8_t CZdc::getOrdresFeu2()
{
    uint8_t interOrdre;
    lock();
        interOrdre = _adrZdc->intersection.interOrdre2;
    unlock();
    return interOrdre;
}

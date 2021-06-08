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
    // init de toute la ZDC
}

CZdc::~CZdc()
{
    detach();
}

void CZdc::init()
{
    CConfig cfg;
    clear();
    _adrZdc->eclairage = reinterpret_cast<T_ECLAIRAGE *>(_adrZdc + 1);
    setAddrPark(cfg._addrPark.toInt(nullptr,16));
    setAddrInter(cfg._addrInter.toInt(nullptr,16));
    setAddrEclair(cfg._addrEclair.toInt(nullptr,16));
    setNbEclairage(static_cast<unsigned char>(cfg._nbEclair.toUInt(nullptr, 10)));
    setCpt(8);
    QString nb = QString::number(getCpt());
    QString sup;
    sup = "PLACES LIBRES:";
    setLigneSup(sup);
    setLigneInf(nb);
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
    _adrZdc->eclairage[0].addr = addrE;
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
    addr = _adrZdc->eclairage[0].addr;
    unlock();
    return addr;
}

void CZdc::clear()
{
    // RAZ de toutes les informations
    lock();
    CConfig cfg;
    bzero(_adrZdc, sizeof(T_ZDC)+sizeof(T_ECLAIRAGE)*static_cast<uint8_t>(cfg._nbEclair.toUInt()));// ràz de la bdd en fonction du nombre de cartes éclairage
    unlock();
}

void CZdc::setEtatsBarrieres(uint8_t parkState)
{
    lock();
    _adrZdc->parking.etats = parkState;
    unlock();
}

uint8_t CZdc::getEtatsBarrieres()
{
    uint8_t parkState;
    lock();
    parkState = _adrZdc->parking.etats;
    unlock();
    return parkState;
}

void CZdc::setOrdreBarrieres(uint8_t parkOrder)
{
    lock();
    _adrZdc->parking.parkOrdre = parkOrder;
    unlock();
}

uint8_t CZdc::getOrdreBarrieres()
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
}

void CZdc::setCptMoins()
{
    lock();
    uint8_t places = getCpt();
    places--;
    _adrZdc->parking.cptPlaces = places;
    unlock();
}

void CZdc::setRfidE(QByteArray rfid)
{
    lock();
    for(int i = 0; i < 5; i++)
        _adrZdc->parking.rfidE[i] = rfid[i];
    unlock();
}

void CZdc::setRfidS(QByteArray rfid)
{
    lock();
    for(int i = 0; i < 5; i++)
        _adrZdc->parking.rfidS[i] = rfid[i];
    unlock();
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

void CZdc::setLigneSup(QString &liSup)
{
    int nb = liSup.size();
    lock();
    bzero(_adrZdc->parking.affLigneSup, sizeof(_adrZdc->parking.affLigneSup));
    memcpy(_adrZdc->parking.affLigneSup, liSup.left(16).toStdString().c_str(),(nb>16?16:nb));
    unlock();
}

void CZdc::setLigneInf(QString &liInf)
{
    int nb = liInf.size();
    lock();
    bzero(_adrZdc->parking.affLigneInf, sizeof(_adrZdc->parking.affLigneInf));
    memcpy(_adrZdc->parking.affLigneInf, liInf.left(16).toStdString().c_str(),(nb>16?16:nb));
    unlock();
}

QString CZdc::getLigneSup()
{
    QString sup;
    lock();
    sup = _adrZdc->parking.affLigneSup;
    unlock();
    return sup;
}

QString CZdc::getLigneInf()
{
    QString inf;
    lock();
    inf = _adrZdc->parking.affLigneInf;
    unlock();
    return inf;
}

void CZdc::setConsigneEclair(uint8_t noCarte, uint8_t consigne)
{
    lock();
    _adrZdc->eclairage[noCarte].consigne = consigne;
    unlock();
}

uint8_t CZdc::getConsigneEclair(uint8_t noCarte)
{
    uint8_t consigne;
    lock();
    consigne = _adrZdc->eclairage[noCarte].consigne;
    unlock();
    return consigne;
}

void CZdc::setLampFonct(uint8_t noCarte, uint8_t nb)
{
    lock();
    _adrZdc->eclairage[noCarte].lampFonct = nb;
    unlock();
}

uint8_t CZdc::getLampFonct(uint8_t noCarte)
{
    uint8_t nb;
    lock();
    nb = _adrZdc->eclairage[noCarte].lampFonct;
    unlock();
    return nb;
}

void CZdc::setPresence(uint8_t noCarte, bool presence)
{
    lock();
    _adrZdc->eclairage[noCarte].presence = presence;
    unlock();
}

bool CZdc::getPresence(uint8_t noCarte)
{
    bool presence;
    lock();
    presence = _adrZdc->eclairage[noCarte].presence;
    unlock();
    return presence;
}

void CZdc::setCellule(uint8_t noCarte, bool cellule)
{
    lock();
    _adrZdc->eclairage[noCarte].cellule = cellule;
    unlock();
}

bool CZdc::getCellule(uint8_t noCarte)
{
    bool cellule;
    lock();
    cellule = _adrZdc->eclairage[noCarte].cellule;
    unlock();
    return cellule;
}

void CZdc::setEtatMode(uint8_t mode)
{
    lock();
    _adrZdc->intersection.etatMode = mode;
    unlock();
}

uint8_t CZdc::getEtatMode()
{
    uint8_t mode;
    lock();
    mode = _adrZdc->intersection.etatMode;
    unlock();
    return mode;
}

void CZdc::setModeVoies(uint8_t mode)
{
    lock();
    _adrZdc->intersection.ordreMode = mode;
    unlock();
}

uint8_t CZdc::getModeVoies()
{
    uint8_t mode;
    lock();
    mode = _adrZdc->intersection.ordreMode;
    unlock();
    return mode;
}

void CZdc::setOrdresFeu1(uint8_t interOrdre)
{
    lock();
    _adrZdc->intersection.interOrdre1 = interOrdre;
    unlock();
}

uint8_t CZdc::getOrdresFeu1()
{
    uint8_t interOrdre;
    lock();
    interOrdre = _adrZdc->intersection.interOrdre1;
    unlock();
    return interOrdre;
}

void CZdc::setInterEtat1(uint8_t etat)
{
    lock();
    _adrZdc->intersection.interEtat1 = etat;
    unlock();
}

uint8_t CZdc::getInterEtat1()
{
    uint8_t etat;
    lock();
    etat = _adrZdc->intersection.interEtat1;
    unlock();
    return etat;
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
}

uint8_t CZdc::getOrdresFeu2()
{
    uint8_t interOrdre;
    lock();
    interOrdre = _adrZdc->intersection.interOrdre2;
    unlock();
    return interOrdre;
}

void CZdc::setInterEtat2(uint8_t etat)
{
    lock();
    _adrZdc->intersection.interEtat2 = etat;
    unlock();
}

uint8_t CZdc::getInterEtat2()
{
    uint8_t etat;
    lock();
    etat = _adrZdc->intersection.interEtat1;
    unlock();
    return etat;
}

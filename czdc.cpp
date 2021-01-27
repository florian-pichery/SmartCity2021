#include "czdc.h"


CZdc::CZdc(QObject *parent) : QSharedMemory (parent)
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


void CZdc::setEtatBarriers(bool state, int msk)
{
    (state ? _adrZdc->parking.etats|=msk : _adrZdc->parking.etats &=~msk);
}

void CZdc::getDetect()
{

}


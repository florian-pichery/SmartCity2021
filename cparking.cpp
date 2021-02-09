#include "cparking.h"

CParking::CParking(QObject *parent) : QObject(parent)
{

}

void CParking::on_OrderBarrier(uint8_t ordres)
{
  // _zdc->setBarriersState(true, BEM);
} // à faire plus tard

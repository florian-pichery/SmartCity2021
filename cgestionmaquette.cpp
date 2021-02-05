#include "cgestionmaquette.h"

CGestionMaquette::CGestionMaquette(CZdc *zdc)
{
    _zdc = zdc;
    _ecran = nullptr;
    _inter = nullptr;
    _park = nullptr;
    _eclair = nullptr;
    _gthm = new QThread;

    CI2c::getInstance();

    _zdc->setBarriersState(true, BEM);
}

CGestionMaquette::~CGestionMaquette()
{
    CI2c::freeInstance();
    delete _gthm;
}



#include "cgestionmaquette.h"

CGestionMaquette::CGestionMaquette(CZdc *zdc)
{
    _zdc = zdc;
    _ecran = nullptr;
    _inter = nullptr;
    _park = nullptr;
    _eclair = nullptr;



    _zdc->setBarriersState(true, BEM);
}

CGestionMaquette::~CGestionMaquette()
{

}

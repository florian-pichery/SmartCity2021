#include "cgestionmaquette.h"

CGestionMaquette::CGestionMaquette()
{
    _zdc = new CZdc;
    _ecran = nullptr;
    _inter = nullptr;
    _park = nullptr;
    _eclair = nullptr;

    _zdc->setEtatBarriers(true, BEM);
}

CGestionMaquette::~CGestionMaquette()
{

}

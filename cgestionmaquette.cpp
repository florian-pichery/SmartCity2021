#include "cgestionmaquette.h"

CGestionMaquette::CGestionMaquette()
{
    _zdc = new CZdc();
    _ecran = new CEcran(_zdc, nullptr);
    _eclair = new CEclairage(_zdc, nullptr);
    _park = new CParking(_zdc, nullptr);
    _inter = new CIntersection(_zdc, nullptr);

    connect(this, &CGestionMaquette::sigEclair, _eclair, &CEclairage::on_sigEclair);
    connect(_eclair, &CEclairage::sigParking, _park, &CParking::onPark);
    connect(_park, &CParking::sigEcran, _ecran, &CEcran::onEcran);
    connect(_ecran, &CEcran::sigInter, _inter, &CIntersection::onInter);
    connect(_inter, &CIntersection::sigRestart, this, &CGestionMaquette::on_go);

//    connect(this, &CGestionMaquette::sigInter, _inter, &CIntersection::onInter);

//    connect(this, &CGestionMaquette::sigPark, _park, &CParking::onPark);
//    connect(_park, &CParking::sigRestart, this, &CGestionMaquette::on_go);

}

CGestionMaquette::~CGestionMaquette()
{
    delete _inter;
    delete _park;
    delete _eclair;
    delete _ecran;
    delete _zdc;
}

void CGestionMaquette::on_go()
{
//    emit sigInter();

//    emit sigPark();

//    //Eclairage : lance le cycle de l'éclairage
    int addr = _zdc->getAddrEclair(); //Passage des adresses en int dû à des problèmes de compilation avec le uint8_t
    int nb = _zdc->getNbEclairage();
    emit sigEclair(addr, nb, addr);
}

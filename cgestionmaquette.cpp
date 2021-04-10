#include "cgestionmaquette.h"

CGestionMaquette::CGestionMaquette(CZdc *zdc)
{
    _zdc = zdc;
    _ecran = new CEcran(zdc, nullptr);
    _eclair = new CEclairage(zdc, nullptr);
    _park = new CParking(zdc, nullptr);
    _inter = new CIntersection(zdc, nullptr);

    //connect(this, &CGestionMaquette::sigEcran, _ecran, &CEcran::onEcran);

    connect(this, &CGestionMaquette::sigEclair, _eclair, &CEclairage::on_sigEclair);
    //connect(this, &CGestionMaquette::sigPark, _park, &CParking::onPark);
    connect(_eclair, &CEclairage::sigParking, _park, &CParking::onPark);
    connect(_park, &CParking::sigEcran, _ecran, &CEcran::onEcran);
    connect(_ecran, &CEcran::sigInter, _inter, &CIntersection::onInter);
    connect(_inter, &CIntersection::sigRestart, this, &CGestionMaquette::on_go);

}//Faire un diagramme

CGestionMaquette::~CGestionMaquette()
{
    delete _inter;
    delete _ecran;
    delete _park;
    delete _eclair;
}

void CGestionMaquette::on_go()
{
    //Eclairage : lance le cycle de l'éclairage
    int addr = _zdc->getAddrEclair(); //Passage des adresses en int dû à des problèmes de compilation avec le uint8_t
    int nb = _zdc->getNbEclairage();
    emit sigEclair(addr, nb, addr);

    //emit sigPark();

    //emit sigEcran("3");
}

#include "capp.h"

CApp::CApp(QObject *parent) : QObject(parent)
{
    _zdc = new CZdc();
    CConfig cfg;
    _gthm = new QThread();
    _maquette = new CGestionMaquette(_zdc);
    _maquette->moveToThread(_gthm);
    connect(this, &CApp::sig_go, _maquette, &CGestionMaquette::on_go);
    connect(_gthm, &QThread::finished, _maquette, &QObject::deleteLater);
    _gthm->start();
    emit sig_go();

}

CApp::~CApp()
{
    _gthm->quit();
    _gthm->wait();
    delete _zdc;
}


//POUR L'IHM

void CApp::setAddrEclair(uint8_t addrEclair)
{
    _zdc->setAddrEclair(addrEclair);
}

void CApp::setAddrInter(uint8_t addrInter)
{
    _zdc->setAddrInter(addrInter);
}

void CApp::setAddrPark(uint8_t addrPark)
{
    _zdc->setAddrPark(addrPark);
}

/*Partie éclairage à refaire*/

//void CApp::setConsigne(uint8_t consigne)
//{
//    _zdc->setConsigneEclair(consigne);

//    consigne = _zdc->getConsigneEclair();

//    switch(consigne){
//    case 0:
//        emit sig_msg_consigne(" 0%");
//        break;
//    case 1:
//        emit sig_msg_consigne(" 50%");
//        break;
//    case 2:
//        emit sig_msg_consigne(" 100%");
//        break;
//    default:
//        emit sig_msg_consigne("Erreur valeur");
//        break;
//    }
//}

//void CApp::setCellule(bool cellule)
//{
//    _zdc->setCellule(cellule);

//    cellule = _zdc->getCellule();

//    if(cellule == true){
//        emit sig_msg_cellule(" 1 (Jour)");
//    }

//    if(cellule == false){
//        emit sig_msg_cellule(" 0 (Nuit)");
//    }
//}

//void CApp::setPresence(bool presence)
//{
//    _zdc->setPresence(presence);

//    presence = _zdc->getPresence();

//    if(presence == true){
//        emit sig_msg_presence(" 1 (Présence)");
//    }

//    if(presence == false){
//        emit sig_msg_presence(" 0 (RAS)");
//    }
//}

void CApp::setInterOrdre(uint8_t interOrdre)
{
    _zdc->setOrdresFeu1(interOrdre);

    interOrdre = _zdc->getOrdresFeu1();

    switch(interOrdre){
    case 0:
        emit sig_msg_interOrdre1(" rouge");
        break;
    case 1:
        emit sig_msg_interOrdre1(" orange");
        break;
    case 2:
        emit sig_msg_interOrdre1(" vert");
        break;
    //voie 2
    case 128:
        emit sig_msg_interOrdre2(" rouge");
        break;
    case 129:
        emit sig_msg_interOrdre2(" orange");
        break;
    case 130:
        emit sig_msg_interOrdre2(" vert");
        break;
    default:
        emit sig_msg_interOrdre1("Erreur valeur");
        emit sig_msg_interOrdre2("Erreur valeur");
        break;
    }
}

void CApp::setMode(uint8_t mode)
{
    _zdc->setModeVoies(mode);

    mode = _zdc->getModeVoies();

    switch (mode){
    //Voie 1
    case 0:
        emit sig_msg_mode(" auto");
        break;
    case 1:
        emit sig_msg_mode(" clignotant");
        break;
    case 2:
        emit sig_msg_mode(" manuel");
        break;
    default:
        emit sig_msg_mode("Erreur valeur");
        break;
    }
}

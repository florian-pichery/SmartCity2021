#include "capp.h"

CApp::CApp(QObject *parent) : QObject(parent)
{
    _zdc = new CZdc();
    _zdc->init();
    CConfig cfg;
    _gthm = new QThread();
    _maquette = new CGestionMaquette();
    _maquette->moveToThread(_gthm);
    connect(this, &CApp::sig_go, _maquette, &CGestionMaquette::on_go);
    connect(_gthm, &QThread::finished, _maquette, &QObject::deleteLater);
    _gthm->start();
    emit sig_go();

    _serv = new CGererServeur(PORT);
    connect(_serv, &CGererServeur::sig_erreur, this, &CApp::on_erreur);
    connect(_serv, &CGererServeur::sig_info, this, &CApp::on_info);
}

CApp::~CApp()
{
    delete _serv;
    _gthm->quit();
    _gthm->wait();
    delete _zdc;
}

//POUR L'IHM

void CApp::setAddrEclair(uint8_t addrEclair)
{
    _zdc->setAddrEclair(addrEclair);
}

uint8_t CApp::getAddrEclair()
{
    uint8_t addr;
    addr = static_cast<uint8_t>(_zdc->getAddrEclair());
    return addr;
}

void CApp::setAddrInter(uint8_t addrInter)
{
    _zdc->setAddrInter(addrInter);
}

void CApp::setAddrPark(uint8_t addrPark)
{
    _zdc->setAddrPark(addrPark);
}


void CApp::setConsigne(uint8_t consigne)
{
    _zdc->setConsigneEclair(0, consigne + ACK);
    consigne = _zdc->getConsigneEclair(0);

    switch(consigne){
    case 128:
        emit sig_msgConsigne(" 0%");
        break;
    case 129:
        emit sig_msgConsigne(" 50%");
        break;
    case 130:
        emit sig_msgConsigne(" 100%");
        break;
    default:
        emit sig_msgConsigne("Erreur valeur");
        break;
    }
}


void CApp::setCellule(bool cellule)
{
    _zdc->setCellule(0, cellule);

    cellule = _zdc->getCellule(0);

    if(cellule == true){
        emit sig_msgCellule(" 1 (Jour)");
    }

    if(cellule == false){
        emit sig_msgCellule(" 0 (Nuit)");
    }
}


void CApp::setPresence(bool presence)
{
    _zdc->setPresence(0 , presence);

    presence = _zdc->getPresence(0);

    if(presence == true){
        emit sig_msgPresence(" 1 (Présence)");
    }

    if(presence == false){
        emit sig_msgPresence(" 0 (RAS)");
    }
}

void CApp::setInterOrdre1(uint8_t interOrdre)
{
    _zdc->setOrdresFeu1(interOrdre);

    interOrdre = _zdc->getOrdresFeu1();

    switch(interOrdre){
    case 0:
        emit sig_msgInterOrdre1(" rouge");
        break;
    case 1:
        emit sig_msgInterOrdre1(" orange");
        break;
    case 2:
        emit sig_msgInterOrdre1(" vert");
        break;
    default:
        emit sig_msgInterOrdre1("Erreur valeur");
        break;
    }
}

void CApp::setInterOrdre2(uint8_t interOrdre)
{
    _zdc->setOrdresFeu2(interOrdre);

    interOrdre = _zdc->getOrdresFeu2();

        switch(interOrdre){
        case 0:
            emit sig_msgInterOrdre2(" rouge");
            break;
        case 1:
            emit sig_msgInterOrdre2(" orange");
            break;
        case 2:
            emit sig_msgInterOrdre2(" vert");
            break;
        default:
            emit sig_msgInterOrdre2("Erreur valeur");
            break;
        }
}

void CApp::setMode(uint8_t mode)
{
    _zdc->setModeVoies(mode+ACK);

    mode = _zdc->getModeVoies();
    mode -= ACK;

    switch (mode){
    //Voie 1
    case 0:
        emit sig_msgMode(" auto");
        break;
    case 1:
        emit sig_msgMode(" clignotant");
        break;
    case 2:
        emit sig_msgMode(" manuel");
        break;
    default:
        emit sig_msgMode("Erreur valeur");
        break;
    }
}

int CApp::getCpt()
{
    int val;
    val = _zdc->getCpt();
    return val;
}

void CApp::setCptPlus()
{
    _zdc->setCptPlus();
}

void CApp::setCptMoins()
{
    _zdc->setCptMoins();
}

void CApp::on_erreur(QString mess)
{
    emit sig_erreur(mess);  // remontée des erreurs à l'IHM
}

void CApp::on_info(QString mess)
{
    emit sig_info(mess);  // remontée des erreurs à l'IHM
}

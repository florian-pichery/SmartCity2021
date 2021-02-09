#include "capp.h"

CApp::CApp(QObject *parent) : QObject(parent)
{
    _serv = new CGererServeur(PORT);
    connect(_serv, &CGererServeur::sig_erreur, this, &CApp::on_erreur);
    connect(_serv, &CGererServeur::sig_info, this, &CApp::on_info);


    //_zdc = new CZdc();  // accès à la mémoire partagée commune
    //_zdc->setEtatApp(ETAT_APP_ATTENTE_CONNEXION);  // en attente de connexion d'un client

}

CApp::~CApp()
{
     delete _serv;
}

void CApp::on_erreur(QString mess)
{
    emit sig_erreur(mess);  // remontée des erreurs à l'IHM
}

void CApp::on_info(QString mess)
{
    emit sig_info(mess);  // remontée des erreurs à l'IHM
}


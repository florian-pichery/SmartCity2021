#include "capp.h"

CApp::CApp(QObject *parent) : QObject(parent)
{
    _serv = new CServerTcp(this, PORT);
    connect(_serv, &CServerTcp::sig_erreur, this, &CApp::on_erreur);
    connect(_serv, &CServerTcp::sig_info, this, &CApp::on_info);

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

void CApp::on_newClient(QTcpSocket *client)
{
    _gthc = new QThread();
    CGestionClient *_client = new CGestionClient(client);
    //connect
    _client->moveToThread(_gthc);
    _gthc->start();//lance le thread
    emit sig_goServer();//lance le travail du thread
}

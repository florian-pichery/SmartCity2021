#include "cservertcp.h"

CServerTcp::CServerTcp(QObject *parent, quint16 noPort): QObject (parent)
{
    m_noPort = noPort;
    //init();
    _server = new QTcpServer();
    connect(_server,&QTcpServer::newConnection, this, &CServerTcp::onNewConnectionClient);

    _server->listen(QHostAddress::AnyIPv4, m_noPort);

}

CServerTcp::~CServerTcp()
{
    // destruction optionnelle car déjà pris en charge par le serveur
    /*for (int i=0 ; i<_listeClients.size() ; i++) {
        if (_listeGestion.at(i)->isConnected()){
            delete (_listeClients.at(i));
            _listeThread.at(i)->quit();
            _listeThread.at(i)->wait();
        }
    } // for i*/

    _listeClients.clear();
    _listeGestion.clear();
    _listeThread.clear();
    sig_info("clients détruits");
    delete _server;
}
/*
int CServerTcp::init()
{
    return 1;
}
*/
///////////////////  SLOTs ////////////////////////

void CServerTcp::onNewConnectionClient()
{
    _sock = _server->nextPendingConnection();
    //_sock = new QTcpSocket;

    _client = new CGestionClient(nullptr, _sock);
    _gthc = new QThread();
    qDebug() << "CServerTcp::onNewConnectionClient() thread crée";

    qDebug() << "CServerTcp::onNewConnectionClient() gestionclient crée";
    //connect
    //_sock->moveToThread(_gthc);
    _client->moveToThread(_gthc);
    qDebug() << "CServerTcp::onNewConnectionClient() movetoThread";

    //connect(newClient, &QTcpSocket::disconnected, newClient, &QTcpSocket::deleteLater);
    //connect(newClient, &QTcpSocket::disconnected, _client, &CGestionClient::deleteLater);
    connect(_gthc, &QThread::finished, _client, &QObject::deleteLater);
    //connect(_gthc, &QThread::finished, _sock, &QObject::deleteLater);
    connect(_client,&CGestionClient::sig_socketDeconnected,this,&CServerTcp::onDisconnectedClient);

    //connect(_sock, &QTcpSocket::disconnected, this, &CServerTcp::onDisconnectedClient);
    connect(_client, &CGestionClient::sig_info,this , &CServerTcp::on_info);
    connect(_client, &CGestionClient::sig_erreur, this, &CServerTcp::on_erreur);

    _gthc->start();//lance le thread
    qDebug() << "CServerTcp::onNewConnectionClient() lancement thread";

    _listeClients.append(_sock);  // sauve l'adresse de l'objet dans la liste
    _listeGestion.append(_client);
    _listeThread.append(_gthc);
    sprintf(ch,"CServeurTcp::onNewConnectionClient, Nouvelle connexion : %p",static_cast<void*>(_sock));
    emit sig_info(ch);
}

void CServerTcp::onDisconnectedClient(QTcpSocket *s)
{
    sprintf(ch,"CServerTcp::onDisconnectedClient, Socket déconnectée :  %p",static_cast<void*>(s));
    emit sig_info(ch);
    int index = _listeClients.indexOf(static_cast<QTcpSocket*>(s));
//    delete _gthc;
//    _listeThread.at(index)->quit();
//    _listeThread.at(index)->wait();
//    delete _listeClients.at(index);
    _listeClients.removeAt(index);
    //delete _listeGestion.at(index);
    _listeGestion.removeAt(index);
    _listeThread.removeAt(index);
    emit sig_info("CServeurTcp::on_disconnected : Effacement d'un client");

    /*
    for (int i=0 ; i<_listeClients.size() ; i++) {
        int st = _listeClients.at(i)->state();
        if ( st == QAbstractSocket::UnconnectedState) {
            //delete _listeClients.at(i);
            _listeClients.removeAt(i);
            _listeGestion.removeAt(i);
            _listeThread.removeAt(i);
            i=0;
            continue;  // recommence la boucle avec i=0
        } // if connected
    } // for
    */
}

void CServerTcp::on_erreur(QString mess)
{
    emit sig_erreur(mess);
}

void CServerTcp::on_info(QString mess)
{
    emit sig_info(mess);
}


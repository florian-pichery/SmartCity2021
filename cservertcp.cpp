#include "cservertcp.h"

CServerTcp::CServerTcp(QObject *parent, quint16 noPort) :
    QTcpServer(parent)
{
    m_noPort = noPort;
    init();
}

CServerTcp::~CServerTcp()
{
    // destruction optionnelle car déjà pris en charge par le serveur
    for (int i=0 ; i<listeClients.size() ; i++) {
        listeClients.at(i)->close();
        delete listeClients.at(i);
    } // for i
}

int CServerTcp::init()
{
    listen(QHostAddress::Any, m_noPort);
    connect(this,SIGNAL(newConnection()), this, SLOT(onNewConnectionClient()));
    return 1;
}

///////////////////  SLOTs ////////////////////////

void CServerTcp::onNewConnectionClient()
{


    QTcpSocket *newClient;
    newClient = nextPendingConnection();

    _gthc = new QThread();
    _client = new CGestionClient(newClient);
    //connect
    _client->moveToThread(_gthc);
    _gthc->start();//lance le thread
    connect(newClient, &QTcpSocket::disconnected, newClient, &QTcpSocket::deleteLater);
    connect(newClient, &QTcpSocket::disconnected, this, &CServerTcp::onDisconnectedClient);


    qDebug() << "Nouvelle connexion : " << newClient;
    emit sig_info("new client");
    listeClients.append(newClient);  // sauve l'adresse de l'objet dans la liste
    //ici on peux creer émettre un signal pour créer un thread qui s'occupera du client

}

void CServerTcp::onDisconnectedClient()
{
    QTcpSocket *client = (QTcpSocket *)sender(); // Déterminer quel client ?
    listeClients.removeOne(client);
    /*if (_client->isConnected() == QAbstractSocket::UnconnectedState) {
        _client
    }*/
    //detruire le thread correspondant
    //la deconnection provoque l'arret du programme
}


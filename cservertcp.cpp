#include "cservertcp.h"

CServerTcp::CServerTcp(QObject *parent, quint16 noPort): QTcpServer (parent)
{
    m_noPort = noPort;
    init();
}

CServerTcp::~CServerTcp()
{
    // destruction optionnelle car déjà pris en charge par le serveur
    for (int i=0 ; i<_listeClients.size() ; i++) {
        if (_listeGestion.at(i)->isConnected()){
            delete (_listeClients.at(i));
            _listeThread.at(i)->quit();
            _listeThread.at(i)->wait();
        }
    } // for i

    _listeClients.clear();
    _listeGestion.clear();
    _listeThread.clear();
    sig_info("clients détruits");
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
    connect(newClient, &QTcpSocket::disconnected, _client, &CGestionClient::deleteLater);
    connect(_gthc, &QThread::finished, _client, &CGestionClient::deleteLater);

    connect(_client, &CGestionClient::sig_info,this , &CServerTcp::on_info);
    connect(_client, &CGestionClient::sig_erreur, this, &CServerTcp::on_erreur);


    sprintf(ch,"CServeurTcp::onNewConnectionClient, Nouvelle connexion : %p",static_cast<void*>(newClient));
    emit sig_info(ch);
    _listeClients.append(newClient);  // sauve l'adresse de l'objet dans la liste
    _listeGestion.append(_client);
    _listeThread.append(_gthc);
}

void CServerTcp::onDisconnectedClient()
{
    for (int i=0 ; i<_listeClients.size() ; i++) {
        if (_listeGestion.at(i)->isConnected() == QAbstractSocket::UnconnectedState) {
            //delete listeClient.at(i); // effacé par deleteLater()
            _listeClients.removeAt(i);
            _listeGestion.removeAt(i);
            _listeThread.removeAt(i);
            emit sig_info("CServeurTcp::on_disconnected : Effacement d'un client");
            i=0;
            continue;  // recommence la boucle avec i=0
        } // if connected
    } // for
}

void CServerTcp::on_erreur(QString mess)
{
    emit sig_erreur(mess);
}

void CServerTcp::on_info(QString mess)
{
    emit sig_info(mess);
}


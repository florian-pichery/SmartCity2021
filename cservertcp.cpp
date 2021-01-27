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

int CServerTcp::emettreVersClients(QString mess)
{
    for (int i=0 ; i<listeClients.size() ; i++) {
        listeClients.at(i)->write(mess.toStdString().c_str());
        qDebug() << "Envoi vers " << listeClients.at(i);
    } // for i
    return 1;
}//CGestionClient

int CServerTcp::init()
{
    listen(QHostAddress::Any, m_noPort);
    connect(this,SIGNAL(newConnection()), this, SLOT(onNewConnectionClient()));
    return 1;
}

///////////////////  SLOTs ////////////////////////

void CServerTcp::onNewConnectionClient()
{

    QTcpSocket *newClient = this->nextPendingConnection();
    emit sig_newClient(newClient);
    qDebug() << "Nouvelle connexion : " << newClient;
    emit sig_info("new client");
    if (newClient == nullptr)
        emit sigErreur(QAbstractSocket::ConnectionRefusedError);
    connect(newClient, SIGNAL(readyRead()), this, SLOT(onReadyReadClient()));
    connect(newClient, SIGNAL(disconnected()), this, SLOT(onDisconnectedClient()));
    listeClients.append(newClient);  // sauve l'adresse de l'objet dans la liste
    //ici on peux creer émettre un signal pour créer un thread qui s'occupera du client
    emit sigEvenementServeur("CON");
    emit sigAdrClient(newClient->localAddress().toString());
    emit sigMaJClients(listeClients);  // pour IHM
}

void CServerTcp::onDisconnectedClient()
{
    QTcpSocket *client = (QTcpSocket *)sender(); // Déterminer quel client ?
    emit sigEvenementServeur("DEC");
    listeClients.removeOne(client);
    delete client;
    emit sigMaJClients(listeClients);
}

void CServerTcp::onErreurReseau(QAbstractSocket::SocketError err)
{
    qDebug() << "Erreur réseau !";
    emit sigErreur(err);
}

void CServerTcp::onReadyReadClient()
{
    QByteArray ba;
    // Déterminer quel client ?
    QTcpSocket *client = (QTcpSocket *)sender();
    ba=client->readAll();
    qint64 nb = client->write("OK");
    if (nb == -1)
        qDebug() << "ERREUR ENVOI";
    qDebug() << "Client : " << client << ba.size() << " Caractères reçus.";
    emit sigDataClient(client->localAddress().toString(), QString(ba));
}//CgestionClient


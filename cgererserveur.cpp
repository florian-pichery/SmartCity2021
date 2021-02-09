#include "cgererserveur.h"

CGererServeur::CGererServeur(quint16 noPort, QObject *parent) : QObject(parent)
{
    m_noPort = noPort;
    _serv = new CMonServeurTcp();
    connect(_serv, &CMonServeurTcp::sig_sdClient, this, &CGererServeur::on_newConnection);
    _serv->listen(QHostAddress::AnyIPv4, m_noPort);
}

CGererServeur::~CGererServeur()

{// effacement des threads
    for (int i=0 ; i<_listeThread.size() ; i++) {
        _listeThread.at(i)->quit();  // demande au thread de se terminer
        _listeThread.at(i)->wait();  // attends la fin de la boucle événements
    } // for
    _listeThread.clear();
    _listeClient.clear();
    delete _serv;
}

void CGererServeur::on_newConnection(qintptr sd)
{
    // Nouvelle connexion d'un client
    // La méthode héritée QTcpServer::incomingConnection() a été redéfinie.

    QThread *gthc = new QThread();  // création du thread
    gthc->setObjectName("servGcl_"+QString::number(sd));    //changer son nom pour le reconnaitre dans pstree
    CGererClient *client = new CGererClient(sd, nullptr);  // il créera la socket de comm grace à sd

    client->moveToThread(gthc);// déplacement vers le thread

    connect(client, &CGererClient::sig_info, this, &CGererServeur::on_info);
    connect(client, &CGererClient::sig_erreur, this, &CGererServeur::on_erreur);

    connect(client, &CGererClient::sig_disconnected, this, &CGererServeur::on_disconnected); // provoque la destruction du client et du thread
    connect(this, &CGererServeur::sig_goGestionClient, client, &CGererClient::on_goGestionClient); // provoque création socket client
    connect(gthc, &QThread::finished, client, &QObject::deleteLater); // The object will be deleted when control returns to the event loop

    gthc->start(); // départ boucle des événements du thread.
    emit sig_goGestionClient(); // Départ de la gestion du client

    // Mémorisation des objets créés
    _listeClient.append(client);
    _listeThread.append(gthc);
}

void CGererServeur::on_disconnected()
{
    CGererClient *client = static_cast<CGererClient *>(sender());
    // le signal disconnected est parfois envoyé 2 fois !!! la 2ème fois, client = 0 !
    // Alors on filtre.
    if (client != nullptr) {
        QThread *gthc = client->thread(); // on récupère le thread qui a envoyé le signal
        int pos = _listeThread.indexOf(gthc); // on cherche la position
        if (pos > -1) {  // s'il existe
            _listeThread.at(pos)->quit();  // demande au thread de se terminer
            _listeThread.at(pos)->wait();  // attends la fin de la boucle événements
            // effacement de la liste des objets dépendant du client
            // _gcl s'effacera automatiquement avec le thread. C'est vérifié,
            //    mettre un point d'arrêt dans ~CGestionClient pour le prouver !
            _listeThread.removeAt(pos);
            _listeClient.removeAt(pos);
        } // if pos
        emit sig_info("CGererServeur::on_disconnected : Nb client restant : "+QString::number(_listeClient.size()));
    } // if 0
}

void CGererServeur::on_erreur(QString mess)
{
    emit sig_erreur(mess);
}

void CGererServeur::on_info(QString mess)
{
    emit sig_info(mess);
}


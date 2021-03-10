#include "cgestionclient.h"

CGestionClient::CGestionClient(QObject *parent, QTcpSocket *sock) : QObject(parent)
{
    //init
    _sock = sock;
    connect(_sock,&QTcpSocket::readyRead,this,&CGestionClient::on_readyRead);
    connect(_sock, &QTcpSocket::disconnected,this,&CGestionClient::on_disconnected);
    connect(_sock, static_cast<void(QAbstractSocket::*)(QAbstractSocket::SocketError)>(&QAbstractSocket::error),
          [=](QAbstractSocket::SocketError socketError){
               emit sig_info("Erreur socket : "+QString::number(socketError));
          });

    /*_modbus = new CModbusTcp();
    connect(_modbus, &CModbusTcp::sig_erreur, this, &CGestionClient::on_erreur);
    connect(_modbus, &CModbusTcp::sig_info, this, &CGestionClient::on_info);
    */
    //zdc

}

CGestionClient::~CGestionClient()
{/*
    if (_sock->isOpen()) {
        _sock->close();
        //delete _sock;
    }//if open
    delete _modbus;*/
}

bool CGestionClient::isConnected()
{
    return _sock->state();
}

void CGestionClient::on_readyRead()
{
    QByteArray bA;
//    quint64 lg = _sock->bytesAvailable();//combien de octets reçus
    bA = _sock->readAll();
    on_writeToClients("OK");

    sprintf(chaine,"Client : %p, %d caractères reçus",static_cast<void*>(_sock),bA.size());
    emit sig_info(chaine);
    sprintf(chaine,"Requete client : %s",bA.toStdString().c_str());
    emit sig_info(chaine);
}

void CGestionClient::on_writeToClients(QByteArray rep)
{
    qint64 nb = _sock->write(rep);
    if (nb == -1){
       qDebug() << "ERREUR ENVOI";
       sig_erreur("ERREUR ENVOI");
    }
}

void CGestionClient::on_erreur(QString mess)
{
    emit sig_erreur(mess);
}

void CGestionClient::on_info(QString mess)
{
    emit sig_info(mess);
}

void CGestionClient::on_disconnected()
{
    emit sig_socketDeconnected(static_cast<QTcpSocket*>(sender()));
}

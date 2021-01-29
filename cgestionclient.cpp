#include "cgestionclient.h"

CGestionClient::CGestionClient(QObject *parent, QTcpSocket *sock) : QObject(parent)
{
    //init
    _sock = sock;
    connect(_sock,&QTcpSocket::readyRead,this,&CGestionClient::on_readyRead);

    _modbus = new CModbusTcp();
    //connect

    //zdc

}

bool CGestionClient:isConnected()
{
    return _sock->state();
}

CGestionClient::~CGestionClient()
{
    if (_sock->isOpen()) {
        _sock->close();
        delete _sock;
    }//if open

    delete _modbus;
}


void CGestionClient::on_readyRead()
{
    QByteArray bA;
//    quint64 lg = _sock->bytesAvailable();//combien de octets reçus
    bA = _sock->readAll();
    qint64 nb = _sock->write("OK");
    if (nb == -1)
       qDebug() << "ERREUR ENVOI";
}

void CGestionClient::on_writeToClients(QByteArray rep)
{
    _sock->write(rep);
}//CGestionClient


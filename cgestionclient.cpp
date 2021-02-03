#include "cgestionclient.h"

CGestionClient::CGestionClient( QTcpSocket *sock)
{
    //init
    _sock = sock;
    connect(_sock,&QTcpSocket::readyRead,this,&CGestionClient::on_readyRead);

    _modbus = new CModbusTcp();

    //connect

    //zdc

}

CGestionClient::~CGestionClient()
{
    if (_sock->isOpen()) {
        _sock->close();
        delete _sock;
    }//if open

    delete _modbus;
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

    sprintf(ch,"Client : %p, %d caractères reçus",static_cast<void*>(_sock),bA.size());
    emit sig_info(ch);
    sprintf(ch,"Requete client : %s",bA.toStdString().c_str());
    emit sig_info(ch);
}

void CGestionClient::on_writeToClients(QByteArray rep)
{
    qint64 nb = _sock->write(rep);
    if (nb == -1){
       qDebug() << "ERREUR ENVOI";
       sig_erreur("ERREUR ENVOI");
    }
}

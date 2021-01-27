#include "cgestionclient.h"

CGestionClient::CGestionClient(QObject *parent, QTcpSocket *client) : QObject(parent)
{
    //init
    _client = client;
    CModbusTcp *_modbus = new CModbusTcp();//delete
    connect(_client,&QTcpSocket::readyRead,this,&CGestionClient::on_readyRead);
}

void CGestionClient::on_readyRead()
{
    QByteArray ba;
    quint64 lg = _client->bytesAvailable();//combien de octets reçus
    ba = _client->readAll();
}

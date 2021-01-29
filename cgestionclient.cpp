#include "cgestionclient.h"

CGestionClient::CGestionClient(QObject *parent, QTcpSocket *client) : QObject(parent)
{
    //init
    _client = client;
    _modbus = new CModbusTcp();//delete
}
CGestionClient::~CGestionClient()
{
    delete _modbus;
}


void CGestionClient::on_readyRead()
{
    QByteArray ba;
    quint64 lg = _client->bytesAvailable();//combien de octets reçus
    ba = _client->readAll();
    emit sig_info(QString(ba));
    int nb = _client->write("OK");
    if (nb == -1)
        qDebug() << "ERREUR ENVOI";
}

void CGestionClient::on_writeToClients(QString req)
{/*
    for (int i=0 ; i<listeClients.size() ; i++) {
        listeClients.at(i)->write(mess.toStdString().c_str());
        qDebug() << "Envoi vers " << listeClients.at(i);
    } // for i
    return 1;*/

    //_client->write(req.toStdString().c_str);
}//CGestionClient


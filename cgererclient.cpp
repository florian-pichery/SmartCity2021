   #include "cgererclient.h"

CGererClient::CGererClient(qintptr sd, QObject *parent) : QObject(parent)
{
    _sd = sd;
}

CGererClient::~CGererClient()
{
    qDebug() << "GCererClient::~CGererClient : destruction !";
    emit sig_info("GCererClient::~CGererClient : destruction !");
    _sock->close();
    delete _sock;
}

void CGererClient::on_goGestionClient()
{
        _sock = new QTcpSocket();  // la socket est maintenant gérée par le thread
        // init des params du client
        if (_sock->setSocketDescriptor(_sd)) {
            _hostAddress = _sock->peerAddress();
            _localAddress = _sock->localAddress();
            _peerPort = _sock->peerPort();
            _localPort = _sock->localPort();
            emit sig_info("CGererClient::on_goGestionClient : Connexion de IP="+_hostAddress.toString()+
                         " Port="+QString::number(_peerPort));
            // signaux de fonctionnement de la socket
            connect(_sock, &QTcpSocket::readyRead, this, &CGererClient::on_readyRead);
            connect(_sock, &QTcpSocket::disconnected, this, &CGererClient::on_disconnected);
            connect(_sock, static_cast<void(QAbstractSocket::*)(QAbstractSocket::SocketError)>(&QAbstractSocket::error),
                  [=](QAbstractSocket::SocketError socketError) {
                    switch(socketError) {
                    case QAbstractSocket::RemoteHostClosedError:
                        emit sig_info("Erreur socket : Remote Host Closed !");
                        break;
                    default:
                        emit sig_info("Erreur socket : Erreur non précisée : "+QString::number(socketError));
                    } // sw
                  });
        } // if setsocket....
}

void CGererClient::on_readyRead()
{
    QTcpSocket *client = static_cast<QTcpSocket *>(sender());
    qint64 nb = client->bytesAvailable();
    QString all=client->readAll();
    //QString all = _sock->readAll();
    emit sig_info("IP Local="+_localAddress.toString()+" Port="+QString::number(_localPort));
    emit sig_info(QString::number(nb)+" car reçus de IP="+_hostAddress.toString()+" Port="+QString::number(_peerPort)+" : "+all);
    on_writeToClients("Bien reçu !");
}

void CGererClient::on_writeToClients(QByteArray rep)
{
    qint64 nb = _sock->write(rep);
    if (nb == -1){
        emit sig_erreur("Erreur d'envoi");
    }
}

void CGererClient::on_disconnected()
{
    emit sig_info("CGererClient::on_disconnected : Déconnexion de IP="+_hostAddress.toString()+" Port="+QString::number(_peerPort));
    emit sig_disconnected();
}

void CGererClient::on_erreur(QString mess)
{
    emit sig_erreur(mess);
}

void CGererClient::on_info(QString mess)
{
    emit sig_info(mess);
}


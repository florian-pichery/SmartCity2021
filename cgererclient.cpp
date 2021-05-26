#include "cgererclient.h"
#include "cgererclient.h"

CGererClient::CGererClient(qintptr sd, QObject *parent) : QObject(parent)
{
    _sd = sd;
    _zdc = new CZdc;
}

CGererClient::~CGererClient()
{
    delete _zdc;
    qDebug() << "GCererClient::~CGererClient : destruction !";
    on_info("GCererClient::~CGererClient : destruction !");
    _sock->close();
    delete _modbus;
    delete _sock;
}


void CGererClient::on_goGestionClient()
{
    _modbus = new CModbusTcp;
    connect(_modbus, &CModbusTcp::sig_info, this, &CGererClient::on_info);
    connect(_modbus, &CModbusTcp::sig_erreur, this, &CGererClient::on_erreur);

    _sock = new QTcpSocket();  // la socket est gérée par le thread

    // init des params du client et stockage dans variable commune
    if (_sock->setSocketDescriptor(_sd)) {
        _hostAddress = _sock->peerAddress();//addr IP du client
        _localAddress = _sock->localAddress();//addr IP du serveur
        _peerPort = _sock->peerPort();//port du client
        _localPort = _sock->localPort();//port du serveur
        on_info("CGererClient::on_goGestionClient : Connexion de IP="+_hostAddress.toString()
                +" Port="+QString::number(_peerPort)); //affichage dans l'Ihm

        // signaux de fonctionnement de la socket
        connect(_sock, &QTcpSocket::readyRead, this, &CGererClient::on_readyRead);
        connect(_sock, &QTcpSocket::disconnected, this, &CGererClient::on_disconnected);

        connect(_sock, static_cast<void(QAbstractSocket::*)(QAbstractSocket::SocketError)>(&QAbstractSocket::error),
                [=](QAbstractSocket::SocketError socketError) {
            switch(socketError) {
            case QAbstractSocket::RemoteHostClosedError:
                on_erreur("Erreur socket : Remote Host Closed !");
                break;
            default:
                on_erreur("Erreur socket : Erreur non précisée : "+QString::number(socketError));
            } // sw
        });// cette longue commande gère les erreurs de manière intelligente et
        // l'affiche ainsi que son code d'erreur sauf si c'est une déconnection

    } // if setsocket....
}

void CGererClient::on_readyRead()
{
    QTcpSocket *client = static_cast<QTcpSocket *>(sender()); //on détermine qui envoie le message
    qint64 nb = client->bytesAvailable();// aquisition du nombre d'octets reçus
    QByteArray trameClient=client->readAll();// lecture de la trame qui nous est retourné en ASCII
    //on_info("IP Local="+_localAddress.toString()+" Port="+QString::number(_localPort));//affichage le l'ip et le port du serveur
    emit sig_info(QString::number(nb)+" caractères reçus de IP="+_hostAddress.toString()+" Port="+QString::number(_peerPort)+" :" );
    //affichage du nombre de d'octets reçus, de l'IP, du port de la source, et le contenu du message
    emit sig_info("trame reçue -> " + trameClient);
    //on_writeToClients("Bien reçu !\n"); // ACK (acquittement : informer à l'émmeteur de la bonne reception du message)
    int commande = _modbus->on_trameClient(trameClient);//retourne -1 si mal passé, 0 si trame non complète, 1 si c'est bon

    switch(commande){

    case -1:    //si mal passé
        emit sig_erreur("CGererClient::on_readyRead : Erreur dans le format de la trame, requette supprimée");
        //on_writeToClients("format de requette incorrecte, requete supprimée");
        _modbus->deleteTc();
        break;

    case 0:     //si trame non complète
        part += 1;
        emit sig_erreur("Trame incomplète, attente de la "+QByteArray::number(part)+"ème partie de la requette.");
        //on_writeToClients("attente de la "+QByteArray::number(part)+"ème partie de la requette");
        break;

    case 1:     //trame complète
        part = 1;
        emit sig_info("Trame complète.");
        bool verifier =_modbus->verifier();
        if (!verifier){
            emit sig_erreur("CGererClient::on_readyRead : erreur verification");
            //on_writeToClients("Vérification de la trame échouée");
            _modbus->deleteTc();
            break;
        }//if(!verifier)
        //si le MBAP header + CRC sont bon
        emit sig_info("Trame verifiée.");
        int ordre = _modbus->decoder();
        QByteArray reponse;
        QByteArray valeursMots;
        switch (_modbus->get_functionCode()) {
        case 1://Lecture
            valeursMots = read(ordre);//retourne un QbyteArray de la partie data d'un réponse à une lecture
            reponse = _modbus->reponseLecture(valeursMots);//retourne la trame à envoyer au client
            break;
        case 2://Ecriture
            bool exec = write(ordre);//retourne si ça s'est bien executé
            reponse = _modbus->reponseEcriture(exec);//retourne la trame à envoyer au client
            break;
        }
        on_writeToClients(reponse);
        _modbus->deleteTc();
        emit sig_info("Réponse envoyée.");
        break;

    }//sw
}

QByteArray CGererClient::read(int ordre)
{
    QByteArray data = "";
    uintmax_t dataInt = 0;
    uint value[8];
    uint bit[8];
    uint8_t Addr1wordInt =_modbus->get_Addr1WordInt();
    uint8_t nbrOfWordsInt = _modbus->get_nbrOfWords();
    uint8_t nbrEclair;
    QByteArray dataCalcArray ="";
    QString dataCalcString;
    switch (ordre) {

    case 3://Parking
    {
        value[0] = _zdc->getEtatsBarrieres();
        bit[0] = value[0]%2;
        for (int i=0; i!=7; i++) {
            value[i+1] = (value[i]-bit[i])/2;
            bit[i+1] = value[i+1]%2;
        }
        if (bit[0] == 1) dataInt+=2;
        if (bit[1] == 1) dataInt+=1;
        if (bit[2] == 1) dataInt+=8;
        if (bit[3] == 1) dataInt+=4;
        if (bit[4] == 1) dataInt+=16;
        if (bit[7] == 1) dataInt+=32;

        dataCalcString = QString::number( dataInt, 16 ).toUpper();
        dataCalcArray = dataCalcString.toLatin1();
        if (dataCalcArray.size() == 1) data += "000";
        if (dataCalcArray.size() == 2) data += "00";
        if (dataCalcArray.size() == 3) data += "0";
        data += dataCalcArray;
    }
        break;
    case 4://RFID
    {
        QByteArray rfidE = "ff23430154";//_zdc->getRfidE();
        QByteArray rfidS = "AC34549001";//_zdc->getRfidS();
        data += "00";
        data+= rfidE.toUpper();
        data += "00";
        data+= rfidS.toUpper();
    }
        break;
    case 6://éclairage
    {
        Addr1wordInt = Addr1wordInt-32;
        nbrEclair = 32;//_zdc->getNbEclairage();
        for (uint8_t i=0; i<nbrOfWordsInt && nbrEclair > i && nbrEclair >= Addr1wordInt+1 + i ;i++) {
            if (!_zdc->getPresence(i+Addr1wordInt)) dataInt+=1;
            if (!_zdc->getCellule(i+Addr1wordInt)) dataInt+=2;

            value[0] = _zdc->getLampFonct(i+Addr1wordInt);
            bit[0] = value[0]%2;
            for (int i=0; i!=7; i++) {
                value[i+1] = (value[i]-bit[i])/2;
                bit[i+1] = value[i+1]%2;
            }

            if (bit[2] == 1) dataInt+=4;
            if (bit[3] == 1) dataInt+=8;
            if (bit[4] == 1) dataInt+=16;
            if (bit[5] == 1) dataInt+=32;
            if (bit[6] == 1) dataInt+=64;
            if (bit[7] == 1) dataInt+=128;

            dataCalcString = QString::number( dataInt, 16 ).toUpper();
            dataCalcArray = dataCalcString.toLatin1();
            if (dataCalcArray.size() == 1) data += "000";
            if (dataCalcArray.size() == 2) data += "00";
            if (dataCalcArray.size() == 3) data += "0";
            data += dataCalcArray;
            dataInt = 0;
        }
    }
        break;
    case 8://Intersection
    {
        value[0] = _zdc->getBoutonPietonVoie1();
        bit[0] = value[0]%2;
        for (int i=0; i!=7; i++) {
            value[i+1] = (value[i]-bit[i])/2;
            bit[i+1] = value[i+1]%2;
        }
        if ( bit[2] == 0 && bit[1] == 0 && bit[0] == 1 ) dataInt+=1;
        if ( bit[2] == 0 && bit[1] == 1 && bit[0] == 0 ) dataInt+=2;
        if ( bit[2] == 0 && bit[1] == 1 && bit[0] == 1 ) dataInt+=4;
        if ( bit[2] == 1 && bit[1] == 0 && bit[0] == 0 ) dataInt+=8;

        value[0] = _zdc->getBoutonPietonVoie2();
        bit[0] = value[0]%2;
        for (int i=0; i!=7; i++) {
            value[i+1] = (value[i]-bit[i])/2;
            bit[i+1] = value[i+1]%2;
        }
        if ( bit[2] == 0 && bit[1] == 0 && bit[0] == 1 ) dataInt+=16;
        if ( bit[2] == 0 && bit[1] == 1 && bit[0] == 0 ) dataInt+=32;
        if ( bit[2] == 0 && bit[1] == 1 && bit[0] == 1 ) dataInt+=64;
        if ( bit[2] == 1 && bit[1] == 0 && bit[0] == 0 ) dataInt+=128;

        dataCalcString = QString::number( dataInt, 16 ).toUpper();
        dataCalcArray = dataCalcString.toLatin1();
        if (dataCalcArray.size() == 1) data += "000";
        if (dataCalcArray.size() == 2) data += "00";
        if (dataCalcArray.size() == 3) data += "0";
        data += dataCalcArray;
    }
        break;
    default :
        on_erreur("CGererClient::read : erreur de décodage");
        break;
    }

    return data;
}

bool CGererClient::write(int ordre)
{
    bool REturn = true;
    uint8_t Addr1wordInt;
    QByteArray tc = _modbus->get_tc();
    QByteArray caracterTaked;
    QString ligne;
    uint8_t nbrEclair;
    uint value[8];
    bool bit[8];

    if (ordre == 2 || ordre == 7){
        QByteArray valueWordToForce = tc;
        QString stringByteArray = QString::fromStdString(valueWordToForce.data());
        value[0] = stringByteArray.toUInt(nullptr,16);
        //permet de décomposer la partie valeur donc dans l'exemple = "00B2" en valeur entière = 178(10)
        bit[0] = value[0]%2;
        for (int i=0; i!=7; i++) {
            value[i+1] = (value[i]-bit[i])/2;
            bit[i+1] = value[i+1]%2;
        }// permet de transformer cette valeur en bits grâce a des divisions euclidiènnes
    }

    switch(ordre){

    case 1://EcranS
        if(tc.size() == 32){
            ligne = QString(tc.left(16));
            _zdc->setLigneSup(ligne);
            ligne = QString(tc.right(16));
            _zdc->setLigneInf(ligne);
        }else REturn = false;
        break;

    case 2://Parking
        if (bit[0] == 1 && bit[1] == 0) _zdc->setOrdreBarrieres(128+1);//montée barriere entrée
        if (bit[1] == 1 && bit[0] == 0) _zdc->setOrdreBarrieres(128+2);//descente barriere entrée
        if (bit[2] == 1 && bit[3] == 0) _zdc->setOrdreBarrieres(128+4);//montée barriere sortie
        if (bit[3] == 1 && bit[2] == 0) _zdc->setOrdreBarrieres(128+8);//descente barriere sortie
        if ( (bit[3] == 1 && bit[2] == 1) || (bit[3] == 1 && bit[2] == 1) ) REturn = false;
        break;

    case 5://éclairage
        REturn = 0;
        nbrEclair = _zdc->getNbEclairage();
        Addr1wordInt = _modbus->get_Addr1WordInt();
        // 1ère addr =0, 32eme addr = 31
        for (uint8_t i=0; (tc.size()/2)>i && nbrEclair > i && nbrEclair >= Addr1wordInt+1 + i ;i++) {
            int caracterTakedInt;
            caracterTaked = tc.left(2);
            tc = tc.right(tc.size()-2);
            caracterTakedInt = static_cast<int>(_modbus->valueOf(caracterTaked));
            switch (caracterTakedInt) {
            case 0:
            case 2://0%
                _zdc->setConsigneEclair(i+Addr1wordInt,128);
                break;
            case 1://50%
                _zdc->setConsigneEclair(i+Addr1wordInt,128+1);
                break;
            case 3://100%
                _zdc->setConsigneEclair(i+Addr1wordInt,128+2);
                break;
            default:
                REturn = false;
                break;
            }
            REturn = 1;
        }
        break;

    case 7://Intersection
        if (bit[0] == 0 && bit[1] == 0) _zdc->setModeVoies(128);//orange clignotant
        if (bit[0] == 1 && bit[1] == 0) _zdc->setModeVoies(128+1);//auto
        if (bit[0] == 0 && bit[1] == 1) {//manuel
            _zdc->setModeVoies(128+2);
            if (bit[6] == 1){
                //voie 1
                if (bit[2] == 0 && bit[3] == 0) _zdc->setOrdresFeu1(128);//eteint
                if (bit[2] == 1 && bit[3] == 0) _zdc->setOrdresFeu1(128+1);//vert
                if (bit[2] == 0 && bit[3] == 1) _zdc->setOrdresFeu1(128+2);//orange
                if (bit[2] == 1 && bit[3] == 1) _zdc->setOrdresFeu1(128+3);//rouge
            }
            if (bit[7] == 1){
                //voie 2
                if (bit[4] == 0 && bit[5] == 0) _zdc->setOrdresFeu2(128);//eteint
                if (bit[4] == 1 && bit[5] == 0) _zdc->setOrdresFeu2(128+1);//vert
                if (bit[4] == 0 && bit[5] == 1) _zdc->setOrdresFeu2(128+2);//orange
                if (bit[4] == 1 && bit[5] == 1) _zdc->setOrdresFeu2(128+3);//rouge
            }

        }
        break;

    case 9://authentification
        emit sig_info("Requette d'authentification.");
        REturn = _modbus->verificationMdp();
        break;

    default :
        on_erreur("CGererClient::write : Erreur de décodage");
        REturn = false;
        break;
    }
    return REturn;
}

void CGererClient::on_writeToClients(QByteArray rep)
{
    qDebug() <<"envoie message ->" << rep;
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


#include "cmodbustcp.h"
#include "cmodbustcp.h"
#include "cmodbustcp.h"

CModbusTcp::CModbusTcp(QObject *parent) : QObject(parent)
{
    /*
    QByteArray crc16array = _tc.right(4);
    _tc = _tc.left(_tc.size()-4);
    QString stringCrc16 = QString::fromStdString(crc16array.data());
    crc16 = stringCrc16.toUInt(nullptr,16);
*/
}

CModbusTcp::~CModbusTcp()
{

}

void CModbusTcp::deleteTc()
{
    _mode = 0;
    _tc.clear();
}

int CModbusTcp::get_functionCode()
{
    switch (_fonction) {
    case 3:
    case 4:
        return 1;
    case 16:
        return 2;
    }
    return 0;
}

QByteArray CModbusTcp::get_tc()
{
    return _tc;
}

uint8_t CModbusTcp::get_Addr1WordInt()
{
    return static_cast<uint8_t>(_Addr1WordInt);
}

uint8_t CModbusTcp::get_nbrOfWords()
{
    return static_cast<uint8_t>(valueOf(_nbrOfWord));
}

int CModbusTcp::on_trameClient(QByteArray trameClient)
{
    //methode qui permet de vérifier que la trame est au format :data:
    _tc += trameClient;//on met la trame dans une variable commune

    //test et purge début trame
    int deb = _tc.indexOf(":",0); // recherche début trame
    if (deb == -1) { // si pas de car de début
        return -1;
    } // if pas de car de debut
    int u [_tc.size()];
    int fin = -1;
    // test et purge fin de trame
    for (int i = 0; i != _tc.size(); i++) {
        u[i] = _tc.indexOf(":", i);
        if (u[i] > u[i-1])//Sert pour prendre le dernier caractère ':' de la trame
            fin  = u[i];
    }

    if (fin == -1) return 0; // si il n'y a pas de caractère ':' on attend la suite
    _tc.remove(fin, _tc.size()-fin);  // au cas ou, on enlève tout après la fin
    _tc.remove(0, deb+1);  // on enlève tout avant le : au cas ou
    return 1;
}

bool CModbusTcp::verifier()
{
    //trame qui permet de vérifier que la trame respecte le protocole modbus établi

    //on récupere le Crc de la trame
    QByteArray crc16array = _tc.right(4);//on prend les 4dernier caractère de la trame
    QString stringCrc16 = QString::fromStdString(crc16array.data());
    uint crc16 = stringCrc16.toUInt(nullptr,16);//convertion en unsigned int
    _tc = _tc.left(_tc.size()-4);//on enleve le Crc de la trame
    uint16_t crc16Calc = calculCrc16(_tc);//on calcule le crc de la trame
    if (crc16 != crc16Calc){
        emit sig_erreur("CRC16 mauvais, crc recupéré = "+QString::number(crc16)
                              +" crc calculé = "+QString::number(crc16Calc));
        return 0;
    }//test CRC

    QByteArray transIdent = takeCharacter(4);
    uint Uint_transIdent = valueOf(transIdent);
    if (Uint_transIdent != 1) {
        emit sig_erreur("transaction identifier mauvais");
        return 0;
    }//test transaction IDENT

    QByteArray protIdent = takeCharacter(4);
    uint Uint_protIdent = valueOf(protIdent);
    if (Uint_protIdent != 0) {
        emit sig_erreur("Protocol identifier mauvais : "+protIdent);
        return 0;
    }//test Protocol identifier

    QByteArray lenght = takeCharacter(4);
    uint Uint_lenght = valueOf(lenght);
    if (static_cast<int>(Uint_lenght) != _tc.size()+2+4+4+4+4) {
        emit sig_erreur("La taille recupérée est mauvaise, taille récupérée = "+QString::number(Uint_lenght)
                        +", taille calculée = "+QString::number(_tc.size()+2+4+4+4+4));
        return 0;
    }//test taille

    QByteArray UnitIdent = takeCharacter(1);
    _mode = 0;
    if (UnitIdent == "P") _mode = 1;
    if (UnitIdent == "E") _mode = 2;
    if (UnitIdent == "I") _mode = 3;
    if (UnitIdent == "A") _mode = 4;
    if (_mode == 0){ //on les compares
        emit sig_erreur("erreur unit identifier");
        return 0;
    }//test mode

    QByteArray functionCode = takeCharacter(2);
    _fonction = static_cast<int>(valueOf(functionCode));
    if ( _fonction != 16 && _fonction != 03 && _fonction != 04){
        emit sig_erreur("fonction mauvaise");
        return 0;
    }//test fonction

    return 1;
}

int CModbusTcp::decoder()
{
    //décodage de "_tc" pour traduire un ordre
    //cet odre sera affilié a un nombre et retourné par la fonction, dans on ready read

    //return 1 //Ecriture ecran
    //return 2 //Ecriture parking
    //return 3 //Lecture parking
    //return 4 //Lecture RFID
    //return 5;//Ecriture éclairage
    //return 6;//Lecture éclairage
    //return 7 //Ecriture inter
    //return 8 //Lecture inter
    //return 9;//Ecriture authentification
    switch (_mode)
    {
    case 1 ://si parking
        return decodeParking();

    case 2 ://si Eclairage
        return decodeEclairage();

    case 3 ://si intersection
        return decodeIntersection();

    case 4 ://si Authentification
        return decodeAuthentification();

    }//switch
    return 0;
}

int CModbusTcp::decodeParking()
{
    _Addr1Word = takeCharacter(4);
    uint Uint_Addr1Word = valueOf(_Addr1Word);
    _nbrOfWord = takeCharacter(4);
    QByteArray nbrOfBytes;
    switch (_fonction) {
    case 16:
        nbrOfBytes = takeCharacter(2);

        switch(Uint_Addr1Word){
        case 128:
            if (_nbrOfWord != "0010"){
                emit sig_erreur("erreur nombre de mots");
                return false;
            }
            if (nbrOfBytes != "20"){
                emit sig_erreur("erreur nombre d'octets");
                return false;
            }//if
            return 1;//Ecriture ecran

        case 160:
            if (_nbrOfWord != "0001"){
                emit sig_erreur("erreur nombre de mots");
                return false;
            }
            if (nbrOfBytes != "02"){
                emit sig_erreur("erreur nombre d'octets");
                return false;
            }//if
            return 2;//Ecriture parking

        default:
            emit sig_erreur("erreur 1er mot"+ QString::number(Uint_Addr1Word));
            return false;
        }

    case 03:
        [[clang::fallthrough]];
    case 04:
        switch(Uint_Addr1Word){
        case 161:

            if (_nbrOfWord != "0001"){
                emit sig_erreur("erreur nombre de mots");
                return false;
            }
            if (_tc != ""){
                emit sig_erreur("erreur caractères restants");
                return false;
            }
            return 3;//Lecture parking

        case 162:
            if (_nbrOfWord != "0006"){
                emit sig_erreur("erreur nombre de mots");
                return false;
            }
            if (_tc != ""){
                emit sig_erreur("erreur caractères restants");
                return false;
            }
            return 4;//Lecture RFID

        default:
            emit sig_erreur("erreur 1er mot "+QString::number(Uint_Addr1Word));
            return false;
        }
    }
    return false;
}

int CModbusTcp::decodeEclairage()
{
    _Addr1Word = takeCharacter(4);
    _Addr1WordInt = static_cast<int>(valueOf(_Addr1Word));
    _nbrOfWord = takeCharacter(4);
    int nbrOfWordInt = static_cast<int>(valueOf(_nbrOfWord));
    QByteArray nbrOfBytes;
    int nbrOfBytesInt;
    //a modifier
    switch (_fonction) {
    case 16:

        if (_Addr1WordInt >= 32){
            emit sig_erreur("erreur 1er mot");
            return false;
        }//if
        if (nbrOfWordInt+_Addr1WordInt > 32){
            emit sig_erreur("erreur nombre de mots");
            return false;
        }//if
        nbrOfBytesInt = static_cast<int>(valueOf(takeCharacter(2)));
        if (nbrOfBytesInt != nbrOfWordInt*2 || _tc.size()/2 != nbrOfBytesInt){
            emit sig_erreur("erreur nombre de d'octets");
            return false;
        }//if
        return 5;//écriture éclairage

    case 03:
        [[clang::fallthrough]];
    case 04:
        if ( 31 > _Addr1WordInt && _Addr1WordInt > 64){
            emit sig_erreur("erreur 1er mot");
            return false;
        }//if
        if (nbrOfWordInt+_Addr1WordInt-32 > 32){
            emit sig_erreur("erreur nombre de mots");
            return false;
        }//if
        if (_tc != ""){
            emit sig_erreur("erreur caractères restants");
            return false;
        }
        return 6;//lecture éclairage
    }
    return false;
}

int CModbusTcp::decodeIntersection()
{
    _Addr1Word = takeCharacter(4);//recupere l'adresse du premier mot
    _nbrOfWord = takeCharacter(4);//recupere le nombres de mots
    QByteArray nbrOfBytes;

    switch (_fonction) {
    case 16:

        if (_Addr1Word != "00AA"){
            emit sig_erreur("erreur 1er mot");
            return false;
        }//if
        if (_nbrOfWord != "0001"){
            emit sig_erreur("erreur nombre de mots");
            return false;
        }//if
        nbrOfBytes = takeCharacter(2);//recupere le nombre d'octets
        if (nbrOfBytes != "02"){
            emit sig_erreur("erreur nombre d'octets");
            return false;
        }//if
        return 7;//ecriture inter

    case 03:
        [[clang::fallthrough]];
    case 04:
        if (_Addr1Word != "00AB"){
            emit sig_erreur("erreur 1er mot");
            return false;
        }//if
        if (_nbrOfWord != "0001"){
            emit sig_erreur("erreur nombre de mots");
            return false;
        }//if
        if (_tc != ""){
            emit sig_erreur("erreur caractères restants");
            return false;
        }
        return 8;//lecture inter

    }
    return false;
}

int CModbusTcp::decodeAuthentification()
{
    if(_fonction != 16){//fonction16
        emit sig_erreur("erreur fonction");
        return 0;
    }//if
    _Addr1Word = takeCharacter(4);
    if (_Addr1Word != "00C0"){
        emit sig_erreur("erreur 1er mot");
        return 0;
    }//if
    _nbrOfWord = takeCharacter(4);
    if (_nbrOfWord != "0020"){
        emit sig_erreur("erreur nombre de mots");
        return 0;
    }//if
    QByteArray nbrOfBytes = takeCharacter(2);
    if (nbrOfBytes != "40"){
        emit sig_erreur("erreur nombre d'octets");
        return 0;
    }//if
    return 9;//Ecriture authentification
}

QByteArray CModbusTcp::reponseEcriture(bool exec)
{
    _reponse ="";
    _reponse += ":";
    //MBAP header
    QByteArray data = "00010000001D";
    switch (_mode) {//Unit identifier
    case 1://si parking
        data += "P";
        break;
    case 2://si eclairage
        data += "E";
        break;
    case 3://si intersection
        data += "I";
        break;
    case 4://si authentification
        data += "A";
        break;
    }
    //fonction code
    data += "10";
    //data
    data += _Addr1Word;//adresse du premier mot forcé
    if (exec) data += _nbrOfWord;//nombre de mots forcés si tout s'est bien passé
    else data += "0000";//sinon rien
    uint16_t crc16Calc = calculCrc16(data);//calcul du CRC16
    QString crcCalcString = QString::number( crc16Calc, 16 ).toUpper();//conversion hexadécimale
    QByteArray crcCalcArray = crcCalcString.toLatin1();//convertion QByteArray

    data += crcCalcArray;//ajout du CRC16 dans la trame

    _reponse += data;
    _reponse += ":";//encapsulation dans les '::'
    return _reponse;
}

QByteArray CModbusTcp::reponseLecture(QByteArray val)
{
    _reponse ="";
    _reponse += ":";
    QByteArray data = "00010000";

    uint16_t lenght = 2+4+4+4+1+2+2+val.size()+4;//8+4+1+2+2+4+4+2
    QString lenghtString = QString::number( lenght, 16 ).toUpper();
    QByteArray lenghtArray = lenghtString.toLatin1();
    if (lenghtArray.size() == 2)data+= "00";
    if (lenghtArray.size() == 3)data+= "0";
    data+=lenghtArray;

    switch (_mode) {//Unit identifier
    case 1://si parking
        data += "P";
        break;
    case 2://si eclairage
        data += "E";
        break;
    case 3://si intersection
        data += "I";
        break;
    case 4://si authentification
        data += "A";
        break;
    }
    data += "03";
    uint16_t nbrOfByte= static_cast<uint16_t>(valueOf(_nbrOfWord)*2);
    QString nbrOfByteString = QString::number( nbrOfByte, 16 ).toUpper();
    QByteArray nbrOfByteArray = nbrOfByteString.toLatin1();
    if (nbrOfByteArray.size() == 1) data+="0";
    data +=nbrOfByteArray;
    data += val;

    uint16_t crc16Calc = calculCrc16(data);
    QString crcCalcString = QString::number( crc16Calc, 16 ).toUpper();
    QByteArray crcCalcArray = crcCalcString.toLatin1();
    if (crcCalcArray.size() == 1) data += "000";
    if (crcCalcArray.size() == 2) data += "00";
    if (crcCalcArray.size() == 3) data += "0";
    data += crcCalcArray;

    _reponse += data;
    _reponse +=":";
    return _reponse;
}

bool CModbusTcp::verificationMdp()
{
    QByteArray PREidentifiant = takeCharacter(32);
    QByteArray identifiant;
    QByteArray host = "";//_bdd->get_idClient().toLatin1();//getter sur la base de donnée
    if (host == "") host = "root";
    for (int i = 1;identifiant != host || PREidentifiant.left(1) != " " ; i++) {
        if (i == 32){
            return false;
        }
        identifiant += PREidentifiant.left(1);
        PREidentifiant = PREidentifiant.right(PREidentifiant.size()-1);
    }

    QByteArray PREmotDePasse = takeCharacter(32);
    QByteArray motDePasse;
    QByteArray mdp = "";//_bdd->get_mdpClient().toLatin1();//getter sur la base de donnée
    if (mdp == "") mdp = "admin";
    for (int i = 1;motDePasse!= mdp || PREmotDePasse.left(1) != " " ; i++) {
        if (i == 32){
            return false;
        }
        motDePasse += PREmotDePasse.left(1);
        PREmotDePasse = PREmotDePasse.right(PREmotDePasse.size()-1);
    }
    return true;
}

QByteArray CModbusTcp::takeCharacter(int nbOfBytes)
{
    //méthode qui prend un nombre de caractère donné de la trame client
    QByteArray byteArrayTaked = _tc.left(nbOfBytes); //on récupère les nbrOfBytes premiers caractère de la variable commune _tc
    _tc = _tc.right(_tc.size()-nbOfBytes); //on enleve ces caractères a la variable commune
    return byteArrayTaked;//retourne les nbOfBytes premiers caractères de _tc
}

uint CModbusTcp::valueOf(QByteArray ByteArray)
{
    //methode qui retourne la valeur en décimale dans un unsigned int d'un QByteArray en Hexadécimal
    QString stringByteArray = QString::fromStdString(ByteArray.data());
    uint uintByteArray = stringByteArray.toUInt(nullptr,16);
    return uintByteArray;
}

uint16_t CModbusTcp::calculCrc16(QByteArray ByteArray)
{
    //methode qui permet de calculer le CRC16 modbus d'un QByteArray
    uint8_t nbDec;
    uint8_t yaun;
    uint8_t indice;
    uint16_t crc;

    crc = 0xFFFF;
    indice = 0;

    do {
        crc ^= ByteArray[indice];
        nbDec = 0;

        do {
            if ( (crc&0x0001) == 1)
                yaun = 1;
            else
                yaun = 0;
            crc >>= 1;
            if (yaun == 1)
                crc ^= 0xA001;
            nbDec++;
        } while (nbDec < 8);
        indice++;
    } while (indice < (ByteArray.size()-3));

    return crc;
}

void CModbusTcp::on_erreur(QString mess)
{
    emit sig_erreur(mess);
}

void CModbusTcp::on_info(QString mess)
{
    emit sig_info(mess);
}

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

int CModbusTcp::on_trameClient(QByteArray trameClient)
{
    //les trames au complet contiendrons :data-crc16:
    _tc += trameClient;

    //test et purge début trame
    int deb = _tc.indexOf(":",0); // recherche début trame
    if (deb == -1) { // si pas de car de début
        return -1;
    } // if pas de car de debut

    // test et purge fin de trame
    int fin = _tc.indexOf(":",1);
    if (fin == -1) return 0; // on attend la suite
    _tc.remove(fin, _tc.size()-fin);  // au cas ou, on enlève tout après la fin
    _tc.remove(0, deb+1);  // on enlève tout avant le : au cas ou

    //emit sig_info("voici la trame au complet : "+_tc);
    return 1;
}

bool CModbusTcp::verifier()
{
    QByteArray crc16array = _tc.right(4);
    QString stringCrc16 = QString::fromStdString(crc16array.data());
    uint crc16 = stringCrc16.toUInt(nullptr,16);
    //on récupere le Crc de la trame
    _tc = _tc.left(_tc.size()-4);//on enleve le Crc de la trame
    uint16_t crc16Calc = calculCrc16(_tc);//on calcule le crc de la trame
    //qDebug () << "taille trame = " << _tc.size() << ", trame : "<< _tc ;
    if (crc16 != crc16Calc){
        qDebug() << "CRC16 mauvais";
        qDebug () << "crc = " << crc16 << " crc calculé = "<< crc16Calc ;
        return 0;
    }//test CRC

    QByteArray transIdent = takeCharacter(4);
    uint Uint_transIdent = valueOf(transIdent);
    if (Uint_transIdent != 1) {
        qDebug() << "Transaction Identifier mauvais";
        return 0;
    }//test transaction IDENT

    QByteArray protIdent = takeCharacter(4);
    uint Uint_protIdent = valueOf(protIdent);
    if (Uint_protIdent != 0) {
        qDebug() << "Protocol identifier mauvais";
        return 0;
    }//test Protocol identifier

    QByteArray lenght = takeCharacter(4);
    uint Uint_lenght = valueOf(lenght);
    if (static_cast<int>(Uint_lenght) != _tc.size()+2+4+4+4+4) {
        qDebug() << "taille mauvaise";
        qDebug() << Uint_lenght << " taille" << _tc.size()+2+4+4+4+4 ;
        return 0;
    }//test taille

    QByteArray UnitIdent = takeCharacter(1);
    _mode = 0;
    if (UnitIdent == "P") _mode = 1;
    if (UnitIdent == "E") _mode = 2;
    if (UnitIdent == "I") _mode = 3;
    if (UnitIdent == "A") _mode = 4;
    if (_mode == 0){ //on les compares
        qDebug() << "mode mauvais";
        return 0;
    }//test mode

    QByteArray functionCode = takeCharacter(2);
    _fonction = static_cast<int>(valueOf(functionCode));
    if ( _fonction != 16 && _fonction != 03 && _fonction != 04){
        qDebug() << "fonction mauvais";
        return 0;
    }//test fonction

    return 1;
}

int CModbusTcp::decoder()
{
    //décodage de "_tc" pour traduire un ordre
    //cet odre sera afilié a un nombre et retourné par la fonction, dans on ready read
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
    return -1;
}

int CModbusTcp::decodeParking()
{

}

int CModbusTcp::decodeIntersection()
{
    switch (_fonction) {
    case 16:
        _Addr1Word = takeCharacter(4);
        if (_Addr1Word != "0000"){
            qDebug() << "erreur 1er mot";
            return false;
        }//if
        return true;
    case 03:
        [[clang::fallthrough]];
    case 04:
        _Addr1Word = takeCharacter(4);
        uint Uint_Addr1Word = valueOf(_Addr1Word);
        if (Uint_Addr1Word < 64 || Uint_Addr1Word > 128){
            qDebug() << "erreur 1er mot";
            return 0;
        }//if
        _nbrOfWord = takeCharacter(4);
        uint Uint_nbrOfWord = valueOf(_nbrOfWord);
        if (Uint_nbrOfWord==0 || Uint_nbrOfWord >= 129-Uint_Addr1Word){
            qDebug() << "erreur nombre de mots";
            return 0;
        }//if
        return 2;//lecture intersection

    }
    return false;
}

int CModbusTcp::decodeEclairage()
{

}

int CModbusTcp::decodeAuthentification()
{
    if(_fonction != 16){//fonction16
        qDebug() << "erreur fonction";
        return 0;
    }//if
    _Addr1Word = takeCharacter(4);
    if (_Addr1Word != "00C0"){
        qDebug() << "erreur 1er mot";
        return 0;
    }//if
    _nbrOfWord = takeCharacter(4);
    if (_nbrOfWord != "0020"){
        qDebug() << "erreur nombre de mots";
        return 0;
    }//if
    QByteArray nbrOfBytes = takeCharacter(2);
    if (nbrOfBytes != "40"){
        qDebug() << "erreur nombre d'octets";
        return 0;
    }//if

    //requette d'authentification répérée
    return 1;//authentification
}


QByteArray CModbusTcp::reponseEcriture(bool exec)
{
    _reponse ="";
    _reponse += ":";
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

    bool si03 = false;
    switch (_fonction) {//fonction

    case 3:
        data += "03";
        si03 = true;
        [[clang::fallthrough]];
    case 4:
        if (!si03) data += "04";
        break;

    case 16:
        data += "10";
        data += _Addr1Word;
        if (exec) data += _nbrOfWord;
        else data += "0000";
        uint crc16Calc = calculCrc16(data);
        QString crcCalcString = QString::number( crc16Calc, 16 ).toUpper();
        QByteArray crcCalcArray = crcCalcString.toLatin1();
        //qDebug () << "crc16Calc : " << crc16Calc;qDebug () << "crc16Hex : " <<crcCalcString;qDebug () << "taille trame = " << crcCalcArray.size() << ", trame : "<< crcCalcArray ;
        data += crcCalcArray;
        break;

    }

    _reponse += data;
    _reponse += ":";
    qDebug() << _reponse;
    return _reponse;
}

QByteArray CModbusTcp::reponseLecture(QByteArray val)
{

    _reponse ="";
    if (val=="")return _reponse;
    _reponse += ":";
    QByteArray data = "00010000";


        return _reponse;
}

bool CModbusTcp::verificationMdp()
{
    qDebug() << _tc;
    QByteArray PREidentifiant = takeCharacter(32);
    QByteArray identifiant;
    QByteArray host = "root";//getter sur la base de donnée
    qDebug() << identifiant << "pRE : " << PREidentifiant << " host:" <<host;
    for (int i = 1;identifiant != host || PREidentifiant.left(1) != " " ; i++) {
        if (i == 32){
            return false;
        }

        identifiant += PREidentifiant.left(1);
        PREidentifiant = PREidentifiant.right(PREidentifiant.size()-1);
        qDebug() << identifiant << "pRE : " << PREidentifiant;
    }
    QByteArray PREmotDePasse = takeCharacter(32);
    QByteArray motDePasse;
    QByteArray mdp = "admin";//getter sur la base de donnée

    qDebug() << motDePasse << "pRE : " << PREmotDePasse;
    for (int i = 1;motDePasse!= mdp || PREmotDePasse.left(1) != " " ; i++) {
        if (i == 32){
            return false;
        }
        motDePasse += PREmotDePasse.left(1);
        PREmotDePasse = PREmotDePasse.right(PREmotDePasse.size()-1);
        qDebug() << motDePasse << "pRE : " << PREmotDePasse;
    }

    return true;
}

QByteArray CModbusTcp::takeCharacter(int nbOfBytes)
{
    QByteArray byteArrayTaked = _tc.left(nbOfBytes);
    _tc = _tc.right(_tc.size()-nbOfBytes);
    return byteArrayTaked;//retourne les nbOfBytes premiers caractères de _tc
}

uint CModbusTcp::valueOf(QByteArray ByteArray)
{
    QString stringByteArray = QString::fromStdString(ByteArray.data());
    uint uintByteArray = stringByteArray.toUInt(nullptr,16);
    return uintByteArray;
}

uint16_t CModbusTcp::calculCrc16(QByteArray ByteArray)// bon
{
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
    emit sig_erreur(mess);
}

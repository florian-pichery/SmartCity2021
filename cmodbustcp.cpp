#include "cmodbustcp.h"
#include "cmodbustcp.h"

CModbusTcp::CModbusTcp(QObject *parent) : QObject(parent)
{

}

CModbusTcp::~CModbusTcp()
{

}

int CModbusTcp::on_trameClient(QByteArray trameClient)
{
    //les trames au complet contiendrons :data-crc16:
    _tc += trameClient;

    //test et purge début trame
    int deb = _tc.indexOf(":",0); // recherche début trame
    if (deb == -1) { // si pas de car de début
        _tc.clear();
        emit sig_erreur("CProtocleClient::on_trameClient : Pas de caractère de début de trame");
        return -1;
    } // if pas de car de debut
    _tc.remove(0, deb+1);  // on enlève tout avant le : au cas ou

    // test et purge fin de trame
    int fin = _tc.indexOf(":",1);
    if (fin == -1) return 0; // on attend la suite
    _tc.remove(fin, _tc.size()-fin);  // au cas ou, on enlève tout après la fin

    return 1;
}

bool CModbusTcp::verifier()
{
    uint16_t crc16 = static_cast<uint16_t>((_tc[_tc.size()-3]<<8) + _tc[_tc.size()-2]);
        //on récupere le Crc de la trame
    uint16_t crc16Calc = calculCrc16();
        //on calcule le crc de la trame
    if (crc16 == crc16Calc) //on les compares
        return true;
    else {
        _tc.clear();
        emit sig_erreur("CModbusTcp::verifier() : Erreur de CRC16");
        return 0;
    }
}

uint16_t CModbusTcp::calculCrc16()
{
    uint8_t nbDec;
    uint8_t yaun;
    uint8_t indice;
    uint16_t crc;

    crc = 0xFFFF;
    indice = 0;

    do {
        crc ^= _tc[indice];
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
    } while (indice < (_tc.size()-3));

    return crc;
}

int CModbusTcp::decoder()
{
    //décodage de "_data" pour traduire un ordre
    //cet odre sera afilié a un nombre et retourné par la fonction, dans on ready read
    return 0;
}



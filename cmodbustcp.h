#ifndef CMODBUSTCP_H
#define CMODBUSTCP_H

#include <QObject>
#include <QDebug>
#include <QChar>
#include <QByteArray>

class CModbusTcp : public QObject
{
    Q_OBJECT

public:
    explicit CModbusTcp(QObject *parent = nullptr);
    ~CModbusTcp();

    int on_trameClient(QByteArray trameClient);
    bool verifier();
    int decoder();
    QByteArray reponseEcriture(bool exec);
    QByteArray reponseLecture(QByteArray val);
    bool verificationMdp();
    void deleteTc();

    uint valueOf(QByteArray ByteArray);

    QByteArray get_tc();
    uint8_t get_Addr1WordInt();
    uint8_t get_nbrOfWords();
    int get_functionCode();



private:
    int decodeParking();
    int decodeIntersection();
    int decodeEclairage();
    int decodeAuthentification();

    uint16_t calculCrc16(QByteArray ByteArray);
    QByteArray takeCharacter(int nbOfBytes);

    QByteArray _tc;  // trame du client
    QByteArray _reponse;
    QByteArray _Addr1Word;
    QByteArray _nbrOfWord;
    int _Addr1WordInt;
    int _fonction;
    int _mode = 0;

signals:
    void sig_erreur(QString mess);
    void sig_info(QString mess);

private slots:

    void on_erreur(QString mess);
    void on_info(QString mess);

};

#endif // CMODBUSTCP_H

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
    int get_functionCode();
    QByteArray get_tc();

    QByteArray _Addr1Word;
    QByteArray _nbrOfWord;

private:

    QByteArray _tc;  // trame du client
    QByteArray _reponse;
    int _fonction;
    int _mode = 0;

    int decodeParking();
    int decodeIntersection();
    int decodeEclairage();
    int decodeAuthentification();
    uint16_t calculCrc16(QByteArray ByteArray);
    QByteArray takeCharacter(int nbOfBytes);
    uint valueOf(QByteArray ByteArray);

signals:
    void sig_erreur(QString mess);
    void sig_info(QString mess);

private slots:

    void on_erreur(QString mess);
    void on_info(QString mess);

};

#endif // CMODBUSTCP_H

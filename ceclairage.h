#ifndef CECLAIRAGE_H
#define CECLAIRAGE_H

#include <QObject>

#include "czdc.h"
#include "ci2c.h"

#define ORDRE_RECU 128

typedef struct{     //LSB vers MSB
    uint8_t
        bitPresence : 1, bitJN : 1, bitLamps : 6;
}T_LAMP;

typedef union{
    T_LAMP partie;
    unsigned char octet;
}U_LAMP;

class CEclairage : public QObject
{
    Q_OBJECT
public:
    explicit CEclairage(CZdc *zdc = nullptr, QObject *parent = nullptr);
    ~CEclairage();

private:
    CI2c *_i2c;
    CZdc *_zdc;

signals:
    void sigEclair(int addr, int nb, int addr_base);
    void sigParking();
    void sigRestart();

public slots:
    void on_sigEclair(int addr, int nb, int addr_base);

};

#endif // CECLAIRAGE_H

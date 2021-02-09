#ifndef CPARKING_H
#define CPARKING_H

#include <QObject>
#include "ci2c.h"

class CParking : public QObject
{
    Q_OBJECT
public:
    explicit CParking(QObject *parent = nullptr);

private:
    CI2c *_i2c;

signals:

public slots:
    void on_OrderBarrier(uint8_t ordres);
};

#endif // CPARKING_H

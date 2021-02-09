#ifndef CECRAN_H
#define CECRAN_H

#include <QObject>

#include "ci2c.h"

class CEcran : public QObject
{
    Q_OBJECT
public:
    explicit CEcran(QObject *parent = nullptr);

private:
    CI2c *_i2c;

signals:

public slots:
};

#endif // CECRAN_H

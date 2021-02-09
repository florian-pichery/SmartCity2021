#ifndef CECLAIRAGE_H
#define CECLAIRAGE_H

#include <QObject>

#include "ci2c.h"

class CEclairage : public QObject
{
    Q_OBJECT
public:
    explicit CEclairage(QObject *parent = nullptr);

private:
    CI2c *_i2c;

signals:

public slots:

};

#endif // CECLAIRAGE_H

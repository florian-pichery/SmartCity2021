#ifndef CINTERSECTION_H
#define CINTERSECTION_H

#include <QObject>

#include "ci2c.h"

class CIntersection : public QObject
{
    Q_OBJECT
public:
    explicit CIntersection(QObject *parent = nullptr);

private:
    CI2c *_i2c;

signals:

public slots:
};

#endif // CINTERSECTION_H

#ifndef CECRAN_H
#define CECRAN_H

#include <QObject>

class CEcran : public QObject
{
    Q_OBJECT
public:
    explicit CEcran(QObject *parent = nullptr);

signals:

public slots:
};

#endif // CECRAN_H
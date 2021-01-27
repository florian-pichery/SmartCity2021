#ifndef CZDC_H
#define CZDC_H
// ZDC : Zone De Données Communes

#include <QObject>
#include <QSharedMemory>
#include "capp.h"

class CZdc : public QSharedMemory
{
        Q_OBJECT
public:
    explicit CZdc(QObject *parent = nullptr);
    ~CZdc();

private:

    void clear();

public slots:

    void on_sigErreur();
    void on_sigUpdate();

signals:
    void sig_erreur(QString mess);
    void sig_update();
};

#endif // CZDC_H

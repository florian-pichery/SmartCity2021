#ifndef CIHM_H
#define CIHM_H

#include <QMainWindow>

#include "czdc.h"

QT_BEGIN_NAMESPACE
namespace Ui { class cihm; }
QT_END_NAMESPACE

class cihm : public QMainWindow
{
    Q_OBJECT

public:
    cihm(QWidget *parent = nullptr);
    ~cihm();

private slots:
    void on_pbPresence_clicked();

    void on_pbAbsence_clicked();

private:
    Ui::cihm *ui;
    CZdc *_zdc;
};
#endif // CIHM_H

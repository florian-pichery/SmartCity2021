#ifndef CIHMI2C_H
#define CIHMI2C_H

#include <QMainWindow>
#include "cgestionmaquette.h"


namespace Ui { class CIhmI2c; }

class CIhmI2c : public QMainWindow
{
    Q_OBJECT

public:
    explicit CIhmI2c(QWidget *parent = nullptr);
    ~CIhmI2c();

private:
    Ui::CIhmI2c *ui;
    CGestionMaquette *_maquette;
};

#endif // CIHMI2C_H

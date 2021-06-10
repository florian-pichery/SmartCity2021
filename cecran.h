#ifndef CECRAN_H
#define CECRAN_H

#include <QObject>
#include <QDebug>
#include <QThread>

#include "czdc.h"
#include "ci2c.h"
#include "cbdd.h"

// Device I2C Adresses
#define LCD_ADDRESS     0x3e
#define RGB_ADDRESS     0x62

// color define
#define WHITE           0
#define RED             1
#define GREEN           2
#define BLUE            3
#define BLACK           4

#define REG_RED         0x04        // pwm2
#define REG_GREEN       0x03        // pwm1
#define REG_BLUE        0x02        // pwm0

#define REG_MODE1       0x00
#define REG_MODE2       0x01
#define REG_OUTPUT      0x08

// commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

class CEcran : public QObject
{
    Q_OBJECT
public:
    explicit CEcran(CZdc *zdc, QObject *parent);
    ~CEcran();
    void begin(int rows = 2, int charsize = LCD_5x8DOTS);
    void clear();
    void home();
    void noDisplay();
    void display();
    void noBlink();
    void blink();
    void noCursor();
    void cursor();
    void scrollDisplayLeft();
    void scrollDisplayRight();
    void leftToRight();
    void rightToLeft();
    void autoscroll();
    void noAutoscroll();
    void createChar(int, int[]);
    void setCursor(int, int);
    int ecrire(QString text);
    void command(int);
    // color control
    void setRGB(unsigned char r, unsigned char g, unsigned char b);               // set rgb
    void setPWM(unsigned char color, unsigned char pwm){setReg(color, pwm);}      // set pwm
    void setColor(unsigned char color);
    void setColorOff(){setRGB(0, 0, 0);}
    void setColorWhite(){setRGB(255, 255, 255);}
    // blink the LED backlight
    void blinkLED(void);
    void noBlinkLED(void);

private:
    CI2c *_i2c;
    CZdc *_zdc;
    CBdd *_bdd;
    void send(int, int);
    void setReg(unsigned char adr, unsigned char data);
    int m_displayfunction;
    int m_displaycontrol;
    int m_displaymode;
    int m_initialized;
    int m_numlines;
    int m_currline;

signals:
    void sigErreur(QString mess);
    void sigInter();
    void sigEcran();
    void sigRestart();

public slots:
    void onErreur(QString mess);
    void onEcran(QString nb);
};

#endif // CECRAN_H

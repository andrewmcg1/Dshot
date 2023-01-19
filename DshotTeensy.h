#ifndef D_SHOT
#define D_SHOT

#include "IntervalTimerEx.h"

class Dshot
{
  public:
    Dshot(int version, bool direction = false, bool tele = false);

    void attach(unsigned int num);

    void arm();
    void write(int value);

  public: // private
    bool biDirectional;
    bool tele;
    bool data[16];

    int baud;
    int pin;
    double highTime, lowTime, bitTime, frameTime;

    IntervalTimerEx bitTimer;
    IntervalTimerEx upTimer;

    void getBit();
    void sendBit();

    short int throttle(unsigned int value);
    int checkSum(int value);
    void bits(unsigned int value);
};


#endif // D_SHOT
/*

*/
#ifndef voice_h
#define voice_h

#include "Arduino.h"
#include <SPI.h>

class Voice
{
  public:
    Voice(int pin, int dac);
    void writeDAC();
    void on(int velocity, unsigned long timeOn);
    void off();
    boolean readGate();
    void setGateMode(boolean mode);
    boolean readGateMode();
    int readNote();
    unsigned long readTimeOn();
    
  private:
    int _pin;
    int _dac;
    boolean _gateMode;
    boolean _gateStatus;
    int _midiVelocity;
    unsigned long _timeOn;
    
};

#endif


#include "Arduino.h"
#include "voice.h"

#include <SPI.h>

Voice::Voice(int pin, int dac)
{
  _pin = pin;
  _dac = dac;
  _gateMode = 0;
}

void Voice::on(int velocity, unsigned long timeOn)
{
  _midiVelocity = velocity;
  _timeOn = timeOn;
  _gateStatus = 1;
  /*
  Serial.print("ON  - ");
  Serial.print(_pin);
  Serial.print(_dac);
  Serial.print(" - ");
  Serial.print(_midiVelocity);
  Serial.print(" - ");
  Serial.println(_timeOn);
  */
  writeDAC();

}

void Voice::off()
{
  
  _gateStatus = 0;
  _timeOn = 0;
  _midiVelocity = 0;
   writeDAC();

  /*
  Serial.print("OFF - ");
  Serial.print(_pin);
  Serial.println(_dac);
  */
}


void Voice::writeDAC() {  
  byte byte_0 = 0;
  // picks DAC 0-7
  bitSet(byte_0, _dac);
  byte byte_1 = _midiVelocity << 1 ;
  digitalWriteFast(_pin, LOW);  
  /*
  Serial.print("PIN ");
  Serial.print(_pin);
  Serial.print(" DAC ");
  Serial.print(_dac);
  Serial.print(" : ");
  Serial.println(byte_0,BIN);
  Serial.println(byte_1,BIN);
  */
  SPI.transfer(byte_0);
  SPI.transfer(byte_1);
  digitalWriteFast(_pin, HIGH);
}



boolean Voice::readGate()
{
  return _gateStatus;
}

void Voice::setGateMode(boolean mode) {
  _gateMode = mode;
}

boolean Voice::readGateMode()
{
  return _gateMode;
}

unsigned long Voice::readTimeOn()
{
  return _timeOn;
}

/*
 * DAC0 CS 10
 * DAC1 CS 8
 * MOSI 11
 * MISO 12
 * SCK  13
 * 
 */

#include <SPI.h>
#include "voice.h"

#define DAC0 10
#define DAC1 8
#define C_D_GATE_TOGGLE_PIN 5
#define F_G_A_GATE_TOGGLE_PIN 6
#define VELOCITY_TOGGLE_PIN 7
#define RANGE_TOGGLE_PIN 9

#define TRIGGER 0
#define GATE 1

#define FIVE_VOLTS 0
#define TEN_VOLTS 1

boolean range = 0;

boolean velocityMode = 0;

Voice voice[12] {
  Voice(DAC0,0),  // Voice 0, C
  Voice(DAC0,1),  // Voice 1, C#
  Voice(DAC0,2),  // Voice 2, D
  Voice(DAC0,3),  // Voice 3, D#
  Voice(DAC0,4),  // Voice 4, E
  Voice(DAC0,5),  // Voice 5, F
  Voice(DAC0,6),  // Voice 6, F#
  Voice(DAC0,7),  // Voice 7, G
  Voice(DAC1,0),  // Voice 8, G#
  Voice(DAC1,1),  // Voice 9, A
  Voice(DAC1,2),  // Voice 10, A#
  Voice(DAC1,3)   // Voice 11, B
};

void OnNoteOn(byte channel, byte note, byte velocity) {
  int voice_number = note % 12;
  if (velocityMode == 0) {
    velocity = 127;
  }
  velocity = velocity << range;
  voice[voice_number].on(velocity, micros());
}

void OnNoteOff(byte channel, byte note, byte velocity) {
    int voice_number = note % 12;
    // if ((voice[voice_number].readGate() == 1) && (voice[voice_number].readGateMode() == GATE))...
    // ... actually, I probably don't need to check to see if the gate is active.  Just turn it off.
    if (voice[voice_number].readGateMode() == GATE) {
      voice[voice_number].off();
    }
}



void setGateMode() {
  if (digitalRead(C_D_GATE_TOGGLE_PIN) == GATE) {
    // set voices to gate
    voice[1].setGateMode(GATE);
    voice[3].setGateMode(GATE);
  } else {
    // set voices to trig;
    voice[1].setGateMode(TRIGGER);
    voice[3].setGateMode(TRIGGER);
  }
  if (digitalRead(F_G_A_GATE_TOGGLE_PIN) == GATE) {
    // set voices to gate
    voice[6].setGateMode(GATE);
    voice[8].setGateMode(GATE);
    voice[10].setGateMode(GATE);
  } else {
    // set voices to trig;
    voice[6].setGateMode(TRIGGER);
    voice[8].setGateMode(TRIGGER);
    voice[10].setGateMode(TRIGGER);
  }
}

void setVelocityMode() {
    velocityMode = digitalRead(VELOCITY_TOGGLE_PIN);
    range = digitalRead(RANGE_TOGGLE_PIN);

}

void setup() {
  SPI.begin();  
  SPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE0));
  SPI.endTransaction();

  pinMode (DAC0, OUTPUT);
  digitalWrite(DAC0, HIGH);
  pinMode (DAC1, OUTPUT);
  digitalWrite(DAC1, HIGH);
  
  // config internal pulldowns for switches.
  pinMode (C_D_GATE_TOGGLE_PIN, INPUT_PULLDOWN);
  pinMode (F_G_A_GATE_TOGGLE_PIN, INPUT_PULLDOWN);
  pinMode (VELOCITY_TOGGLE_PIN, INPUT_PULLDOWN);
  pinMode (RANGE_TOGGLE_PIN, INPUT_PULLDOWN);


  // configure MAX 528s to "half buffered" mode
  byte byte_0 = B00000000;
  byte byte_1 = B10110110 ;
  digitalWrite(DAC0, LOW);
  SPI.transfer(byte_0);
  SPI.transfer(byte_1);
  digitalWrite(DAC0, HIGH);
  digitalWrite(DAC1, LOW);
  SPI.transfer(byte_0);
  SPI.transfer(byte_1);
  digitalWrite(DAC1, HIGH);
  
  usbMIDI.setHandleNoteOff(OnNoteOff);
  usbMIDI.setHandleNoteOn(OnNoteOn);
}

void loop() {
  usbMIDI.read();
  
  setGateMode();
  setVelocityMode();
  
  for (int i=0; i <= 11; i++){
    // if it's set to trigger and it's been on for more than a millisecond, kill it.
    if ((voice[i].readGateMode() == TRIGGER) && (voice[i].readGate() == 1) && (micros() >= voice[i].readTimeOn() + 1000)) {
       voice[i].off();
    }
  }
}

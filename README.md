# midi-trig
 Teensy LC based MIDI->TRIG converter for modular synths.

This is a very hastily written and not fully debugged 12 channel MIDI to TRIG (or GATE) converter for modular synthesizers.  I had two MAX528s layin' around, but this code could probably be adapted to other eight bit DACs.  

Each note on the chromatic scale triggers one of the 12 outputs, regardless of octave.  "Trigger Mode" means pressing down a note shoots off a 1ms trigger.  "Gate Mode" means the output goes high until the note is leg go.

There are four switches that change behavior when the pins go high:

* Pin 5 toggles "Gate Mode" for C# and D#
* Pin 6 toggles "Gate Mode" for F#, G#, and A#.
* Pin 7 turns velocity on or off for all notes.
* Pin 9 toggles range between 0-5 volts and 0-10 volts.

MAX528 wiring notes:

* VH is tied to +10V 
* VL is tied to ground.
* Pin 10 is used for DAC0's CS line.
* Pin 8 is used for DAC1's CS line.
* Pins for MOSI, MISO, and SCK on 11, 12, and 13 respectively.
* Outputs 4-7 on DAC1 are unused.


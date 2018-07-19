#ifndef IrBugCanLib_h
#define IrBugCanLib_h

#include "Arduino.h"
#include "SoftwareSerial.h"

enum {
  DATA_START,
  TIME_PASS,
  BUG_PEST,
  DATA_END,
  //DATA_LEN must be the last one
  DATA_LEN
};

#define DATA_START_VAL -1
#define DATA_END_VAL -2

#define RS485Transmit    HIGH
#define RS485Receive     LOW

class IrBugCanLib {
  public:
    IrBugCanLib(SoftwareSerial *serial, unsigned char pinTXControl,
        unsigned char pinTransmissionLED, int dataRate);
    void begin();
    void snedData(int, int);
  private:
    int _pinTXControl, _pinTransmissionLED, _dataRate;
	  SoftwareSerial *_rs485Serial;
    void blinkLED();
    String decToHex(int, byte);
};

#endif
#ifndef IrBugCanLib_h
#define IrBugCanLib_h

#include "Arduino.h"
#include "SoftwareSerial.h"

#define RS485Transmit    HIGH
#define RS485Receive     LOW

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

typedef struct IR_BUG_CAN_DATA
{
  unsigned int time_pass;
  unsigned int bug_pest; 
  unsigned int has_data; 

  unsigned int error_count;
  unsigned int illegal_serial_start_or_end;
} IrBugCanData;

static const IrBugCanData emptyIrBugCanData{};

class IrBugCanLib {
  public:
    IrBugCanLib(SoftwareSerial *serial, unsigned char pinTXControl,
        unsigned char pinTransmissionLED, int dataRate);
    void begin();
    void snedData(int, int);
    void readData(IrBugCanData &);
  private:
    int _pinTXControl, _pinTransmissionLED, _dataRate;
	  SoftwareSerial *_rs485Serial;
    void blinkLED();
    String decToHex(int, byte);
    unsigned int hexToDec(String);
};

#endif
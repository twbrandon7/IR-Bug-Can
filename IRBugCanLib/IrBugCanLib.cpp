#include "Arduino.h"
#include "IrBugCanLib.h"
#include "SoftwareSerial.h"

IrBugCanLib::IrBugCanLib(SoftwareSerial *serial, unsigned char pinTXControl,
                         unsigned char pinTransmissionLED, int dataRate)
{
    _pinTXControl = pinTXControl;
    _pinTransmissionLED = pinTransmissionLED;
    _dataRate = dataRate;
    _rs485Serial = serial;
}

void IrBugCanLib::begin()
{
    if(_pinTransmissionLED >= 0)
        pinMode(_pinTransmissionLED, OUTPUT);  
    pinMode(_pinTXControl, OUTPUT); 
    _rs485Serial->begin(_dataRate); 
}

void IrBugCanLib::blinkLED()
{
    if (_pinTransmissionLED >= 0)
    {
        digitalWrite(_pinTransmissionLED, LOW);
        delay(5);
        digitalWrite(_pinTransmissionLED, HIGH);
    }
}

String IrBugCanLib::decToHex(int decValue, byte desiredStringLength)
{
    String hexString = String(decValue, HEX);
    while (hexString.length() < desiredStringLength)
        hexString = "0" + hexString;

    return hexString;
}

void IrBugCanLib::snedData(int timePass, int bugPest)
{
    digitalWrite(_pinTXControl, RS485Transmit);
    String data[DATA_LEN];
    data[DATA_START] = this->decToHex(DATA_START_VAL, 4);
    data[TIME_PASS] = this->decToHex(timePass, 4);
    data[BUG_PEST] = this->decToHex(bugPest, 4);
    data[DATA_END] = this->decToHex(DATA_END_VAL, 4);
    for (int i = 0; i < DATA_LEN; i++)
    {
        for (int j = 0; j < data[i].length(); j++)
        {
            _rs485Serial->write(data[i].charAt(j));
            this->blinkLED();
        }
    }
}
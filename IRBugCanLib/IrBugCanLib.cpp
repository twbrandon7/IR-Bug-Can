#include "Arduino.h"
#include "IrBugCanLib.h"

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
    if (_pinTransmissionLED >= 0)
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

void IrBugCanLib::readData(IrBugCanData &data)
{
    digitalWrite(_pinTXControl, RS485Receive);
    data = emptyIrBugCanData;
    bool haveData = false;
    String bufferStr;
    while (_rs485Serial->available())
    {
        haveData = true;
        bufferStr += (char)_rs485Serial->read();
        delay(10);
    }
    if (haveData)
    {
        data.has_data++;
        int len = bufferStr.length() / 4;
        int dataArr[len];
        for (int i = 0; i < len; i++)
        {
            dataArr[i] = this->hexToDec(bufferStr.substring(i * 4, i * 4 + 4));
        }
        if (dataArr[0] == DATA_START_VAL && dataArr[len - 1] == DATA_END_VAL)
        {
            data.time_pass = dataArr[1];
            data.bug_pest = dataArr[2];
        }
        else
        {
            data.illegal_serial_start_or_end++;
            data.error_count++;
        }
    }
}

String IrBugCanLib::decToHex(int decValue, byte desiredStringLength)
{
    String hexString = String(decValue, HEX);
    while (hexString.length() < desiredStringLength)
        hexString = "0" + hexString;

    return hexString;
}

unsigned int IrBugCanLib::hexToDec(String hexString)
{
    unsigned int decValue = 0;
    int nextInt;

    for (int i = 0; i < hexString.length(); i++)
    {
        nextInt = int(hexString.charAt(i));
        if (nextInt >= 48 && nextInt <= 57)
            nextInt = map(nextInt, 48, 57, 0, 9);
        if (nextInt >= 65 && nextInt <= 70)
            nextInt = map(nextInt, 65, 70, 10, 15);
        if (nextInt >= 97 && nextInt <= 102)
            nextInt = map(nextInt, 97, 102, 10, 15);
        nextInt = constrain(nextInt, 0, 15);

        decValue = (decValue * 16) + nextInt;
    }
    return decValue;
}
/* 
 *  RS485 Module Master Example (Read data from bug can)
 *  Author : Li-Xian Chen (b0543017@ems.niu.edu.tw)
 *  2018/07/19
 *  
 *  Reference : YourDuino SoftwareSerialExample1
 *  https://arduino-info.wikispaces.com/SoftwareSerialRS485Example
*/
#include <IrBugCanLib.h>

/*-----( Declare Constants and Pin Numbers )-----*/
#define SSerialRX        10  //Serial Receive pin
#define SSerialTX        11  //Serial Transmit pin

#define SSerialTxControl 3   //RS485 Direction control
#define PinTransmissionLED 13
#define CommunicationBaudRate 4800

/*-----( Declare objects )-----*/
SoftwareSerial RS485Serial(SSerialRX, SSerialTX); // RX, TX
IrBugCanLib irBugCanLib(&RS485Serial, SSerialTxControl, PinTransmissionLED, CommunicationBaudRate);

void setup() {
  // Start the built-in serial port, probably to Serial Monitor
  Serial.begin(9600);   
  irBugCanLib.begin();
}

void loop() {
  IrBugCanData data;
  irBugCanLib.readData(data);
  if(data.has_data){
    Serial.println(String(data.has_data));
    Serial.println("Time Pass : " + String(data.time_pass));
    Serial.println("Bug Pest : " + String(data.bug_pest));
  }
}

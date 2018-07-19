/* 
 *  RS485 Module Master Example (Read data from bug can)
 *  Author : Li-Xian Chen (b0543017@ems.niu.edu.tw)
 *  2018/07/19
 *  
 *  Reference : YourDuino SoftwareSerialExample1
 *  https://arduino-info.wikispaces.com/SoftwareSerialRS485Example
*/
#include <SoftwareSerial.h>

/*-----( Declare Constants and Pin Numbers )-----*/
#define SSerialRX        10  //Serial Receive pin
#define SSerialTX        11  //Serial Transmit pin

#define SSerialTxControl 3   //RS485 Direction control

#define RS485Transmit    HIGH
#define RS485Receive     LOW

#define PinTransmissionLED         7

/*-----( Declare objects )-----*/
SoftwareSerial RS485Serial(SSerialRX, SSerialTX); // RX, TX

/*-----( Declare Variables )-----*/
int byteReceived;

void setup() {
  // Start the built-in serial port, probably to Serial Monitor
  Serial.begin(9600);
   
  pinMode(PinTransmissionLED, OUTPUT);   
  pinMode(SSerialTxControl, OUTPUT);    
  
  digitalWrite(SSerialTxControl, RS485Receive);  // Init Transceiver   
  
  // Start the software serial port, to another device
  RS485Serial.begin(4800);   // set the data rate 

}

void loop() {
  digitalWrite(PinTransmissionLED, HIGH);  // Show activity
  
  if (RS485Serial.available()) {
    digitalWrite(PinTransmissionLED, LOW);  // Show activity
    byteReceived = RS485Serial.read();    // Read received byte
    Serial.println(byteReceived);        // Show on Serial Monitor
    delay(10);
    digitalWrite(PinTransmissionLED, HIGH);  // Show activity   
  }  

}

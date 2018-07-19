/* 
 *  IR Bug Can RS485 Module Framework
 *  Author : Li-Xian Chen (b0543017@ems.niu.edu.tw)
 *  2018/07/19
 *  
 *  Reference : YourDuino SoftwareSerialExample1
 *  https://arduino-info.wikispaces.com/SoftwareSerialRS485Example
*/

#include <SoftwareSerial.h>

/*-----( Declare Constants and Pin Numbers )-----*/
#define PinRX        10  //pin which connected to RO
#define PinTX        11  //pin which connected to DI

#define PinTXControl 3   //RS485 Direction control

#define RS485Transmit    HIGH
#define RS485Receive     LOW

#define PinTransmissionLED         13

/*-----( Declare objects )-----*/
SoftwareSerial RS485Serial(PinRX, PinTX); // RX, TX

/*-----( Declare Variables )-----*/
int byteSend;

void setup() {
  // Start the built-in serial port, probably to Serial Monitor
  Serial.begin(9600);
  
  pinMode(PinTransmissionLED, OUTPUT);   
  pinMode(PinTXControl, OUTPUT);  
  
  digitalWrite(PinTXControl, RS485Transmit);  // Init Transceiver
  
  // Start the software serial port, to another device
  RS485Serial.begin(4800);   // set the data rate 
}


void loop() {
  byteSend = 87;    
  RS485Serial.write(byteSend); // Send the byte back
  digitalWrite(PinTransmissionLED, HIGH);  // Show activity    
  delay(10);     
  digitalWrite(PinTransmissionLED, LOW);
  Serial.println("SENT");
  delay(1000);    
}


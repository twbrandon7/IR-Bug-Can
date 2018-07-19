/* 
 *  IR Bug Can RS485 Module Framework
 *  Author : Li-Xian Chen (b0543017@ems.niu.edu.tw)
 *  2018/07/19
 *  
 *  Reference : YourDuino SoftwareSerialExample1
 *  https://arduino-info.wikispaces.com/SoftwareSerialRS485Example
*/

#include <IrBugCanLib.h>

/*-----( Declare Constants and Pin Numbers )-----*/
#define PinRX        10  //pin which connected to RO
#define PinTX        11  //pin which connected to DI

#define PinTXControl 3   //RS485 Direction control
#define PinTransmissionLED 13
#define CommunicationBaudRate 4800

#define PinSimulationButton 7
#define ReportDuration 5000 //millisecond

/*-----( Declare objects )-----*/
SoftwareSerial RS485Serial(PinRX, PinTX); // RX, TX
IrBugCanLib irBugCanLib(&RS485Serial, PinTXControl, PinTransmissionLED, CommunicationBaudRate);

int bugPestCount = 0;
unsigned long timeLast;

void setup() {
  // Start the built-in serial port, probably to Serial Monitor
  Serial.begin(9600);
  
  pinMode(PinSimulationButton, INPUT); 
  irBugCanLib.begin();
  timeLast = millis();
}

void loop() {
  if(digitalRead(PinSimulationButton)){
    bugPestCount++;
    Serial.println("Pass! current amount : " + String(bugPestCount));
  }

  if(millis() >= timeLast + ReportDuration){
    int actualDuration = (int) (millis() - timeLast);
    
    irBugCanLib.snedData(actualDuration, bugPestCount);
    
    bugPestCount = 0;
    Serial.println("SENT");
    timeLast = millis();
  }
}

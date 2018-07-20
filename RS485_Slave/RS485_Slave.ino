/* 
 *  IR Bug Can RS485 Module Framework
 *  Author : Li-Xian Chen (b0543017@ems.niu.edu.tw)
 *  2018/07/20
 *  
 *  Reference : https://github.com/angeloc/simplemodbusng
*/

#include <SoftwareSerial.h>
#include <SimpleModbusSlaveSoftwareSerial.h>

/*-----( Declare Constants and Pin Numbers )-----*/
#define PinRX        10  //pin which connected to RO
#define PinTX        11  //pin which connected to DI

#define PinTXControl 2   //RS485 Direction control
#define PinTransmissionLED 13
#define CommunicationBaudRate 4800

#define PinSimulationButton 7

#define DeviceId 3

enum 
{     
  // just add or remove registers and your good to go...
  // The first register starts at address 0
  PWM_0, //Preserve for TX
  PWM_1, //Preserve for RX
  ADC0, //Preserve for PinTXControl
  TIME_PASS, //Time Pass
  BUG_PEST, //Bug Pest
  TOTAL_ERRORS,
  // leave this one
  TOTAL_REGS_SIZE 
  // total number of registers for function 3 and 16 share the same register array
};

unsigned int holdingRegs[TOTAL_REGS_SIZE]; // function 3 and 16 register array

int bugPestCount = 0;
unsigned long timeLast;

SoftwareSerial rs485Serial(PinRX, PinTX);

void setup() {
  // Start the built-in serial port, probably to Serial Monitor
  Serial.begin(9600);
  modbus_configure(&rs485Serial, CommunicationBaudRate, DeviceId, PinTXControl, TOTAL_REGS_SIZE);
  pinMode(PinSimulationButton, INPUT); 
  timeLast = millis();
}

void loop() {
  if(digitalRead(PinSimulationButton)){
    bugPestCount++;
    Serial.println("Pass! current amount : " + String(bugPestCount));
  }

  holdingRegs[TOTAL_ERRORS] = modbus_update(holdingRegs);
  holdingRegs[TIME_PASS] = 0;
  holdingRegs[BUG_PEST] = bugPestCount;
//  if(millis() >= timeLast + ReportDuration){
//    int actualDuration = (int) ((millis() - timeLast) / 1000); //minute
//    
//    irBugCanLib.snedData(actualDuration, bugPestCount);
//    
//    bugPestCount = 0;
//    Serial.println("SENT");
//    timeLast = millis();
//  }
}

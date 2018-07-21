/* 
 *  IR Bug Can RS485 Modbus RTU
 *  Author : Li-Xian Chen (b0543017@ems.niu.edu.tw)
 *  2018/07/21
 *  
 *  Reference : https://github.com/smarmengol/Modbus-Master-Slave-for-Arduino
*/

#include <ModbusRtu.h>

/*-----( Declare Constants and Pin Numbers )-----*/
#define PinRX        10  //pin which connected to RO
#define PinTX        11  //pin which connected to DI

#define PinTXControl 2   //RS485 Direction control
#define PinTransmissionLED 13
#define CommunicationBaudRate 57600
#define DeviceId 3
#define DataLength 16
#define TimeoutArrLen 16

#define PinSimulationButton 7

enum{
  HOURS,
  MINUTES,
  SECONDS,
  BUG_PEST,
  END,
  DATA_LENGTH
};

void addSecond();
void addMinute();
void addHour();
void resetTimer();

uint16_t au16data[DATA_LENGTH];
uint16_t data[DATA_LENGTH];

unsigned int bugPestCount = 0, hours = 0, minutes = 0, seconds = 0;
unsigned long timeLast;
bool lastTimeOutState = true;

/**
 *  Modbus object declaration
 *  u8id : node id = 0 for master, = 1..247 for slave
 *  u8serno : serial port (use 0 for Serial, >= 4 for SoftwareSerial)
 *  u8txenpin : 0 for RS-232 and USB-FTDI 
 *               or any pin number > 1 for RS-485
 */
Modbus slave(DeviceId, 4, PinTXControl); // this is slave @1 and RS-485
SoftwareSerial rs485Serial(PinRX, PinTX);

void setup() {
  au16data[END] = -1;
  data[END] = -1;

  slave.begin(&rs485Serial, CommunicationBaudRate); // baud-rate at 19200
  slave.setTimeOut(1500);
  
  pinMode(PinSimulationButton, INPUT); 
  Serial.begin(9600);
  
  timeLast = millis();
}

void loop() {
  au16data[HOURS] = hours;
  au16data[MINUTES] = minutes;
  au16data[SECONDS] = seconds;
  au16data[BUG_PEST] = bugPestCount;

  if(slave.getTimeOutState()){
    memcpy( data, au16data, DATA_LENGTH*sizeof(data[0]) );
  }
  
  slave.poll( data, DATA_LENGTH );
  
  if(digitalRead(PinSimulationButton)){
    bugPestCount++;
    Serial.println("Pass! current amount : " + String(bugPestCount));
  }

  if(millis() - timeLast >= 1000){
    addSecond();
    timeLast = millis();
  }
  
  if(lastTimeOutState && !slave.getTimeOutState()){ //Connected
    bugPestCount = 0;
    resetTimer();
  }else if(!lastTimeOutState && slave.getTimeOutState()){ //Disconnected
    //
  }
  
  lastTimeOutState = slave.getTimeOutState();
}

void addSecond(){
  if(seconds + 1 >= 60){
    seconds = 0;
    addMinute();
  }else{
    seconds++;
  }
}

void addMinute(){
  if(minutes + 1 >= 60){
    minutes = 0;
    addHour();
  }else{
    minutes++;
  }
}

void addHour(){
  hours++;
}

void resetTimer(){
  hours = 0; minutes = 0; seconds = 0;
}

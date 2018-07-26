/* 
 *  IR Bug Can with RS485 Modbus RTU
 *  Author : Li-Xian Chen (b0543017@ems.niu.edu.tw)
 *  2018/07/25
 *  
 *  Reference : https://github.com/smarmengol/Modbus-Master-Slave-for-Arduino
*/

#include <ModbusRtu.h>

/*-----( Declare Constants and Pin Numbers )-----*/
#define PinRX        10  //pin which connected to RO
#define PinTX        11  //pin which connected to DI

#define PinTXControl 2   //RS485 Direction control (DE, RE in RS485 Module)
#define PinBugLED 5
#define PinConnectedLED 6
#define PinTransmissionLED 13
#define ModbusBaud 9600
#define DeviceId 3

#define IrPin 4  //the pin that connect to the IR receiver.

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
void doModbusSlave();
bool isIrBlock();

uint16_t au16data[DATA_LENGTH];
uint16_t data[DATA_LENGTH];

unsigned int bugCount = 0, hours = 0, minutes = 0, seconds = 0;
bool isLastIrBlock = false;
unsigned int irSignal = 0;
unsigned int tick = 0;

unsigned long timeLast;
bool lastTimeOutState = true;

/**
 *  Modbus object declaration
 *  u8id : node id = 0 for master, = 1..247 for slave
 *  u8serno : serial port (use 0 for Serial, >= 4 for SoftwareSerial)
 *  u8txenpin : 0 for RS-232 and USB-FTDI 
 *               or any pin number > 1 for RS-485
 */
Modbus slave(DeviceId, 4, PinTXControl); // this is slave @DeviceId and RS-485(SoftwareSerial)
SoftwareSerial rs485Serial(PinRX, PinTX);

void setup() {
  au16data[END] = -1;
  data[END] = -1;

  slave.begin(&rs485Serial, ModbusBaud);
  slave.setTimeOut(1500);
  
  pinMode(IrPin, INPUT_PULLUP);
  
  pinMode(PinTransmissionLED, OUTPUT);
  pinMode(PinBugLED, OUTPUT);
  pinMode(PinConnectedLED, OUTPUT);
  
  timeLast = millis();
  Serial.begin(57600);
}

void loop() {
  if(tick % 100 == 0){
    countBugPest();
  }
  doModbusSlave();
  tick = (tick +1) % 5000;
  delay(1);
}

void doModbusSlave(){
  au16data[HOURS] = hours;
  au16data[MINUTES] = minutes;
  au16data[SECONDS] = seconds;
  au16data[BUG_PEST] = bugCount;

  if(slave.getTimeOutState()){
    memcpy( data, au16data, DATA_LENGTH*sizeof(data[0]) );
  }
  
  slave.poll( data, DATA_LENGTH );
  if(!slave.getTimeOutState()){
    digitalWrite(PinTransmissionLED, LOW);
    delay(50);
    digitalWrite(PinTransmissionLED, HIGH);
    delay(50);
    digitalWrite(PinTransmissionLED, LOW);
  }

  if(millis() - timeLast >= 1000){
    addSecond();
    timeLast = millis();
  }
  
  if(lastTimeOutState && !slave.getTimeOutState()){ //Connected
    bugCount = 0;
    digitalWrite(PinConnectedLED, HIGH);
    resetTimer();
  }else if(!lastTimeOutState && slave.getTimeOutState()){ //Disconnected
    //
    digitalWrite(PinConnectedLED, LOW);
  }
  
  lastTimeOutState = slave.getTimeOutState();
}

void countBugPest(){
  bool irBlock = isIrBlock();
  if(!isLastIrBlock && irBlock){
//    Serial.println("BUG ENTER");
    bugCount++;
    digitalWrite(PinBugLED, HIGH);
    delay(50);
    digitalWrite(PinBugLED, LOW);
  }
  if(isLastIrBlock && !irBlock){
//    Serial.println("BUG LEAVE");
  }
  isLastIrBlock = irBlock;
}

bool isIrBlock(){
  irSignal = digitalRead(IrPin);
  if(irSignal == LOW) {
    return true;
  } else {
    return false;
  }
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

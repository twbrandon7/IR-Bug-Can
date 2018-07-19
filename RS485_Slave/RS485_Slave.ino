/* 
 *  IR Bug Can RS485 Module Framework
 *  Author : Li-Xian Chen (b0543017@ems.niu.edu.tw)
 *  2018/07/19
 *  
 *  Reference : YourDuino SoftwareSerialExample1
 *  https://arduino-info.wikispaces.com/SoftwareSerialRS485Example
*/

#include <SoftwareSerial.h>

//Data format
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

/*-----( Declare Constants and Pin Numbers )-----*/
#define PinRX        10  //pin which connected to RO
#define PinTX        11  //pin which connected to DI

#define PinTXControl 3   //RS485 Direction control

#define RS485Transmit    HIGH
#define RS485Receive     LOW

#define PinTransmissionLED 13

#define PinSimulationButton 7

#define ReportDuration 5000 //millisecond

void snedData(int, int);
void blinkLED(unsigned char);
String decToHex(int, byte);
/*-----( Declare objects )-----*/
SoftwareSerial RS485Serial(PinRX, PinTX); // RX, TX

/*-----( Declare Variables )-----*/
int byteSend;

int bugPestCount = 0;
unsigned long timeLast;

void setup() {
  // Start the built-in serial port, probably to Serial Monitor
  Serial.begin(9600);
  
  pinMode(PinTransmissionLED, OUTPUT);   
  pinMode(PinTXControl, OUTPUT); 
  pinMode(PinSimulationButton, INPUT); 
  
  digitalWrite(PinTXControl, RS485Transmit);  // Init Transceiver
  
  // Start the software serial port, to another device
  RS485Serial.begin(4800);   // set the data rate 
  timeLast = millis();
}

void loop() {
  if(digitalRead(PinSimulationButton)){
    bugPestCount++;
    Serial.println("Pass! current amount : " + String(bugPestCount));
  }

  if(millis() >= timeLast + ReportDuration){
    int actualDuration = (int) (millis() - timeLast);
    
    snedData(actualDuration, bugPestCount);
    
    bugPestCount = 0;
    Serial.println("SENT");
    timeLast = millis();
  }
}

void snedData(int timePass, int bugPest){
  String data[DATA_LEN];
  data[DATA_START] = decToHex(DATA_START_VAL, 4);
  data[TIME_PASS] = decToHex(timePass, 4);
  data[BUG_PEST] = decToHex(bugPest, 4);
  data[DATA_END] = decToHex(DATA_END_VAL, 4);
  for(int i = 0; i < DATA_LEN; i++){
    for(int j = 0; j < data[i].length(); j++){
      RS485Serial.write(data[i].charAt(j));
      blinkLED(PinTransmissionLED);
    }
  }
}

void blinkLED(unsigned char pin){
  digitalWrite(pin, LOW);
  delay(5);
  digitalWrite(pin, HIGH);
}

String decToHex(int decValue, byte desiredStringLength) {
  String hexString = String(decValue, HEX);
  while (hexString.length() < desiredStringLength) hexString = "0" + hexString;
  
  return hexString;
}



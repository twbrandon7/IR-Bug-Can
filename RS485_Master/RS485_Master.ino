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

typedef struct IR_BUG_CAN_DATA
{
  unsigned int time_pass;
  unsigned int bug_pest; 
  unsigned int has_data; 

  unsigned int error_count;
  unsigned int illegal_serial_start_or_end;
}IrBugCanData;

static const IrBugCanData emptyIrBugCanData{};

void readData(IrBugCanData &);
unsigned int  hexToDec(String);

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
  
  IrBugCanData data;
  readData(data);
  if(data.has_data){
    Serial.println(String(data.has_data));
    Serial.println("Time Pass : " + String(data.time_pass));
    Serial.println("Bug Pest : " + String(data.bug_pest));
  }
}

void readData(IrBugCanData &data){
  data = emptyIrBugCanData;
  bool haveData = false;
  String bufferStr;
  while(RS485Serial.available()){
    haveData = true;
    bufferStr += (char)RS485Serial.read();
    delay(10);
  }
  if(haveData){
    data.has_data++;
    int len = bufferStr.length() / 4;
    int dataArr[len];
    for(int i = 0; i < len; i++){
      dataArr[i] = hexToDec(bufferStr.substring(i*4, i*4+4));
    }
    if(dataArr[0] == -1 && dataArr[len-1] == -2){
      data.time_pass = dataArr[1];
      data.bug_pest = dataArr[2]; 
    }else{
      data.illegal_serial_start_or_end++;
      data.error_count++;
    }
  }
}

unsigned int hexToDec(String hexString) {
  
  unsigned int decValue = 0;
  int nextInt;
  
  for (int i = 0; i < hexString.length(); i++) {
    
    nextInt = int(hexString.charAt(i));
    if (nextInt >= 48 && nextInt <= 57) nextInt = map(nextInt, 48, 57, 0, 9);
    if (nextInt >= 65 && nextInt <= 70) nextInt = map(nextInt, 65, 70, 10, 15);
    if (nextInt >= 97 && nextInt <= 102) nextInt = map(nextInt, 97, 102, 10, 15);
    nextInt = constrain(nextInt, 0, 15);
    
    decValue = (decValue * 16) + nextInt;
  }
  
  return decValue;
}

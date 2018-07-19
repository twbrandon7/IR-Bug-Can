#include <IrBugCanLib.h>

SoftwareSerial ss(10,11);
IrBugCanLib irBugCanLib(&ss,3,13,4800);
void setup() {
  // put your setup code here, to run once:
  irBugCanLib.begin();
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  IrBugCanData data;
  irBugCanLib.readData(data);
  if(data.has_data){
    Serial.println(String(data.has_data));
    Serial.println("Time Pass : " + String(data.time_pass));
    Serial.println("Bug Pest : " + String(data.bug_pest));
  }
}

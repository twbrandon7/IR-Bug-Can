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
  irBugCanLib.snedData(1000, 0);
  Serial.println("SENT");
  delay(5000);
}

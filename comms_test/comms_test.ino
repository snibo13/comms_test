#include <comms.h>

BLDCMotor motor = BLDCMotor(15);

void setup() {
  Serial.begin(9600);
  Serial.println("Setting up....");
  initComms(&motor, true);
  

}

void loop() {
  // put your main code here, to run repeatedly:
  // Serial.println("BUM-BUM");
  
}

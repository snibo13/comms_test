#include <comms.h>

BLDCMotor motor = BLDCMotor(15);

void setup() {
  Serial.begin(115200);
  Serial.println("Setting up....");
  initComms(&motor, true);
  

}

byte packet[3] = {0x00, 0x01, 0x02};

void loop() {
  // put your main code here, to run repeatedly:
  // Serial.println("BUM-BUM");
  send_message(packet);
  delay(100);
  
}

#include "AT24MAC402.h"

AT24MAC402 at24mac402; // all address pins to GND
uint8_t mac[6];
uint8_t uuid[16];

void setup() {
  Serial.begin(9600);
	delay(2000);
  at24mac402.begin();
  at24mac402.readMac(mac);
  Serial.print("MAC address ");
  Serial.println(MACtoString(mac));
  at24mac402.readUUID(uuid);
  Serial.print("UUID ");
  Serial.println(UUIDtoString(mac));
  for (uint8_t i = 0; i < 6; i++) {
    at24mac402.writeByte(i, i);
    }
  for (uint8_t i = 0; i < 6; i++) {
    Serial.print("Byte ");
    Serial.print(i);
    Serial.print(" Value ");
    Serial.println(at24mac402.readByte(i));
    }
  }
  
void loop() {
  }
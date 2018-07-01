#include "slave_sender.h"

SlaveSender i2cSender(2);

void setup() {
  Wire.begin(8);
  Wire.onRequest(requestEvent);
  Serial.begin(115200);
  i2cSender.reset();
}

void loop() {
  i2cSender.setData(1, 5);
  i2cSender.setBitData(2, 2, 1);
  i2cSender.show();
}

void requestEvent(){
  i2cSender.update();
}


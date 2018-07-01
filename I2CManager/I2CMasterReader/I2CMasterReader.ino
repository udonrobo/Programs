#include "master_reader.h"

MasterReader i2cRead(8, 1);

void setup() {
  Wire.begin();
  Serial.begin(115200);
}

void loop() {
  i2cRead.update();
  //i2cRead.show();
  //Serial.print("\t");
  Serial.println(i2cRead.getData(1));
}

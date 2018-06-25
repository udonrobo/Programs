#include <Wire.h>
#include "i2c_master_writer_h"

const int ADDR=5;
const int DATA_SIZE=1;

i2cMasterWriter master(ADDR,DATA_SIZE);

void setup(){
    Wire.begin();
    Serial.begin(115200);
}

void loop(){
    static int counter;

    master.setData(0,counter);
    master.Update();
    
    counter++;
}
#include <Wire.h>
#include "i2c_slave_receiver.h"

const int ADDR = 5;
const int DATA_SIZE = 1;

i2cSlaveReceiver slave(ADDR, DATA_SIZE);

void setup()
{
    Wire.begin(ADDR);
    Wire.onReceive(receiveEvent);
    Serial.begin(11500);
}

int x;

void loop()
{

    x = slave.getData(0);

    slave.Show();
}

void receiveEvent(int any)
{
    slave.Update();
}
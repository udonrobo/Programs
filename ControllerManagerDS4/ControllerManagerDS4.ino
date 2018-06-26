#include <Wire.h>
#include "controller_manager_ds4.h"

const int ADDR = 8;

ControllerManagerDS4 DS4(ADDR);

void setup()
{
    Wire.begin();
    Serial.begin(115200);
}

void loop()
{
    DS4.Update();

    Serial.println(DS4.GetCircle());
}
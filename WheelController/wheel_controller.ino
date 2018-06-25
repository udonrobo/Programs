#include "wheel_controller.h"
#include"moving_average.h"

const int AVERAGE_SIZE = 100;
const int MAX_POWER = 250;
const int CEASETIME_MS = 500;

int16_t rightHatX, leftHatX, leftHatY;

static int16_t Power[4];

bool isLowSpeed, isReverse;

WheelController omni4(MAX_POWER, AVERAGE_SIZE);

void setup() {
  delay(2500);
  Serial.begin(115200);

  Omni4.SetPWMFrequency();
}

void loop() {
  omni4.setAxis(leftHatX,leftHatY,rightHatX,isLowSpeed,isReverse);

  omni4.Set4WheelPower(Power);
  omni4.Drive(Power);
  
}
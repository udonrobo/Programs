#pragma once
#include <Arduino.h>
#include <Wire.h>

class MasterReader {
  private:
    int i2cAddress;
    byte* data;
    int Size;

  public:
    masterReader(int Address, int arraySize);
    virtual ~masterReader() {
      delete[] data;
    }
    void Update();
    byte getData(int arrayNum);
    bool getBitData(int arrayNum, int bitNum);
    void show();
};


masterReader::masterReader(int Address, int arraySize) {
  data = new byte[arraySize];
  for (int i = 0; i < arraySize; i++) 
    data[i] = 0;
  Size = arraySize;
  i2cAddress = Address;
}

void masterReader::Update() {
  Wire.requestFrom(i2cAddress, Size);
  while (Wire.available()) {
    for (int i = 0; i < Size; i++)
      data[i] = Wire.read();
  }
}

byte masterReader::getData(int arrayNum) {
  return data[arrayNum - 1];
}

bool masterReader::getBitData(int arrayNum, int bitNum) {
  return bitRead(data[arrayNum], bitNum);
}

void masterReader::show() {
  for (int i = 0; i < Size; i++) {
    Serial.print(data[i]);
    Serial.print("\t");
  }
  Serial.println();
}

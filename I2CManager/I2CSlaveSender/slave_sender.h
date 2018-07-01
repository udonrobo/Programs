#pragma once
#include <arduino.h>
#include <Wire.h>

class SlaveSender {
  private:
    const int i2cAddress;
    byte* data;
    int Size;

  public:
    slaveSender(int arraySize);
    virtual ~slaveSender() {
      delete[] data;
    }
    void setData(int arrayNum, byte Data);
    void setBitData(int arrayNum, int BitNum, bool Data);
    void Reset();
    void Update();
    void show();
};

slaveSender::slaveSender(int arraySize) {
  data = new byte[arraySize];
  Size = arraySize;
}

void slaveSender::setData(int arrayNum, byte Data) {
  Data = constrain(Data, 0, 255);
  data[arrayNum - 1] = Data;
}

void slaveSender::setBitData(int arrayNum, int BitNum, bool Data) {
  Data = (bool)Data;
  bitWrite(data[arrayNum-1], BitNum - 1, Data);
}

void slaveSender::Reset() {
  for (int i = 0; i < Size; i++)
    data[i] = 0;
}

void slaveSender::Update() {
  Wire.write(data, Size);
}

void slaveSender::show() {
  for (int i = 0; i < Size; i++) {
    Serial.print(data[i]);
    Serial.print("\t");
  }
  Serial.println();
}


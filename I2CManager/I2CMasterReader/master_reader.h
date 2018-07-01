/*
  @program I2C通信でmasterがslaveからデータを受け取るクラス
  @data 2018/07/01
*/


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
  // デストラクタ
    virtual ~masterReader() {
      delete[] data;
    }
    void Update();
    byte getData(int arrayNum);
    bool getBitData(int arrayNum, int bitNum);
    void show();
};

/*
  コンストラクタ
  @param Address I2Cアドレス
  @param arraySize 送信するデータ個数
*/
masterReader::masterReader(int Address, int arraySize) {
  data = new byte[arraySize];
  for (int i = 0; i < arraySize; i++) 
    data[i] = 0;
  Size = arraySize;
  i2cAddress = Address;
}

//slave側から受け取り
void masterReader::Update() {
  Wire.requestFrom(i2cAddress, Size);
  while (Wire.available()) {
    for (int i = 0; i < Size; i++)
      data[i] = Wire.read();
  }
}

/*
  送られてきたデータの取得
  @param 何個目のデータか
*/
byte masterReader::getData(int arrayNum) {
  return data[arrayNum - 1];
}

/*
  送られてきたデータのbitの取得
  @param 何個目のデータか
  @param 何個目のbitか
*/
bool masterReader::getBitData(int arrayNum, int bitNum) {
  return bitRead(data[arrayNum], bitNum);
}

//送られてきたデータの全ての表示
void masterReader::show() {
  for (int i = 0; i < Size; i++) {
    Serial.print(data[i]);
    Serial.print("\t");
  }
  Serial.println();
}

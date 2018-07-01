/*
  @program I2C通信でmasterがslaveからデータを受け取るクラス
  @data 2018/07/01
*/


#pragma once
#include <Arduino.h>
#include <Wire.h>

class MasterReader {
  private:
    int I2CADDRESS;
    byte* data;
    int size;

  public:
    MasterReader(int address, int arraySize);
    // デストラクタ
    virtual ~MasterReader() {
      delete[] data;
    }
    void update();
    byte getData(int arrayNum);
    bool getBitData(int arrayNum, int bitNum);
    void show();
};

/*
  コンストラクタ
  @param Address I2Cアドレス
  @param arraySize 送信するデータ個数
*/
MasterReader::MasterReader(int address, int arraySize) {
  data = new byte[arraySize];
  for (int i = 0; i < arraySize; i++)
    data[i] = 0;
  size = arraySize;
  I2CADDRESS = address;
}

//slave側から受け取り
void MasterReader::update() {
  Wire.requestFrom(I2CADDRESS, size);
  while (Wire.available()) {
    for (int i = 0; i < size; i++)
      data[i] = Wire.read();
  }
}

/*
  送られてきたデータの取得
  @param 何個目のデータか
*/
byte MasterReader::getData(int arrayNum) {
  return data[arrayNum - 1];
}

/*
  送られてきたデータのbitの取得
  @param 何個目のデータか
  @param 何個目のbitか
*/
bool MasterReader::getBitData(int arrayNum, int bitNum) {
  return bitRead(data[arrayNum], bitNum);
}

//送られてきたデータの全ての表示
void MasterReader::show() {
  for (int i = 0; i < size; i++) {
    Serial.print(data[i]);
    Serial.print("\t");
  }
  Serial.println();
}

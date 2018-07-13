/*
  @progma I2C通信でslaveがmasterに送るクラス
  @date 2018/07/01
*/

#pragma once
#include <arduino.h>
#include <Wire.h>

class SlaveSender {
  private:
    const int I2CADDRESS;
    byte* data;
    int Size;

  public:
    SlaveSender(int arraySize);
  // デストラクタ
    virtual ~SlaveSender() {
      delete[] data;
    }
    void setData(int arrayNum, byte value);
    void setBitData(int arrayNum, byte bitNum, bool value);
    void reset();
    void update();
    void show();
};

/*
  コンストラクタ
  @param arraySize 送信するデータの個数
*/
SlaveSender::SlaveSender(int arraySize) {
  data = new byte[arraySize];
  Size = arraySize;
}

/*
  指定した配列にデータをセット
  @param arrayNum 何番目の配列か
  @param data 送信する値(0-255)
*/
void SlaveSender::setData(int arrayNum, byte value) {
  value = constrain(value, 0, 255);
  data[arrayNum - 1] = value;
}

/*
  指定した配列の指定したbitにデータをセット
  @param arrayNum 何番目の配列か
  @param BitNum   何番目のbitか
  @param data     送信する値(1or0)
*/
void SlaveSender::setBitData(int arrayNum, byte bitNum, bool value) {
  value = (bool)value;
  bitWrite(data[arrayNum-1], bitNum - 1, value);
}

//送るデータのリセット
void SlaveSender::reset() {
  for (int i = 0; i < Size; i++)
    data[i] = 0;
}

/*
  masterに送信
  requestEvent内で実行
*/
void SlaveSender::update() {
  Wire.write(data, Size);
}

//送信するデータの表示
void SlaveSender::show() {
  for (int i = 0; i < Size; i++) {
    Serial.print(data[i]);
    Serial.print("\t");
  }
  Serial.println();
}


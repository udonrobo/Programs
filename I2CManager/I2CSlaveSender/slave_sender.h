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
    void setData(int arrayNum, byte Data);
    void setBitData(int arrayNum, int BitNum, bool Data);
    void Reset();
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
  @param Data 送信する値(0-255)
*/
void SlaveSender::setData(int arrayNum, byte Data) {
  Data = constrain(Data, 0, 255);
  data[arrayNum - 1] = Data;
}

/*
  指定した配列の指定したbitにデータをセット
  @param arrayNum 何番目の配列か
  @param BitNum   何番目のbitか
  @param Data     送信する値(1or0)
*/
void SlaveSender::setBitData(int arrayNum, int BitNum, bool Data) {
  Data = (bool)Data;
  bitWrite(data[arrayNum-1], BitNum - 1, Data);
}

//送るデータのリセット
void SlaveSender::Reset() {
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


/*

  @program I2C通信でslaveへデータを送信するクラス
  @date 2018/06/25
  @author Watanabe Rui

*/

#pragma once

#include <arduino.h>
#include <Wire.h>

class i2cMasterWriter
{
private:

  const int M_ADDR;
  uint8_t *mData;
  uint *mSize;

public:

  /*  
        コンストラクタ
        @param  address     I2Cアドレス
        @param  array_size  送信するデータ個数
  */

  i2cMasterWriter(int address, uint data_size) : M_ADDR(address), *mSize(data_size)
  {

    data_ = new uint8_t[*mSize];
    for (int i = 0; i < *mSize; i++)
      data_[i] = 0;
  }

  //  デストラクタ
  virtual ~i2cMasterWriter()
  {
    delete[] data_;
  }

  //送るデータの中身を表示
  void Show()
  {
    for (int i = 0; i < *mSize; i++)
    {
      Serial.print(data_[i]);
      Serial.print("\t");
    }
    Serial.println("");
  }

  /*  
        指定した添字の配列にデータをセット
        @param  array_num  配列の添字
        @param  val        送信する値 (0-255)
  */

  void setData(int array_num, uint8_t val)
  {
    data_[array_num] = val;
  }

  /*  
        指定した添字の配列にビットデータをセット
        @param  array_num  配列の添字
        @param  bit_num    bit番号
        @param  bit        1 or 0    
  */
  
  void setBitData(int array_num, byte bit_num, bool bit)
  {
    bitWrite(data_[array_num], bit_num, bit);
  }

  //  データの初期化
  void Reset()
  {
    for (int i = 0; i < *mSize; i++)
      data_[i] = 0;
  }

  //  Slave側へ送信
  void Update()
  {
    Wire.beginTransmission(M_ADDR);
    Wire.write(data_, *mSize);
    Wire.endTransmission();
  }

};
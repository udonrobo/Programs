/*

  @program I2C通信でmasterからデータを受け取るクラス
  @date 2018/06/25
  @author Watanabe Rui

*/

#pragma once

#include <arduino.h>
#include <Wire.h>

//I2C通信でマスターからデータを受け取るクラス
class i2cSlaveReceiver
{

  private:

    const int M_ADDR;
    uint8_t *mData;
    uint *mSize;

  public:

    /*
        コンストラクタ
        @param  address    I2Cアドレス
        @param  data_size  受け取るデータの個数
    */

    i2cSlaveReceiver(int address, uint data_size) : M_ADDR(address), *mSize(data_size)
    {

        data_ = new uint8_t[*mSize];
        for (int i = 0; i < *mSize; i++)
            data_[i] = 0;
    }

    //  デストラクタ
    virtual ~i2cSlaveReceiver()
    {
        delete[] data_;
    }

    /*
        データを取得
        @param  array_num  受け取りたいデータを格納している配列の添字
    */

    uint8_t getData(uint array_num)
    {
        return data_[array_num];
    }

    /*
        データを一括で取得
        @param  array   受け取る用の配列
    */

    void getData(uint8_t array[])
    {
        for (int i = 0; i < *mSize; i++)
            array[i] = data_[i];
    }

    //  データの初期化
    void Reset()
    {
        for (int i = 0; i < *mSize; i++)
            data_[i] = 0;
    }

    //  受け取ったデータの中身を表示
    void Show()
    {
        for (int i = 0; i < *mSize; i++)
        {
            Serial.print(data_[i]);
            Serial.print("\t");
        }
        Serial.println("");
    }

    //  データの更新[ReceiveEvent内での処理]
    void Update()
    {
        while (Wire.available() >= *mSize)
        {
            for (int i = 0; i < *mSize; i++)
                data[i] = Wire.read();
        }
    }
};
/*
    @Description    Mbedで読み取ったエンコーダの値をArduinoで受け取るクラス
    @Author    Watanabe Rui
    @Date    2018/05/11
*/

#pragma once

#include <Wire.h>

class EncoderBoardMbed
{

private:
  int ADDR; //I2Cアドレス

  //4byteのデータが1byte区切りで送られてくるのでそれぞれを一時格納しておく
  long buf[4][4];

  long offsetCount[4], nowCount[4];

public:
  /*
          コンストラクタ
          @param address MbedとのI2Cアドレス
    */
  EncoderBoardMbed(int address)
  {
    ADDR = address >> 1; // Mbed => 7bit，Arduino => 8bitでアドレス指定するので右に一つシフト

    for (int i = 0; i < 4; i++)
    {
      nowCount[i] = 0;
      offsetCount[i] = 0;
    }
  }

  /*
    デストラクタ
  */
  virtual ~EncoderBoardMbed() {}

  /*
       mbedから全てのエンコーダの値を受信
    */
  void update()
  {
    Wire.requestFrom(ADDR, 16); //データ要求

    while (Wire.available() >= 16)
    {

      buf[0][0] = Wire.read();
      buf[0][1] = Wire.read();
      buf[0][2] = Wire.read();
      buf[0][3] = Wire.read();

      buf[1][0] = Wire.read();
      buf[1][1] = Wire.read();
      buf[1][2] = Wire.read();
      buf[1][3] = Wire.read();

      buf[2][0] = Wire.read();
      buf[2][1] = Wire.read();
      buf[2][2] = Wire.read();
      buf[2][3] = Wire.read();

      buf[3][0] = Wire.read();
      buf[3][1] = Wire.read();
      buf[3][2] = Wire.read();
      buf[3][3] = Wire.read();
    }

    //ビットシフトして4byteに復元
    nowCount[0] = (buf[0][0] << 24) | (buf[0][1] << 16) | (buf[0][2] << 8) | buf[0][3];
    nowCount[1] = (buf[1][0] << 24) | (buf[1][1] << 16) | (buf[1][2] << 8) | buf[1][3];
    nowCount[2] = (buf[2][0] << 24) | (buf[2][1] << 16) | (buf[2][2] << 8) | buf[2][3];
    nowCount[3] = (buf[3][0] << 24) | (buf[3][1] << 16) | (buf[3][2] << 8) | buf[3][3];
  }

  /*
      エンコーダ基板の初期化
    */
  void init()
  {
    //updateを二回行うと値を取りこぼす
    for (int i = 0; i < 2; i++)
      update();

    resetCount();
  }

  /*
    　エンコーダの回転数を取得
      @param port 基板のポート番号
    */
  long getCount(int port)
  {
    return nowCount[port - 1] - offsetCount[port - 1];
  }

  /*
      オーバーロード
    　エンコーダの全ポートの回転数を取得
      @param val[] 値を受け取る配列
    */
  void getCount(long val[])
  {
    for (int i = 0; i < 4; i++)
      val[i] = nowCount[i] - offsetCount[i];
  }

  /*
      エンコーダの回転数の初期化
      @param port 基板のポート番号
    */
  void resetCount(int port)
  {
    offsetCount[port - 1] = nowCount[port - 1];
  }

  /*
      オーバーロード
      エンコーダの全ポートの回転数の初期化
    */
  void resetCount()
  {
    for (int i = 0; i < 4; i++)
    {
      offsetCount[i] = nowCount[i];
    }
  }
};

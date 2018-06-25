/*
    @Description    Mbedで読み取ったエンコーダの値をArduinoで受け取るクラス
    @Author    Watanabe Rui
    @Date    2018/05/11
*/

#pragma once

#include<Wire.h>

class EncoderBoardMbed {
  
  private:
    int mADDR; //I2Cアドレス

    long mEncBuf[4][4];
    long mOffsetEncData[4],mNowEncData[4];
    
  public:

    /*
          コンストラクタ
          @param ADDR MbedとのI2Cアドレス
    */
    EncoderBoardMbed(int ADDR) {
      mADDR = ADDR >> 1; // Mbed->7bit，Arduino->8bitでアドレス指定するので右に一つシフト
    }

    /*
       mbedから全てのエンコーダの値を受信
    */
    void Update() {
      Wire.requestFrom(mADDR, 16);
      while(Wire.available()>=16){
            mEncBuf[0][0] = Wire.read();
            mEncBuf[0][1] = Wire.read();
            mEncBuf[0][2] = Wire.read();
            mEncBuf[0][3] = Wire.read();

            mEncBuf[1][0] = Wire.read();
            mEncBuf[1][1] = Wire.read();
            mEncBuf[1][2] = Wire.read();
            mEncBuf[1][3] = Wire.read();

            mEncBuf[2][0] = Wire.read();
            mEncBuf[2][1] = Wire.read();
            mEncBuf[2][2] = Wire.read();
            mEncBuf[2][3] = Wire.read();

            mEncBuf[3][0] = Wire.read();
            mEncBuf[3][1] = Wire.read();
            mEncBuf[3][2] = Wire.read();
            mEncBuf[3][3] = Wire.read();
      }
      
      mOffsetEncData[0] = (mEncBuf[0][0] << 24) | (mEncBuf[0][1] << 16) | (mEncBuf[0][2] << 8) | mEncBuf[0][3];
      mOffsetEncData[1] = (mEncBuf[1][0] << 24) | (mEncBuf[1][1] << 16) | (mEncBuf[1][2] << 8) | mEncBuf[1][3];
      mOffsetEncData[2] = (mEncBuf[2][0] << 24) | (mEncBuf[2][1] << 16) | (mEncBuf[2][2] << 8) | mEncBuf[2][3];
      mOffsetEncData[3] = (mEncBuf[3][0] << 24) | (mEncBuf[3][1] << 16) | (mEncBuf[3][2] << 8) | mEncBuf[3][3];
    
    }

    /*
      エンコーダ基板の初期化
    */

   void init(){
     Update();
     resetCount();
   }


    /*
    　エンコーダの回転数を取得
      @param port 基板のポート番号
    */
    long getCount(int port) {
      return mNowEncData[port-1] - mEncData[port-1];
    }


    /*
      エンコーダの回転数の初期化
      @param port 基板のポート番号
    */

    void resetCount(int port){
      mOffsetEncData[port-1] = mNowEncData[port-1];
    }

};


#include <Wire.h>
#include"enc_board_mbed.h"

const int ADDR = 2;

EncoderBoardMbed enc(ADDR);

void setup() {

  Wire.begin();
  Wire.setClock(400000UL);
  Serial.begin(115200);

  enc.init();   //エンコーダ基盤の初期化
}

void loop() {

  //port1に接続されているエンコーダの値を表示
  Serial.println(enc.getCount(1));


  enc.Update();
}


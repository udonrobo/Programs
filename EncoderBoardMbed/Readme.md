# Mbed EncoderBoard

## Description

エンコーダの値をMbedで読み取り，ArduinoへI2C通信で送信

## Author
Watanabe Rui

## Usage

### Constructor
* EncoderBoardMbed(I2Cアドレス番号)

### Methods
* init()：エンコーダ基板のイニシャライズ。setup文に必ず記述。
* update()：エンコーダから送られてくる値の更新。
* getCount(ポート番号)：指定したポート番号に接続されているエンコーダの値を取得
* getCount(配列)：全てのポートのエンコーダの値を配列に渡す。
* resetCount(ポート番号)：指定したポート番号に接続されているエンコーダの値を0にリセット。
* resetCount()：全ポートのエンコーダの値を0にリセット。


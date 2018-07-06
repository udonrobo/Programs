# EncoderBoardMbed

## Description

エンコーダの値をMbedで読み取り，ArduinoへI2C通信で送信

## Author
Watanabe Rui

## Usage

### Constructor
* `EncoderBoardMbed([int]I2Cアドレス番号)`

### Methods
* `void init()`：エンコーダ基板のイニシャライズ。setup文に必ず記述。
* `void update()`：エンコーダから送られてくる値の更新。
* `long getCount([int]ポート番号)`：指定したポート番号に接続されているエンコーダの値を取得。
* `void getCount([int配列)`：全てのポートのエンコーダの値を配列に渡す。
* `void resetCount([int]ポート番号)`：指定したポート番号に接続されているエンコーダの値を0にリセット。
* `void resetCount()`：全ポートのエンコーダの値を0にリセット。
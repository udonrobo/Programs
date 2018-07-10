# I2CMasterWriter

## Description
I2C通信でマスターからスレーブにデータを送信するライブラリ。

## Author
Watanabe Rui

## Usage

### Constructor

* `I2CMasterWriter([int]I2Cアドレス番号,[int]送るデータの個数)`

### Methods

* `void update()`：データの送信。
* `void reset()`：データを0に初期化。
* `void setData([int]配列の添字,[byte]送信する値)`：送るデータを配列に格納。
* `void setBitData([int]配列の添字,[byte]書き込むビットの位置,[bool]1/0)`：配列の指定したビットの位置(LSBを0)にtrue/falseをセット。
* `void show()`：送信するデータの表示。

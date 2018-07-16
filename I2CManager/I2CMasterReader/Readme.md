# I2CMasterReader

## Description

masterがslaveに要求をしてslaveから送られてくる値を受けとる

## Author
Niki Ryota

## Usage

### Constructor
* `MasterReader([int]I2Cアドレス番号, [int]受け取るデータのサイズ)`

### Methods
* `void update()`：slaveから送られてくる値の更新.
* `byte getData([int]データの場所)`：指定した場所の値を取得.
* `bool getBitData([int]データの場所, [int], bitの場所)`：：指定した場所の値の指定したbitを取得.
* `void show()`受け取ったデータの表示.

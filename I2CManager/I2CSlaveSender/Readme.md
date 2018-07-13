# I2CSlaveSender

## Description

slaveからmastrに値を送る

## Author
Niki Ryota

## Usage

### Constructor
* `SlaveSender([int]送るデータのサイズ)`

### Methods
* ` void setData([int]データの場所)`：指定した場所に値を入れる.
* ` void setBitData([int]データの場所, [byte]bitの場所)`：指定した場所の指定したbitに値を入れる.
* `void reset()`：値のリセット
* `void update()`：値をmasterに送る.
* `void show()`masterに送るデータの表示.

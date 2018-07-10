# ControllerManagerDS4

## Description

DualShock4の情報を受信する基板からデータを受け取る

## Author
Watanabe Rui

## Usage

### Constructor

* `ControllerManagerDS4([int]I2Cアドレス番号)`

### Methods

* `void update()`：受信データの更新。
* ButtonData
    * `bool get***()`：***で指定した任意のボタンを押下している間trueを返す。
    * `bool get***Click()`：***で指定した任意のボタンを押下した瞬間trueを返す。
    * `bool get***Toggle()`：***で指定した任意のボタンを押したときtrue/falseを切り替える。

* AnalogButtonData
    * `int16_t getAnalog[R2/L2]()`：R2，L2ボタンのアナログ値を取得。

* JoyStick
    * `int16_t get[Left/Right]Hat[X/Y]()`：右/左ジョイスティックのX成分/Y成分を返す。
    * `int16_t get[Left/Right]Axis[X/Y]()`：ジョイスティックの勾配をX成分/Y成分に分解した値を返す。
    * `int16_t getThetaDeg()`：左ジョイスティックの角度を度数で返す。(X軸の正方向を0とし反時計回りに増加)
    * `int16_t getR()`：左ジョイスティックの勾配を取得。

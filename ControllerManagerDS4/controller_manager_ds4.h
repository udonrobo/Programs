/*
  @program  コントローラのデータ受信基板からデータを受け取るクラス
  @date   2018/06/26
  @author  Watanabe Rui

*/
#pragma once

#include <Wire.h>

constexpr int16_t CutAnalogValue = 20;  // アナログスティックの中間不感値
constexpr int16_t CutValue = CutAnalogValue * 2;

  struct buttonData {
  bool oldData = 0;
  bool newData = 0;
  };

class ControllerManagerDS4 {

  private:

    bool Select, L3, R3, START, PS, Up, Right, Down, Left;
    bool L2, R2, L1, R1, Triangle, Circle, Cross, Square;
    bool Touch;

    int16_t LeftHatX, LeftHatY;
    int16_t RightHatX, RightHatY;
    int16_t AnalogL2, AnalogR2;

    int16_t RightHatX_map, RightHatY_map;
    int16_t LeftHatX_map, LeftHatY_map;

    int16_t theta;
    uint16_t r;
    int16_t LeftxAxis, LeftyAxis, RightxAxis;

    buttonData buttonStock[17];
    bool Click_Data[17] = {};
    bool Toggle_Data[17] = {};

    int CountUp_Data[17] = {};

    const int I2C_Address;

  public:

    ControllerManager(int Address) : I2C_Address(Address) {}

    void Update() {
      Wire.requestFrom(I2C_Address, 9);
      uint8_t buf[] = { 0, 0, 127, 127, 127, 127, 0, 0, 0};
      while (Wire.available()) {
        buf[0] = Wire.read();
        buf[1] = Wire.read();
        buf[2] = Wire.read();
        buf[3] = Wire.read();
        buf[4] = Wire.read();
        buf[5] = Wire.read();
        buf[6] = Wire.read();
        buf[7] = Wire.read();
        buf[8] = Wire.read();
      }
      Select = bitRead(buf[0], 0);
      L3 = bitRead(buf[0], 1);
      R3 = bitRead(buf[0], 2);
      START = bitRead(buf[0], 3);
      Up = bitRead(buf[0], 4);
      Right = bitRead(buf[0], 5);
      Down = bitRead(buf[0], 6);
      Left = bitRead(buf[0], 7);

      L2 = bitRead(buf[1], 0);
      R2 = bitRead(buf[1], 1);
      L1 = bitRead(buf[1], 2);
      R1 = bitRead(buf[1], 3);
      Triangle = bitRead(buf[1], 4);
      Circle = bitRead(buf[1], 5);
      Cross = bitRead(buf[1], 6);
      Square = bitRead(buf[1], 7);

      PS = bitRead(buf[8], 0);
      Touch = bitRead(buf[8], 1);

      buttonStock[0].newData = Triangle;
      buttonStock[1].newData = Circle;
      buttonStock[2].newData = Cross;
      buttonStock[3].newData = Square;

      buttonStock[4].newData = Up;
      buttonStock[5].newData = Right;
      buttonStock[6].newData = Down;
      buttonStock[7].newData = Left;

      buttonStock[8].newData = START;
      buttonStock[9].newData = Select;

      buttonStock[10].newData = Touch;

      buttonStock[11].newData = L2;
      buttonStock[12].newData = R2;
      buttonStock[13].newData = L1;
      buttonStock[14].newData = R1;
      buttonStock[15].newData = L3;
      buttonStock[16].newData = R3;

      for (int i = 0; i < 17; i++) {
        if (buttonStock[i].oldData == 0 && buttonStock[i].newData == 1) {
          Click_Data[i] = true;
        }
        else
          Click_Data[i] = false;

        buttonStock[i].oldData = buttonStock[i].newData;
      }

      for (int i = 0; i < 17; i++)
        if (Click_Data[i])   Toggle_Data[i] ^= 1;

      for (int i = 0; i < 17; i++)
        if (Click_Data[i])       CountUp_Data[i]++;

      if (PS) {
        for (int i = 0; i < 17; i++) {
          CountUp_Data[i] = 0;
        }
      }

      RightHatX_map = buf[2] - 127;
      RightHatY_map = -(buf[3] - 127);
      LeftHatX_map = buf[4] - 127;
      LeftHatY_map = -(buf[5] - 127);

      RightHatX = buf[2];
      RightHatY = buf[3];
      LeftHatX = buf[4];
      LeftHatY = buf[5];
      AnalogL2 = buf[6];
      AnalogR2 = buf[7];

      r = sqrt((LeftHatX_map * LeftHatX_map) + (LeftHatY_map * LeftHatY_map)) * 2;
      r = constrain(r, 0, 255);

      float Rad = atan2(LeftHatY_map, LeftHatX_map);
      theta = round(degrees(Rad));
      if (theta < 0) theta += 360;
      if (theta > 359) theta -= 360;

    }
    bool GetTouch() {
      return Touch;
    }
    bool GetPS() {
      return PS;
    }
    bool GetStart() {
      return START;
    }
    bool GetSelect() {
      return Select;
    }
    bool GetL3() {
      return L3;
    }
    bool GetR3() {
      return R3;
    }
    bool GetUp() {
      return Up;
    }
    bool GetRight() {
      return Right;
    }
    bool GetDown() {
      return Down;
    }
    bool GetLeft() {
      return Left;
    }
    bool GetL2() {
      return L2;
    }
    bool GetR2() {
      return R2;
    }
    bool GetL1() {
      return L1;
    }
    bool GetR1() {
      return R1;
    }
    bool GetTriangle() {
      return Triangle;
    }
    bool GetCircle() {
      return Circle;
    }
    bool GetCross() {
      return Cross;
    }
    bool GetSquare() {
      return Square;
    }
    /* Click */
    bool GetTriangleClick() {
      return Click_Data[0];
    }
    bool GetCircleClick() {
      return Click_Data[1];
    }
    bool GetCrossClick() {
      return Click_Data[2];
    }
    bool GetSquareClick() {
      return Click_Data[3];
    }

    bool GetUpClick() {
      return Click_Data[4];
    }
    bool GetRightClick() {
      return Click_Data[5];
    }
    bool GetDownClick() {
      return Click_Data[6];
    }
    bool GetLeftClick() {
      return Click_Data[7];
    }

    bool GetStartClick() {
      return Click_Data[8];
    }
    bool GetShareClick() {
      return Click_Data[9];
    }
    bool GetTouchClick() {
      return Click_Data[10];
    }

    bool GetL2Click() {
      return Click_Data[11];
    }
    bool GetR2Click() {
      return Click_Data[12];
    }
    bool GetL1Click() {
      return Click_Data[13];
    }
    bool GetR1Click() {
      return Click_Data[14];
    }
    bool GetL3Click() {
      return Click_Data[15];
    }
    bool GetR3Click() {
      return Click_Data[16];
    }

    /*    Toggle    */
    bool GetTriangleToggle() {
      return Toggle_Data[0];
    }
    bool GetCircleToggle() {
      return Toggle_Data[1];
    }
    bool GetCrossToggle() {
      return Toggle_Data[2];
    }
    bool GetSquareToggle() {
      return Toggle_Data[3];
    }

    bool GetUpToggle() {
      return Toggle_Data[4];
    }
    bool GetRightToggle() {
      return Toggle_Data[5];
    }
    bool GetDownToggle() {
      return Toggle_Data[6];
    }
    bool GetLeftToggle() {
      return Toggle_Data[7];
    }

    bool GetStartToggle() {
      return Toggle_Data[8];
    }
    bool GetShareToggle() {
      return Toggle_Data[9];
    }
    bool GetTouchToggle() {
      return Toggle_Data[10];
    }

    bool GetL2Toggle() {
      return Toggle_Data[11];
    }
    bool GetR2Toggle() {
      return Toggle_Data[12];
    }
    bool GetL1Toggle() {
      return Toggle_Data[13];
    }
    bool GetR1Toggle() {
      return Toggle_Data[14];
    }
    bool GetL3Toggle() {
      return Toggle_Data[15];
    }
    bool GetR3Toggle() {
      return Toggle_Data[16];
    }

    /*      CountUp     */
    int GetTriangleCountUp() {
      return CountUp_Data[0];
    }
    int GetCircleCountUp() {
      return CountUp_Data[1];
    }
    int GetCrossCountUp() {
      return CountUp_Data[2];
    }
    int GetSquareCountUp() {
      return CountUp_Data[3];
    }

    int GetUpCountUp() {
      return CountUp_Data[4];
    }
    int GetRightCountUp() {
      return CountUp_Data[5];
    }
    int GetDownCountUp() {
      return CountUp_Data[6];
    }
    int GetLeftCountUp() {
      return CountUp_Data[7];
    }

    int GetStartCountUp() {
      return CountUp_Data[8];
    }
    int GetShareCountUp() {
      return CountUp_Data[9];
    }
    int GetTouchCountUp() {
      return CountUp_Data[10];
    }

    int GetL2CountUp() {
      return CountUp_Data[11];
    }
    int GetR2CountUp() {
      return CountUp_Data[12];
    }
    int GetL1CountUp() {
      return CountUp_Data[13];
    }
    int GetR1CountUp() {
      return CountUp_Data[14];
    }
    int GetL3CountUp() {
      return CountUp_Data[15];
    }
    int GetR3CountUp() {
      return CountUp_Data[16];
    }

    int16_t GetLeftHatX() {
      return LeftHatX;
    }
    int16_t GetLeftHatY() {
      return LeftHatY;
    }
    int16_t GetRightHatX() {
      return RightHatX;
    }
    int16_t GetRightHatY() {
      return RightHatY;
    }
    int16_t GetAnalogL2() {
      return AnalogL2;
    }
    int16_t GetAnalogR2() {
      return AnalogR2;
    }

    int16_t Gettheta() {
      return theta;
    }
    uint16_t Getr() {
      return r;
    }
    int16_t GetLeftxAxis() {
      return LeftxAxis;
    }
    int16_t GetLeftyAxis() {
      return LeftyAxis;
    }
    int16_t GetRightxAxis() {
      return RightxAxis;
    }
};
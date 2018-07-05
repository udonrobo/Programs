/*
  @program  コントローラのデータ受信基板からデータを受け取るクラス
  @date   2018/06/26
  @author  Watanabe Rui

*/
#pragma once

#include <Wire.h>

constexpr int16_t cutAnalogValue = 20;  // アナログスティックの中間不感値
constexpr int16_t cutValue = cutAnalogValue * 2;

  struct buttonData {
  bool oldData = 0;
  bool newData = 0;
  };

class ControllerManagerDS4 {

  private:

    bool Select, L3, R3, START, PS, Up, Right, Down, Left;
    bool L2, R2, L1, R1, Triangle, Circle, Cross, Square;
    bool Touch;

    int16_t leftHatX, leftHatY;
    int16_t rightHatX, rightHatY;
    int16_t AnalogL2, AnalogR2;

    int16_t rightHatX_map, rightHatY_map;
    int16_t leftHatX_map, leftHatY_map;

    int16_t theta;
    uint16_t r;
    int16_t LeftxAxis, LeftyAxis, RightxAxis;

    buttonData buttonStock[17];
    bool clickData[17] = {};
    bool toggleData[17] = {};

    int countUpData[17] = {};

    const int I2C_ADDR;

  public:

    ControllerManager(int ADDR) : I2C_ADDR(ADDR) {}

    void Update() {
      Wire.requestFrom(I2C_ADDR, 9);
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
          clickData[i] = true;
        }
        else
          clickData[i] = false;

        buttonStock[i].oldData = buttonStock[i].newData;
      }

      for (int i = 0; i < 17; i++)
        if (clickData[i])   toggleData[i] ^= 1;

      for (int i = 0; i < 17; i++)
        if (clickData[i])       countUpData[i]++;

      if (PS) {
        for (int i = 0; i < 17; i++) {
          countUpData[i] = 0;
        }
      }

      rightHatX_map = buf[2] - 127;
      rightHatY_map = -(buf[3] - 127);
      leftHatX_map = buf[4] - 127;
      leftHatY_map = -(buf[5] - 127);

      rightHatX = buf[2];
      rightHatY = buf[3];
      leftHatX = buf[4];
      leftHatY = buf[5];
      AnalogL2 = buf[6];
      AnalogR2 = buf[7];

      r = sqrt((leftHatX_map * leftHatX_map) + (leftHatY_map * leftHatY_map)) * 2;
      r = constrain(r, 0, 255);

      double rad = atan2(leftHatY_map, leftHatX_map);
      theta = round(degrees(rad));
      if (theta < 0) theta += 360;
      if (theta > 359) theta -= 360;

    }
    bool getTouch() {
      return Touch;
    }
    bool getPS() {
      return PS;
    }
    bool getStart() {
      return START;
    }
    bool getSelect() {
      return Select;
    }
    bool getL3() {
      return L3;
    }
    bool getR3() {
      return R3;
    }
    bool getUp() {
      return Up;
    }
    bool getRight() {
      return Right;
    }
    bool getDown() {
      return Down;
    }
    bool getLeft() {
      return Left;
    }
    bool getL2() {
      return L2;
    }
    bool getR2() {
      return R2;
    }
    bool getL1() {
      return L1;
    }
    bool getR1() {
      return R1;
    }
    bool getTriangle() {
      return Triangle;
    }
    bool getCircle() {
      return Circle;
    }
    bool getCross() {
      return Cross;
    }
    bool getSquare() {
      return Square;
    }
    /* Click */
    bool getTriangleClick() {
      return clickData[0];
    }
    bool getCircleClick() {
      return clickData[1];
    }
    bool getCrossClick() {
      return clickData[2];
    }
    bool getSquareClick() {
      return clickData[3];
    }

    bool getUpClick() {
      return clickData[4];
    }
    bool getRightClick() {
      return clickData[5];
    }
    bool getDownClick() {
      return clickData[6];
    }
    bool getLeftClick() {
      return clickData[7];
    }

    bool getStartClick() {
      return clickData[8];
    }
    bool getShareClick() {
      return clickData[9];
    }
    bool getTouchClick() {
      return clickData[10];
    }

    bool getL2Click() {
      return clickData[11];
    }
    bool getR2Click() {
      return clickData[12];
    }
    bool getL1Click() {
      return clickData[13];
    }
    bool getR1Click() {
      return clickData[14];
    }
    bool getL3Click() {
      return clickData[15];
    }
    bool getR3Click() {
      return clickData[16];
    }

    /*    Toggle    */
    bool getTriangleToggle() {
      return toggleData[0];
    }
    bool getCircleToggle() {
      return toggleData[1];
    }
    bool getCrossToggle() {
      return toggleData[2];
    }
    bool getSquareToggle() {
      return toggleData[3];
    }

    bool getUpToggle() {
      return toggleData[4];
    }
    bool getRightToggle() {
      return toggleData[5];
    }
    bool getDownToggle() {
      return toggleData[6];
    }
    bool getLeftToggle() {
      return toggleData[7];
    }

    bool getStartToggle() {
      return toggleData[8];
    }
    bool getShareToggle() {
      return toggleData[9];
    }
    bool getTouchToggle() {
      return toggleData[10];
    }

    bool getL2Toggle() {
      return toggleData[11];
    }
    bool getR2Toggle() {
      return toggleData[12];
    }
    bool getL1Toggle() {
      return toggleData[13];
    }
    bool getR1Toggle() {
      return toggleData[14];
    }
    bool getL3Toggle() {
      return toggleData[15];
    }
    bool getR3Toggle() {
      return toggleData[16];
    }

    /*      CountUp     */
    int getTriangleCountUp() {
      return countUpData[0];
    }
    int getCircleCountUp() {
      return countUpData[1];
    }
    int getCrossCountUp() {
      return countUpData[2];
    }
    int getSquareCountUp() {
      return countUpData[3];
    }

    int getUpCountUp() {
      return countUpData[4];
    }
    int getRightCountUp() {
      return countUpData[5];
    }
    int getDownCountUp() {
      return countUpData[6];
    }
    int getLeftCountUp() {
      return countUpData[7];
    }

    int getStartCountUp() {
      return countUpData[8];
    }
    int getShareCountUp() {
      return countUpData[9];
    }
    int getTouchCountUp() {
      return countUpData[10];
    }

    int getL2CountUp() {
      return countUpData[11];
    }
    int getR2CountUp() {
      return countUpData[12];
    }
    int getL1CountUp() {
      return countUpData[13];
    }
    int getR1CountUp() {
      return countUpData[14];
    }
    int getL3CountUp() {
      return countUpData[15];
    }
    int getR3CountUp() {
      return countUpData[16];
    }

    int16_t getleftHatX() {
      return leftHatX;
    }
    int16_t getleftHatY() {
      return leftHatY;
    }
    int16_t getrightHatX() {
      return rightHatX;
    }
    int16_t getrightHatY() {
      return rightHatY;
    }
    int16_t getAnalogL2() {
      return AnalogL2;
    }
    int16_t getAnalogR2() {
      return AnalogR2;
    }

    int16_t gettheta() {
      return theta;
    }
    uint16_t getr() {
      return r;
    }
    int16_t getLeftxAxis() {
      return LeftxAxis;
    }
    int16_t getLeftyAxis() {
      return LeftyAxis;
    }
    int16_t getRightxAxis() {
      return RightxAxis;
    }
};
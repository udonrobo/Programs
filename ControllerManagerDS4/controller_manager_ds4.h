/*
  @program  コントローラのデータ受信基板からデータを受け取るクラス
  @date   2018/06/26
  @author  Watanabe Rui

*/
#pragma once

#include <Wire.h>

constexpr int16_t cutAnalogValue = 20; // アナログスティックの閾値
constexpr int16_t cutValue = cutAnalogValue * 2;

//クリック・トグル用
struct buttonData
{
  bool oldData = 0;
  bool newData = 0;
};

class ControllerManagerDS4
{

private:

  bool select, l3, r3, start, ps, up, right, down, left;
  bool l2, r2, l1, r1, triangle, circle, cross, square;
  bool touch; //タッチパッド

  int16_t leftHatX, leftHatY;
  int16_t rightHatX, rightHatY;
  int16_t analogL2, analogR2;

  int16_t mappedRightHatX, mappedRightHatY;
  int16_t mappedLeftHatX, mappedLeftHatY;

  int16_t theta;
  uint16_t r;
  int16_t leftAxisX, leftAxisY, rightAxisX;

  buttonData buttonStock[17];

  bool clickData[17] = {};
  bool toggleData[17] = {};

  int countUpData[17] = {};

  const int I2C_ADDR;

public:

  /*
    コンストラクタ
    @param ADDR：I2Cアドレス番号
  */
  ControllerManagerDS4(int ADDR) : I2C_ADDR(ADDR) {}


  /*  データの更新  */
  void update()
  {
    uint8_t buf[] = {0, 0, 127, 127, 127, 127, 0, 0, 0};

    //データ受け取り
    Wire.requestFrom(I2C_ADDR, 9);
    while (Wire.available())
    {
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

    select = bitRead(buf[0], 0);
    l3 = bitRead(buf[0], 1);
    r3 = bitRead(buf[0], 2);
    start = bitRead(buf[0], 3);
    up = bitRead(buf[0], 4);
    right = bitRead(buf[0], 5);
    down = bitRead(buf[0], 6);
    left = bitRead(buf[0], 7);

    l2 = bitRead(buf[1], 0);
    r2 = bitRead(buf[1], 1);
    l1 = bitRead(buf[1], 2);
    r1 = bitRead(buf[1], 3);
    triangle = bitRead(buf[1], 4);
    circle = bitRead(buf[1], 5);
    cross = bitRead(buf[1], 6);
    square = bitRead(buf[1], 7);

    ps = bitRead(buf[8], 0);
    touch = bitRead(buf[8], 1);

    buttonStock[0].newData = triangle;
    buttonStock[1].newData = circle;
    buttonStock[2].newData = cross;
    buttonStock[3].newData = square;

    buttonStock[4].newData = up;
    buttonStock[5].newData = right;
    buttonStock[6].newData = down;
    buttonStock[7].newData = left;

    buttonStock[8].newData = start;
    buttonStock[9].newData = select;

    buttonStock[10].newData = touch;

    buttonStock[11].newData = l2;
    buttonStock[12].newData = r2;
    buttonStock[13].newData = l1;
    buttonStock[14].newData = r1;
    buttonStock[15].newData = l3;
    buttonStock[16].newData = r3;

    //クリック判定
    for (int i = 0; i < 17; i++)
    {
      if (buttonStock[i].oldData == 0 && buttonStock[i].newData == 1)
      {
        clickData[i] = true;
      }
      else
        clickData[i] = false;

      buttonStock[i].oldData = buttonStock[i].newData;
    }

    //トグル判定
    for (int i = 0; i < 17; i++)
      if (clickData[i])
        toggleData[i] ^= 1;

    mappedRightHatX =   buf[2] - 127;
    mappedRightHatY = -(buf[3] - 127);
    mappedLeftHatX  =   buf[4] - 127;
    mappedLeftHatY  = -(buf[5] - 127);

    rightHatX = buf[2];
    rightHatY = buf[3];
    leftHatX = buf[4];
    leftHatY = buf[5];
    analogL2 = buf[6];
    analogR2 = buf[7];

    r = sqrt((mappedLeftHatX * mappedLeftHatX) + (mappedLeftHatY * mappedLeftHatY)) * 2;
    r = constrain(r, 0, 255);

    double rad = atan2(mappedLeftHatY, mappedLeftHatX);
    theta = round(degrees(rad));

    if (theta < 0)
      theta += 360;
    if (theta > 359)
      theta -= 360;
  }

  bool getTouch()
  {
    return touch;
  }
  bool getPS()
  {
    return ps;
  }
  bool getStart()
  {
    return start;
  }
  bool getSelect()
  {
    return select;
  }
  bool getL3()
  {
    return l3;
  }
  bool getR3()
  {
    return r3;
  }
  bool getUp()
  {
    return up;
  }
  bool getRight()
  {
    return right;
  }
  bool getDown()
  {
    return down;
  }
  bool getLeft()
  {
    return left;
  }
  bool getL2()
  {
    return l2;
  }
  bool getR2()
  {
    return r2;
  }
  bool getL1()
  {
    return l1;
  }
  bool getR1()
  {
    return r1;
  }
  bool getTriangle()
  {
    return triangle;
  }
  bool getCircle()
  {
    return circle;
  }
  bool getCross()
  {
    return cross;
  }
  bool getSquare()
  {
    return square;
  }

  /* Click */
  bool getTriangleClick()
  {
    return clickData[0];
  }
  bool getCircleClick()
  {
    return clickData[1];
  }
  bool getCrossClick()
  {
    return clickData[2];
  }
  bool getSquareClick()
  {
    return clickData[3];
  }

  bool getUpClick()
  {
    return clickData[4];
  }
  bool getRightClick()
  {
    return clickData[5];
  }
  bool getDownClick()
  {
    return clickData[6];
  }
  bool getLeftClick()
  {
    return clickData[7];
  }

  bool getStartClick()
  {
    return clickData[8];
  }
  bool getShareClick()
  {
    return clickData[9];
  }
  bool getTouchClick()
  {
    return clickData[10];
  }

  bool getL2Click()
  {
    return clickData[11];
  }
  bool getR2Click()
  {
    return clickData[12];
  }
  bool getL1Click()
  {
    return clickData[13];
  }
  bool getR1Click()
  {
    return clickData[14];
  }
  bool getL3Click()
  {
    return clickData[15];
  }
  bool getR3Click()
  {
    return clickData[16];
  }

  /*    Toggle    */
  bool getTriangleToggle()
  {
    return toggleData[0];
  }
  bool getCircleToggle()
  {
    return toggleData[1];
  }
  bool getCrossToggle()
  {
    return toggleData[2];
  }
  bool getSquareToggle()
  {
    return toggleData[3];
  }

  bool getUpToggle()
  {
    return toggleData[4];
  }
  bool getRightToggle()
  {
    return toggleData[5];
  }
  bool getDownToggle()
  {
    return toggleData[6];
  }
  bool getLeftToggle()
  {
    return toggleData[7];
  }

  bool getStartToggle()
  {
    return toggleData[8];
  }
  bool getShareToggle()
  {
    return toggleData[9];
  }
  bool getTouchToggle()
  {
    return toggleData[10];
  }

  bool getL2Toggle()
  {
    return toggleData[11];
  }
  bool getR2Toggle()
  {
    return toggleData[12];
  }
  bool getL1Toggle()
  {
    return toggleData[13];
  }
  bool getR1Toggle()
  {
    return toggleData[14];
  }
  bool getL3Toggle()
  {
    return toggleData[15];
  }
  bool getR3Toggle()
  {
    return toggleData[16];
  }

  int16_t getleftHatX()
  {
    return leftHatX;
  }
  int16_t getleftHatY()
  {
    return leftHatY;
  }
  int16_t getrightHatX()
  {
    return rightHatX;
  }
  int16_t getrightHatY()
  {
    return rightHatY;
  }

  int16_t getAnalogL2()
  {
    return analogL2;
  }
  int16_t getAnalogR2()
  {
    return analogR2;
  }

  int16_t getThetaDeg()
  {
    return theta;
  }
  uint16_t getR()
  {
    return r;
  }
  int16_t getLeftAxisX()
  {
    return leftAxisX;
  }
  int16_t getLeftAxisY()
  {
    return leftAxisY;
  }
  int16_t getRightAxisX()
  {
    return rightAxisX;
  }
};

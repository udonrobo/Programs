/*
  @Description    オムニ・メカナムホイールの制御ライブラリ
  @Date           2018/01/06
  @Version        1.0.1
  @Rewriter       Rui Watanabe
*/

#pragma once

#include "moving_average.h"

//pin assign
const int M1A = 2;
const int M1B = 4;
const int M1PWM = 3;

const int M2A = 6;
const int M2B = 5;
const int M2PWM = 9;

const int M3A = 12;
const int M3B = A0;
const int M3PWM = 11;

const int M4A = 8;
const int M4B = 7;
const int M4PWM = 10;

const int BORDER_VALUE = 20;         //ジョイスティックの閾値
const double TURN_RATIO = 0.30;      //旋回速度の減衰率
const double LOW_SPEED_RATIO = 0.25; //低速モードの減衰率

class WheelController
{

private:
  const int M_MAXPOWER;
  double mSlope;
  double mAngle;
  double mMovingAxisX, mMovingAxisY, mTurnAxisX;
  double mRad;
  int mTheta;

  //移動平均用インスタンス
  MovingAverage average1;
  MovingAverage average2;
  MovingAverage average3;
  MovingAverage average4;

public:
  /*
        コンストラクタ
        @param 　MAXPOWER　  PWM出力の最大値
        @param 　average   　 移動平均するデータ数
    */

  WheelController(const int MAXPOWER, const int AVERAGE_SIZE) : average1(AVERAGE_SIZE), average2(AVERAGE_SIZE), average3(AVERAGE_SIZE), average4(AVERAGE_SIZE), M_MAXPOWER(min(MAXPOWER, 255))
  {

    pinMode(M1A, OUTPUT);
    pinMode(M1B, OUTPUT);
    pinMode(M2A, OUTPUT);
    pinMode(M2B, OUTPUT);
    pinMode(M3A, OUTPUT);
    pinMode(M3B, OUTPUT);
    pinMode(M4A, OUTPUT);
    pinMode(M4B, OUTPUT);
  }

  /*
       コンストラクタ　(1輪ずつ出力値を調整する場合)
       @param average   移動平均するデータ数   
    */
  WheelController(const int AVERAGE_SIZE) : average1(AVERAGE_SIZE), average2(AVERAGE_SIZE), average3(AVERAGE_SIZE), average4(AVERAGE_SIZE)
  {

    pinMode(M1A, OUTPUT);
    pinMode(M1B, OUTPUT);
    pinMode(M2A, OUTPUT);
    pinMode(M2B, OUTPUT);
    pinMode(M3A, OUTPUT);
    pinMode(M3B, OUTPUT);
    pinMode(M4A, OUTPUT);
    pinMode(M4B, OUTPUT);
  }

  //PWM周波数の変更
  void setPWMFrenquency()
  {
    TCCR1B = (TCCR1B & 0b11111000) | 0x02;
    TCCR2B = (TCCR2B & 0b11111000) | 0x02;
  }

  //スティックの傾きを取得
  double getStickSlope(double xAxis, double yAxis)
  {

    //勾配を求めた後0~MAXPOWERの範囲で切り取り
    double Slope = constrain(sqrt(pow(yAxis, 2) + pow(xAxis, 2)), 0, M_MAXPOWER);

    return Slope;
  }

  //スティックの角度を取得
  double getAngle(double xAxis, double yAxis)
  {

    double angle = atan2(yAxis, xAxis);

    //0~2πまでとるように補正
    if (angle < 0)
      angle += 2 * PI;

    return angle;
  }

  /*
       制御に用いるジョイスティックの軸を設定

       @param  movingAxisX  　移動軸X
       @param  movingAxisY  　移動軸Y
       @param  turnAxisX    　旋回軸X
       @param  isLowSpeed 　低速モード On/Off
       @param  isReverseMode    反転モード On/Off
    */

  void SetAxis(int16_t movingAxisX, int16_t movingAxisY, int16_t turnAxisX, bool isLowSpeed, bool isReverseMode)
  {
    //メンバ変数に格納
    mMovingAxisX = movingAxisX;
    mMovingAxisY = movingAxisY;
    mTurnAxisX = turnAxisX;

    //低速モード
    if (isLowSpeed)
    {
      mMovingAxisX = -map(mMovingAxisX, 0, 255, -M_MAXPOWER * LOW_SPEED_RATIO, M_MAXPOWER * LOW_SPEED_RATIO);
      mMovingAxisY = -map(mMovingAxisY, 255, 0, -M_MAXPOWER * LOW_SPEED_RATIO, M_MAXPOWER * LOW_SPEED_RATIO);
    }
    else
    {
      mMovingAxisX = -map(mMovingAxisX, 0, 255, -M_MAXPOWER, M_MAXPOWER);
      mMovingAxisY = -map(mMovingAxisY, 255, 0, -M_MAXPOWER, M_MAXPOWER);
    }

    mTurnAxisX = -map(mTurnAxisX, 0, 255, -M_MAXPOWER, M_MAXPOWER);

    if (abs(mTurnAxisX) < BORDER_VALUE)
      mTurnAxisX = 0;
    else
      mTurnAxisX = map(mTurnAxisX, -M_MAXPOWER, M_MAXPOWER, M_MAXPOWER * TURN_RATIO, -M_MAXPOWER * TURN_RATIO);

    //反転モード
    if (isReverseMode)
    {
      mMovingAxisX = -mMovingAxisX;
      mMovingAxisY = -mMovingAxisY;
      mTurnAxisX = -mTurnAxisX;
    }
  }

  /*
         制御に用いるジョイスティックの軸を設定[曲座標方式]

         @param   slope          ジョイスティックの移動軸の傾き
         @param   mTheta          ジョイスティックの移動軸の角度
         @param   turnAxisX     旋回軸
         @param   isLowSpeed  低速モード
         @param   reverse_mod    反転モード
    */

  //[fix] 汚いので直す
  void SetPolarAxis(int16_t slope, int16_t mTheta, int16_t turnAxisX, bool isLowSpeed, bool isReverseMode)
  {
    mSlope = slope;
    mAngle = mTheta;
    mTurnAxisX = turnAxisX;

    mTurnAxisX = -map(mTurnAxisX, 0, 255, -M_MAXPOWER, M_MAXPOWER);
    /*
            if (abs(mTurnAxisX) < BORDER_VALUE) {
              mTurnAxisX = 0;
            }
            else
      */
    mTurnAxisX = map(mTurnAxisX, -M_MAXPOWER, M_MAXPOWER, M_MAXPOWER * TURN_RATIO, -M_MAXPOWER * TURN_RATIO);

    if (isLowSpeed)
      mSlope /= 3;
    if (isReverseMode)
    {
      mAngle += 180;
      if (mAngle > 359)
        mAngle -= 360;
    }
  }

  /*
        三角関数によるモータの出力計算

        @param  Power  モータ出力値(配列)
  */

  void Set4WheelPower(int16_t Power[])
  {
    
    //傾きと角度を取得しメンバ変数へ格納
    mSlope = getStickSlope(mMovingAxisX, mMovingAxisY);
    mAngle = getAngle(mMovingAxisX, mMovingAxisY);

    if (mSlope < BORDER_VALUE)
    {
      Power[0] = Power[1] = Power[2] = Power[3] = 0;
    }
    else
    {

      //閾値からを0に変換
      mSlope = map(mSlope, BORDER_VALUE, M_MAXPOWER, 0, M_MAXPOWER);

      //タイヤの設置角度
      Power[0] = mSlope * sin(mAngle - (PI / 6));
      Power[1] = mSlope * sin(mAngle - (5 * PI / 6));
      Power[2] = mSlope * sin(mAngle - (7 * PI / 6));
      Power[3] = mSlope * sin(mAngle - (11 * PI / 6));
    }

    //Powerの最大値を取得
    double max = abs(Power[0]);

    for (int i = 1; i < 4; i++)
    {
      if (abs(Power[i]) > max)
        max = abs(Power[i]);
    }

    double ratio; //補正倍率
    //最大値とスティックの傾きとの比を導出
    if (max > BORDER_VALUE)
      ratio = mSlope / max;

    //Powerを補正
    for (int i = 0; i < 4; i++)
      Power[i] *= ratio;

    //旋回成分を加算
    Power[0] += mTurnAxisX;
    Power[1] += mTurnAxisX;
    Power[2] += mTurnAxisX;
    Power[3] += mTurnAxisX;

    //Powerの最大値を取得
    max = abs(Power[0]);

    for (int i = 1; i < 4; i++)
    {
      if (abs(Power[i]) > max)
        max = abs(Power[i]);
    }

    //範囲外であれば範囲内(M_MAXPOWERまでの値)に収める
    if (max > M_MAXPOWER)
    {
      float amp = float(M_MAXPOWER) / max;
      Power[0] = int16_t(Power[0] * amp);
      Power[1] = int16_t(Power[1] * amp);
      Power[2] = int16_t(Power[2] * amp);
      Power[3] = int16_t(Power[3] * amp);
    }

    //移動平均
    average1.Update(Power[0]);
    average2.Update(Power[1]);
    average3.Update(Power[2]);
    average4.Update(Power[3]);

    Power[0] = average1.GetValue();
    Power[1] = average2.GetValue();
    Power[2] = average3.GetValue();
    Power[3] = average4.GetValue();
  }

  /*
           動きの設定   極座標形式
           4輪オムニ(メカナム)用

           @param Power  モータ出力値
    */
  void SetPolar4Vector(int16_t Power[])
  {

    int16_t m1_deg = mAngle - 30;
    if (m1_deg < 0)
      m1_deg = m1_deg + 360;
    else if (m1_deg > 359)
      m1_deg = m1_deg - 360;

    int16_t m2_deg = mAngle - 150;
    if (m2_deg < 0)
      m2_deg = m2_deg + 360;
    else if (m2_deg > 359)
      m2_deg = m2_deg - 360;
    /*
            if (mSlope < BORDER_VALUE)
              mSlope = 0;
            else {
      */
    // mSlope = map(mSlope, BORDER_VALUE, M_MAXPOWER, 0, M_MAXPOWER);

    Power[0] = -(sin(mRadians((float)m1_deg)) * mSlope);
    Power[1] = -(sin(mRadians((float)m2_deg)) * mSlope);
    Power[2] = -Power[0];
    Power[3] = -Power[1];
    //}

    /*
            Serial.print(Power[0]); Serial.print("\t");
            Serial.print(Power[1]); Serial.print("\t");
            Serial.print(Power[2]); Serial.print("\t");
            Serial.print(Power[3]); Serial.print("\t");
      */

    //一番大きい値の取得
    //int16_t max = max(max(abs(Power[0]), abs(Power[1])), max(abs(Power[2]), abs(Power[3])));

    double max = abs(Power[0]);

    for (int i = 1; i < 4; i++)
    {
      if (abs(Power[i]) > max)
        max = abs(Power[i]);
    }

    double ratio; //補正倍率
    if (max != 0)
      ratio = mSlope / max;

    for (int i = 0; i < 4; i++)
      Power[i] *= ratio;

    Power[0] += mTurnAxisX;
    Power[1] += mTurnAxisX;
    Power[2] += mTurnAxisX;
    Power[3] += mTurnAxisX;

    max = abs(Power[0]);

    for (int i = 1; i < 4; i++)
    {
      if (abs(Power[i]) > max)
        max = abs(Power[i]);
    }

    //範囲外であれば範囲内(M_MAXPOWERまでの値)に収める
    if (max > M_MAXPOWER)
    {
      float amp = float(M_MAXPOWER) / max;
      Power[0] = int16_t(Power[0] * amp);
      Power[1] = int16_t(Power[1] * amp);
      Power[2] = int16_t(Power[2] * amp);
      Power[3] = int16_t(Power[3] * amp);
    }

    average1.Update(Power[0]);
    average2.Update(Power[1]);
    average3.Update(Power[2]);
    average4.Update(Power[3]);

    Power[0] = average1.GetValue();
    Power[1] = average2.GetValue();
    Power[2] = average3.GetValue();
    Power[3] = average4.GetValue();
  }


  /*

    モータを駆動
    @param  motorPower(配列)  導出したモータの出力値

  */

  void drive(int16_t motorPower[])
  {
    //Motor1
    if (motorPower[0] == 0)
    {
      digitalWrite(M1A, HIGH);
      digitalWrite(M1B, HIGH);
      analogWrite(M1PWM, 0);
    }
    else if (motorPower[0] > 0)
    {
      digitalWrite(M1A, HIGH);
      digitalWrite(M1B, LOW);
      analogWrite(M1PWM, motorPower[0]);
    }
    else
    {
      digitalWrite(M1A, LOW);
      digitalWrite(M1B, HIGH);
      analogWrite(M1PWM, abs(motorPower[0]));
    }

    //Motor2
    if (motorPower[1] == 0)
    {
      digitalWrite(M2A, HIGH);
      digitalWrite(M2B, HIGH);
      analogWrite(M2PWM, 0);
    }
    else if (motorPower[1] > 0)
    {
      digitalWrite(M2A, HIGH);
      digitalWrite(M2B, LOW);
      analogWrite(M2PWM, motorPower[1]);
    }
    else
    {
      digitalWrite(M2A, LOW);
      digitalWrite(M2B, HIGH);
      analogWrite(M2PWM, abs(motorPower[1]));
    }

    //Motor3
    if (motorPower[2] == 0)
    {
      digitalWrite(M3A, HIGH);
      digitalWrite(M3B, HIGH);
      analogWrite(M3PWM, 0);
    }
    else if (motorPower[2] > 0)
    {
      digitalWrite(M3A, HIGH);
      digitalWrite(M3B, LOW);
      analogWrite(M3PWM, motorPower[2]);
    }
    else
    {
      digitalWrite(M3A, LOW);
      digitalWrite(M3B, HIGH);
      analogWrite(M3PWM, abs(motorPower[2]));
    }

    //Motor4
    if (motorPower[3] == 0)
    {
      digitalWrite(M4A, HIGH);
      digitalWrite(M4B, HIGH);
      analogWrite(M4PWM, 0);
    }
    else if (motorPower[3] > 0)
    {
      digitalWrite(M4A, HIGH);
      digitalWrite(M4B, LOW);
      analogWrite(M4PWM, motorPower[3]);
    }
    else
    {
      digitalWrite(M4A, LOW);
      digitalWrite(M4B, HIGH);
      analogWrite(M4PWM, abs(motorPower[3]));
    }
  }
};

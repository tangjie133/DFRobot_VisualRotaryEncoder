/*!
 * @file        setSensor.ino
 * @brief       这个demo演示了，如何设置编码器增益系数，
 * @n           以及在检测按键按下时，将编码器计数值置零。
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence     The MIT License (MIT)
 * @author      [qsjhyy](yihuan.huang@dfrobot.com)
 * @version     V0.1
 * @date        2021-09-15
 * @get from    https://www.dfrobot.com
 * @url         https://github.com/DFRobot/DFRobot_VisualRotaryEncoder
 */
#include <DFRobot_VisualRotaryEncoder.h>

/*
实例化一个对象，来驱动我们的传感器;
地址根据编码器拨码开关设置:
| a | b | ADDR |
|---|---|------|
| 0 | 0 | 0x54 |
| 0 | 1 | 0x55 |
| 1 | 0 | 0x56 |
| 1 | 1 | 0x57 |
*/
DFRobot_VisualRotaryEncoder_IIC sensor(/*iicAddr = */0x54, /*iicBus = */&Wire);

void setup()
{
  Serial.begin(115200);

  // 初始化传感器
  while( NO_ERR != sensor.begin() ){
    Serial.println("Communication with device failed, please check connection");
    delay(3000);
  }
  Serial.println("Begin ok!");

  /**
   * 设置编码器增益系数，转动一格的数值精度。
   * 精度范围：1~51，最小为1（转动约2.5圈LED灯亮一个），最大为51（转动一格LED灯就亮起一个）。
   * gainValue 范围[1, 51], 超出范围设置无效
   */
  sensor.setGainCoefficient(50);

  /**
   * 获取编码器当前增益系数，转动一格的数值精度。
   * 精度范围：1~51，最小为1（转动约2.5圈LED灯亮一个），最大为51（转动一格LED灯就亮起一个）。
   * 返回值范围为： 1-51
   */
  uint8_t gainCoefficient = sensor.getGainCoefficient();
  Serial.print("Encoder current gain coefficient: ");
  Serial.println(gainCoefficient);

  Serial.println();
  delay(1000);
}

void loop()
{
  /**
   * 检测按键是否已按下
   * 返回true 已检测到按键按下，返回false 未检测到按键按下
   */
  if(sensor.detectButtonDown()){
    /**
     * 设置编码器计数值
     * value 范围[0, 1023], 超出范围设置无效
     * 此示例中，检测到按键按下，就将编码器计数值置零，能够看到亮着的led全部熄灭
     */
    sensor.setEncoderValue(0);
  }

  /**
   * 获取编码器当前计数值
   * 返回值范围为： 0-1023
   */
  uint16_t encoderValue = sensor.getEncoderValue();
  Serial.print("The encoder currently counts: ");
  Serial.println(encoderValue);

  Serial.println();
  delay(1000);
}

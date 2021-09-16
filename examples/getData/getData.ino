/*!
 * @file        getData.ino
 * @brief       这个demo演示了如何获取编码器基本信息、当前计数值和当前旋转增益
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence     The MIT License (MIT)
 * @author      [qsjhyy](qsj.huang@dfrobot.com)
 * @version     V0.1
 * @date        2021-09-15
 * @get from    https://www.dfrobot.com
 * @url         https://github.com/DFRobot/DFRobot_RotaryEncoder
 */
#include <DFRobot_RotaryEncoder.h>

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
DFRobot_RotaryEncoder_IIC sensor(/*iicAddr = */0x54, /*iicBus = */&Wire);

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
   * 读取模块基本信息
   * pbuf 读取到的数据的存放地址
   *      第一个元素为：模块的PID
   *      第二个元素为：模块的VID
   *      第三个元素为：固件版本号
   *      第四个元素为：模块的通信地址
   */
  uint16_t infoBuf[4] = {0};
  sensor.readBasicInfo(infoBuf);
  /*模块的PID，默认值0x01F6 (最高两位作为种类判断00：SEN、01：DFR、10：TEL，后面14位作为num)(SEN0502)*/
  Serial.print("PID: 0x0");
  Serial.println(infoBuf[0], HEX);

  /*模块的VID，默认值0x3343（代表厂商为DFRobot）*/
  Serial.print("VID: 0x");
  Serial.println(infoBuf[1], HEX);

  /*固件版本号：0x0100代表V0.1.0.0*/
  Serial.print("versions: 0x0");
  Serial.println(infoBuf[2], HEX);

  /*模块的通信地址，默认值0x54，模块的设备地址(0x54~0x57)*/
  Serial.print("communication address:  0x");
  Serial.println(infoBuf[3], HEX);

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
   * 获取编码器当前计数值
   * 返回值范围为： 0-1023
   */
  uint16_t encoderValue = sensor.getEncoderValue();
  Serial.print("The encoder currently counts: ");
  Serial.println(encoderValue);

  Serial.println();
  delay(1000);
}

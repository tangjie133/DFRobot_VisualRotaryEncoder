# -*- coding: utf-8 -*
'''
  # @file      get_data.py
  # @brief     这个demo演示了如何获取编码器基本信息、当前计数值和当前旋转增益
  # @copyright Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
  # @licence   The MIT License (MIT)
  # @author    [qsjhyy](yihuan.huang@dfrobot.com)
  # @version   V1.0.0
  # @date      2021-09-15
  # @get       from https://www.dfrobot.com
  # @url       https://github.com/DFRobot/DFRobot_VisualRotaryEncoder
'''
from __future__ import print_function
import sys
sys.path.append('../')
from DFRobot_VisualRotaryEncoder import *

'''
实例化一个对象，来驱动我们的传感器;
地址根据编码器拨码开关CH1和CH2设置:
| 1 | 2 | ADDR |
|---|---|------|
| 0 | 0 | 0x54 |
| 0 | 1 | 0x55 |
| 1 | 0 | 0x56 |
| 1 | 1 | 0x57 |
'''
sensor = DFRobot_VisualRotaryEncoder(i2c_addr = 0x54, bus = 1)


def setup():
  while (sensor.begin() == False):
    print ('Please check that the device is properly connected')
    time.sleep(3)
  print("sensor begin successfully!!!\n")

  '''
  * 读取模块基本信息
  * 返回读取的数据列表
  *      第一个元素为：模块的PID
  *      第二个元素为：模块的VID
  *      第三个元素为：固件版本号
  *      第四个元素为：模块的通信地址
  '''
  buf = sensor.read_basic_info()
  if 0 != len(buf):
    # 模块的PID，默认值0x01F6 (最高两位作为种类判断00：SEN、01：DFR、10：TEL，后面14位作为num)(SEN0502)
    print("PID: 0x0%x" %buf[0])

    # 模块的VID，默认值0x3343（代表厂商为DFRobot）
    print("VID: 0x%x" %buf[1])

    # 固件版本号：0x0100代表V0.1.0.0
    print("mailing address: 0x0%x" %buf[2])

    # 模块的通信地址，默认值0x54，模块的设备地址(0x54~0x57)
    print("baudrate: 0x%x" %buf[3])

  '''
  * 获取编码器当前增益系数，转动一格的数值精度。
  * 精度范围：1~51，最小为1（转动约2.5圈LED灯亮一个），最大为51（转动一格LED灯就亮起一个）。
  * 返回值范围为： 1-51
  '''
  gain_coefficient = sensor.get_gain_coefficient()
  print("Encoder current gain coefficient: %d\n" %gain_coefficient)

  time.sleep(1.5)


def loop():
  '''
  * 获取编码器当前计数值
  * 返回值范围为： 0-1023
  '''
  encoder_value = sensor.get_encoder_value()
  print("The encoder currently counts: %d" %encoder_value)

  time.sleep(1)


if __name__ == "__main__":
  setup()
  while True:
    loop()
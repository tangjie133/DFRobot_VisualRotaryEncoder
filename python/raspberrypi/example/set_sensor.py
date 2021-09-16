# -*- coding: utf-8 -*
'''
  # @file      get_data.py
  # @brief     这个demo演示了，如何设置编码器增益系数，
  # @n         以及在检测按键按下时，将编码器计数值置零。
  # @copyright Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
  # @licence   The MIT License (MIT)
  # @author    [qsjhyy](qsj.huang@dfrobot.com)
  # @version   V0.1
  # @date      2021-09-15
  # @get       from https://www.dfrobot.com
  # @url       https://github.com/DFRobot/DFRobot_RotaryEncoder
'''
from __future__ import print_function
import sys
sys.path.append('../')
from DFRobot_RotaryEncoder import *

'''
实例化一个对象，来驱动我们的传感器;
地址根据编码器拨码开关设置:
| a | b | ADDR |
|---|---|------|
| 0 | 0 | 0x54 |
| 0 | 1 | 0x55 |
| 1 | 0 | 0x56 |
| 1 | 1 | 0x57 |
'''
sensor = DFRobot_RotaryEncoder(i2c_addr = 0x54, bus = 1)

def setup():
  while (sensor.begin() == False):
    print ('Please check that the device is properly connected')
    time.sleep(3)
  print("sensor begin successfully!!!\n")

  '''
  * 设置编码器增益系数，转动一格的数值精度。
  * 精度范围：1~51，最小为1（转动约2.5圈LED灯亮一个），最大为51（转动一格LED灯就亮起一个）。
  * gainValue 范围[1, 51], 超出范围设置无效
  '''
  sensor.set_gain_coefficient(50)

  '''
  * 获取编码器当前增益系数，转动一格的数值精度。
  * 精度范围：1~51，最小为1（转动约2.5圈LED灯亮一个），最大为51（转动一格LED灯就亮起一个）。
  * 返回值范围为： 1-51
  '''
  gain_coefficient = sensor.get_gain_coefficient()
  print("Encoder current gain coefficient: %d\n" %gain_coefficient)

  time.sleep(1)


def loop():
  '''
  * 检测按键是否已按下
  * 返回true 已检测到按键按下，返回false 未检测到按键按下
  '''
  if(sensor.detect_button_down()):
    '''
    * 设置编码器计数值
    * value 范围[0, 1023], 超出范围设置无效
    * 此示例中，检测到按键按下，就将编码器计数值置零，能够看到亮着的led全部熄灭
    '''
    sensor.set_encoder_value(0)

  '''
  * 获取编码器当前计数值
  * 返回值范围为： 0-1023
  '''
  encoder_value = sensor.get_encoder_value()
  print("The encoder currently counts: %d\n" %encoder_value)

  time.sleep(1)


if __name__ == "__main__":
  setup()
  while True:
    loop()

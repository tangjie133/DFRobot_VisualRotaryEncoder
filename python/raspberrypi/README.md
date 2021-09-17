# DFRobot_VisualRotaryEncoder
* [中文版](./README_CN.md)

这是一款使用简单的旋转编码器。使用了Gravity-I2C接口输出数据。与常规的电位器相比，它有更好的手感，更高的精度，旋转步进精度可设置。并且，有一圈LED显示当前的值，形象直观。

可用做音量调节，火力调节，转速调节 ；级联3个旋转编码器，可调节RGB灯的输出状态。

![正反面svg效果图](https://github.com/cdjq/DFRobot_Sensor/raw/master/resources/images/SEN0245svg1.png)


## Product Link (https://www.dfrobot.com/)
    SKU：SEN0502


## Table of Contents

* [Summary](#summary)
* [Installation](#installation)
* [Methods](#methods)
* [Compatibility](#compatibility)
* [History](#history)
* [Credits](#credits)


## Summary

* I2C数据输出，正转加1，反转减1，使用简单明了。<br>
* 环形LED灯显示当前的值，漂亮直观。<br>
* 2个I2C端口输出，可级联使用。<br>


## Installation

使用库，首先下载库文件，将其粘贴到指定的目录中，然后打开Examples文件夹并在该文件夹中运行演示。


## Methods

```python

    '''
      @brief Initialize sensor
      @return Return True indicate initialization succeed, return False indicate failed
    '''
    def begin(self)

    '''
      @brief 读取模块基本信息
      @param pbuf 读取到的数据的存放地址
                  第一个元素为：模块的PID
                  第二个元素为：模块的VID
                  第三个元素为：固件版本号
                  第四个元素为：模块的通信地址
    '''
    def read_basic_info(self)

    '''
      @brief 获取编码器当前计数值
      @return 返回值范围为： 0-1023
    '''
    def get_encoder_value(self)

    '''
      @brief 设置编码器计数值
      @param value 范围[0, 1023], 超出范围设置无效
    '''
    def set_encoder_value(self, value)

    '''
      @brief 获取编码器当前增益系数，转动一格的数值精度。
      @n 精度范围：1~51，最小为1（转动约2.5圈LED灯亮一个），最大为51（转动一格LED灯就亮起一个）。
      @return 返回值范围为： 1-51
    '''
    def get_gain_coefficient(self)

    '''
      @brief 设置编码器增益系数，转动一格的数值精度。
      @n 精度范围：1~51，最小为1（转动约2.5圈LED灯亮一个），最大为51（转动一格LED灯就亮起一个）。
      @param gainValue 范围[1, 51], 超出范围设置无效
    '''
    def set_gain_coefficient(self, gain_value)

    '''
      @brief 检测按键是否已按下
      @return 返回true 已检测到按键按下，返回false 未检测到按键按下
    '''
    def detect_button_down(self)

```


## Compatibility

* RaspberryPi Version

| Board        | Work Well | Work Wrong | Untested | Remarks |
| ------------ | :-------: | :--------: | :------: | ------- |
| RaspberryPi2 |           |            |    √     |         |
| RaspberryPi3 |           |            |    √     |         |
| RaspberryPi4 |     √     |            |          |         |

* Python Version

| Python  | Work Well | Work Wrong | Untested | Remarks |
| ------- | :-------: | :--------: | :------: | ------- |
| Python2 |     √     |            |          |         |
| Python3 |     √     |            |          |         |


## History

- data 2021-09-15
- Version V1.0.0


## Credits

Written by qsjhyy(yihuan.huang@dfrobot.com), 2021. (Welcome to our [website](https://www.dfrobot.com/))

# -*- coding: utf-8 -*
'''
  @file DFRobot_VisualRotaryEncoder.py
  @brief Define the infrastructure of DFRobot_VisualRotaryEncoder class.
  @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
  @licence     The MIT License (MIT)
  @author [qsjhyy](yihuan.huang@dfrobot.com)
  @version  V1.0.0
  @date  2021-09-15
  @get from https://www.dfrobot.com
  @url https://github.com/DFRobot/DFRobot_VisualRotaryEncoder
'''
import sys
import time
import smbus
import logging
from ctypes import *


logger = logging.getLogger()
#logger.setLevel(logging.INFO)  #Display all print information
logger.setLevel(logging.FATAL)  #If you don’t want to display too many prints, only print errors, please use this option
ph = logging.StreamHandler()
formatter = logging.Formatter("%(asctime)s - [%(filename)s %(funcName)s]:%(lineno)d - %(levelname)s: %(message)s")
ph.setFormatter(formatter) 
logger.addHandler(ph)


VISUAL_ROTARY_ENCODER_DEFAULT_IIC_ADDR = 0x54     # 默认的IIC通信地址
VISUAL_ROTARY_ENCODER_PID              = 0x01F6   # 模块的PID (SEN0502)(最高两位作为种类判断00：SEN、01：DFR、10：TEL，后面14位作为num)

# VISUAL_ROTARY_ENCODER register address
VISUAL_ROTARY_ENCODER_PID_MSB_REG     = 0x00   # 模块的PID存储寄存器，默认值0x01F6 (最高两位作为种类判断00：SEN、01：DFR、10：TEL，后面14位作为num)
VISUAL_ROTARY_ENCODER_PID_LSB_REG     = 0x01
VISUAL_ROTARY_ENCODER_VID_MSB_REG     = 0x02   # 模块的VID存储寄存器，默认值0x3343（代表厂商为DFRobot）
VISUAL_ROTARY_ENCODER_VID_LSB_REG     = 0x03
VISUAL_ROTARY_ENCODER_VERSION_MSB_REG = 0x04   # 固件版本号存储寄存器：0x0100代表V0.1.0.0
VISUAL_ROTARY_ENCODER_VERSION_LSB_REG = 0x05
VISUAL_ROTARY_ENCODER_ADDR_REG        = 0x07   # 模块的通信地址存储寄存器，默认值0x54，模块的设备地址(1~127)
VISUAL_ROTARY_ENCODER_COUNT_MSB_REG   = 0x08   # 编码器计数值，范围0-1023
VISUAL_ROTARY_ENCODER_COUNT_LSB_REG   = 0x09
VISUAL_ROTARY_ENCODER_KEY_STATUS_REG  = 0x0A   # 编码器按键状态
VISUAL_ROTARY_ENCODER_GAIN_REG        = 0x0B   # 编码器增量系数

class DFRobot_VisualRotaryEncoder(object):
    '''
       @brief Module init
    '''
    def __init__(self, i2c_addr=VISUAL_ROTARY_ENCODER_DEFAULT_IIC_ADDR, bus=1):
        '''初始化配置参数'''
        self._iic_addr = i2c_addr
        self._i2c = smbus.SMBus(bus)

    '''
      @brief Initialize sensor
      @return Return True indicate initialization succeed, return False indicate failed
    '''
    def begin(self):
        ret = True
        chip_id = self._read_reg(VISUAL_ROTARY_ENCODER_PID_MSB_REG, 2)
        logger.info((chip_id[0] << 8) | chip_id[1])
        if 0x01F6 != (chip_id[0] << 8) | chip_id[1]:
            ret = False
        return ret

    '''
      @brief 读取模块基本信息
      @param pbuf 读取到的数据的存放地址
                  第一个元素为：模块的PID
                  第二个元素为：模块的VID
                  第三个元素为：固件版本号
                  第四个元素为：模块的通信地址
    '''
    def read_basic_info(self):
        info = [0, 0, 0, 0]
        data = self._read_reg(VISUAL_ROTARY_ENCODER_PID_MSB_REG, 8)

        info[0] = (data[0] << 8) | data[1]   # PID
        info[1] = (data[2] << 8) | data[3]   # VID
        info[2] = (data[4] << 8) | data[5]   # version
        info[3] = data[7]   # IIC addr

        return info

    '''
      @brief 获取编码器当前计数值
      @return 返回值范围为： 0-1023
    '''
    def get_encoder_value(self):
        data = self._read_reg(VISUAL_ROTARY_ENCODER_COUNT_MSB_REG, 2)
        return (data[0] << 8) | data[1]

    '''
      @brief 设置编码器计数值
      @param value 范围[0, 1023], 超出范围设置无效
    '''
    def set_encoder_value(self, value):
        if ((0x0000 <= value) and (0x3FF >= value)):
            temp_buf = [(value & 0xFF00) >> 8, value & 0x00FF]
            self._write_reg(VISUAL_ROTARY_ENCODER_COUNT_MSB_REG, temp_buf)

    '''
      @brief 获取编码器当前增益系数，转动一格的数值精度。
      @n 精度范围：1~51，最小为1（转动约2.5圈LED灯亮一个），最大为51（转动一格LED灯就亮起一个）。
      @return 返回值范围为： 1-51
    '''
    def get_gain_coefficient(self):
        return self._read_reg(VISUAL_ROTARY_ENCODER_GAIN_REG, 1)[0]

    '''
      @brief 设置编码器增益系数，转动一格的数值精度。
      @n 精度范围：1~51，最小为1（转动约2.5圈LED灯亮一个），最大为51（转动一格LED灯就亮起一个）。
      @param gainValue 范围[1, 51], 超出范围设置无效
    '''
    def set_gain_coefficient(self, gain_value):
        if ((0x01 <= gain_value) and (0x33 >= gain_value)):
            self._write_reg(VISUAL_ROTARY_ENCODER_GAIN_REG, gain_value)

    '''
      @brief 检测按键是否已按下
      @return 返回true 已检测到按键按下，返回false 未检测到按键按下
    '''
    def detect_button_down(self):
        ret = False
        if(1 == self._read_reg(VISUAL_ROTARY_ENCODER_KEY_STATUS_REG, 1)[0]):
            self._write_reg(VISUAL_ROTARY_ENCODER_KEY_STATUS_REG, 0)
            ret = True
        return ret

    '''
      @brief writes data to a register
      @param reg register address
             data written data
    '''
    def _write_reg(self, reg, data):
        if isinstance(data, int):
            data = [data]
            #logger.info(data)
        self._i2c.write_i2c_block_data(self._iic_addr, reg, data)

    '''
      @brief read the data from the register
      @param reg register address
             length read data length
    '''
    def _read_reg(self, reg, length):
        return self._i2c.read_i2c_block_data(self._iic_addr, reg, length)

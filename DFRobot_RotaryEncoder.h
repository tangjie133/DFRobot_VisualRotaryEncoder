/*!
 * @file DFRobot_RotaryEncoder.h
 * @brief DFRobot_RotaryEncoder.h detailed description for DFRobot_RotaryEncoder.cpp
 * @n DFRobot_RotaryEncoder.h 定义了设备信息寄存器和设备功能寄存器，声明了传感器的功能函数api
 * 
 * @copyright Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence The MIT License (MIT)
 * @author [qsjhyy](qsj.huang@dfrobot.com)
 * @version V0.1
 * @date 2021-09-14
 * @get from https://www.dfrobot.com
 * @url https://github.com/DFRobot/DFRobot_RotaryEncoder
 */
#ifndef __DFROBOT_ROTARY_ENCODER_H__
#define __DFROBOT_ROTARY_ENCODER_H__

#if (ARDUINO >= 100)
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif
#include "Wire.h"

// 设置宏ENABLE_DBG为1时，可以看到程序的调试打印信息
#define ENABLE_DBG 0
#if ENABLE_DBG
#define DBG(...) {Serial.print("[");Serial.print(__FUNCTION__); Serial.print("(): "); Serial.print(__LINE__); Serial.print(" ] "); Serial.println(__VA_ARGS__);}
#else
#define DBG(...)
#endif


#define ROTARY_ENCODER_DEFAULT_IIC_ADDR   uint8_t(0x54)      // 默认的IIC通信地址
#define ROTARY_ENCODER_PID                uint16_t(0x01F6)   // 模块的PID (SEN0502)(最高两位作为种类判断00：SEN、01：DFR、10：TEL，后面14位作为num)

// ROTARY_ENCODER register address
#define ROTARY_ENCODER_PID_MSB_REG                 uint8_t(0x00)   // 模块的PID存储寄存器，默认值0x01F6 (最高两位作为种类判断00：SEN、01：DFR、10：TEL，后面14位作为num)
#define ROTARY_ENCODER_PID_LSB_REG                 uint8_t(0x01)
#define ROTARY_ENCODER_VID_MSB_REG                 uint8_t(0x02)   // 模块的VID存储寄存器，默认值0x3343（代表厂商为DFRobot）
#define ROTARY_ENCODER_VID_LSB_REG                 uint8_t(0x03)
#define ROTARY_ENCODER_VERSION_MSB_REG             uint8_t(0x04)   // 固件版本号存储寄存器：0x0100代表V0.1.0.0
#define ROTARY_ENCODER_VERSION_LSB_REG             uint8_t(0x05)
#define ROTARY_ENCODER_ADDR_REG                    uint8_t(0x07)   // 模块的通信地址存储寄存器，默认值0x54，模块的设备地址(1~127)
#define ROTARY_ENCODER_COUNT_MSB_REG               uint8_t(0x08)   // 编码器计数值，范围0-1023
#define ROTARY_ENCODER_COUNT_LSB_REG               uint8_t(0x09)
#define ROTARY_ENCODER_KEY_STATUS_REG              uint8_t(0x0A)   // 编码器按键状态
#define ROTARY_ENCODER_GAIN_REG                    uint8_t(0x0B)   // 编码器增量系数


class DFRobot_RotaryEncoder
{
public:
  #define NO_ERR             0   // No error
  #define ERR_DATA_BUS      (-1)   // 数据总线错误
  #define ERR_IC_VERSION    (-2)   // 芯片版本不匹配

public:
  /**
  * @brief 构造函数
  */
  DFRobot_RotaryEncoder();

  /**
  * @brief 初始化函数
  * @return 返回0表示初始化成功，返回其他值表示初始化失败，返回错误码
  */
  virtual int begin(void);

  /**
  * @brief 读取模块基本信息
  * @param pbuf 读取到的数据的存放地址
  *             第一个元素为：模块的PID
  *             第二个元素为：模块的VID
  *             第三个元素为：固件版本号
  *             第四个元素为：模块的通信地址
  */
  void readBasicInfo(uint16_t* pbuf);

  /**
  * @brief 获取编码器当前计数值
  * @return 返回值范围为： 0-1023
  */
  uint16_t getEncoderValue(void);

  /**
  * @brief 设置编码器计数值
  * @param value 范围[0, 1023], 超出范围设置无效
  */
  void setEncoderValue(uint16_t value);

  /**
  * @brief 获取编码器当前增益系数，转动一格的数值精度。
  * @n 精度范围：1~51，最小为1（转动约2.5圈LED灯亮一个），最大为51（转动一格LED灯就亮起一个）。
  * @return 返回值范围为： 1-51
  */
  uint8_t getGainCoefficient(void);

  /**
  * @brief 设置编码器增益系数，转动一格的数值精度。
  * @n 精度范围：1~51，最小为1（转动约2.5圈LED灯亮一个），最大为51（转动一格LED灯就亮起一个）。
  * @param gainValue 范围[1, 51], 超出范围设置无效
  */
  void setGainCoefficient(uint8_t gainValue);

  /**
  * @brief 检测按键是否已按下
  * @return 返回true 已检测到按键按下，返回false 未检测到按键按下
  */
  bool detectButtonDown(void);

protected:
/***************** 寄存器读写接口 ******************************/

  /**
  * @brief 写寄存器函数，设计为纯虚函数，由派生类实现函数体
  * @param reg  寄存器地址 8bits
  *        pBuf 要写入数据的存放缓存
  *        size 要写入数据的长度
  */
  virtual void writeReg(uint8_t reg, const void* pBuf, size_t size)=0;

  /**
  * @brief 读取寄存器函数，设计为纯虚函数，由派生类实现函数体
  * @param reg  寄存器地址 8bits
  *        pBuf 要读取数据的存放缓存
  *        size 要读取数据的长度
  * @return 返回读取的长度，返回0表示读取失败
  */
  virtual size_t readReg(uint8_t reg, void* pBuf, size_t size)=0;

private:
  // 定义私有变量
};

/***************** IIC接口的初始化和读写 ******************************/

class DFRobot_RotaryEncoder_IIC:public DFRobot_RotaryEncoder
{
public:
  /**
  * @brief 构造函数，根据模块拨码开关状态，配置传感器IIC通信地址
  * @param IIC_addr RotaryEncoder IIC communication address
  *        pWire Wire.h里定义了Wire对象，因此使用&Wire就能够指向并使用Wire中的方法
  */
  DFRobot_RotaryEncoder_IIC(uint8_t IIC_addr=ROTARY_ENCODER_DEFAULT_IIC_ADDR, TwoWire *pWire = &Wire);

  /**
  * @brief 子类初始化函数
  * @return 返回0表示初始化成功，返回其他值表示初始化失败
  */
  virtual int begin(void);

protected:

  /**
  * @brief 通过IIC总线写入寄存器值
  * @param reg  寄存器地址 8bits
  *        pBuf 要写入数据的存放缓存
  *        size 要写入数据的长度
  */
  virtual void writeReg(uint8_t reg, const void* pBuf, size_t size);

  /**
  * @brief 通过IIC总线读取寄存器值
  * @param reg  寄存器地址 8bits
  *        pBuf 要读取数据的存放缓存
  *        size 要读取数据的长度
  * @return 返回读取的长度，返回0表示读取失败
  */
  virtual size_t readReg(uint8_t reg, void* pBuf, size_t size);

private:
  TwoWire *_pWire;   // IIC通信方式的指针
  uint8_t _deviceAddr;   // IIC通信的设备地址
};

#endif

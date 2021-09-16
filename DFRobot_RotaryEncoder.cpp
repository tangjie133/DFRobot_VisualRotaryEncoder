/*!
 * @file DFRobot_RotaryEncoder.CPP
 * @brief DFRobot_RotaryEncoder.cpp Initialize the IIC,
 * @n 获取编码器基本信息、按键触发情况和当前计数值，调整编码器计数值和旋转增量
 * @copyright Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence The MIT License (MIT)
 * @author [qsjhyy](qsj.huang@dfrobot.com)
 * @version V0.1
 * @date 2021-09-15
 * @get from https://www.dfrobot.com
 * @url https://github.com/DFRobot/DFRobot_RotaryEncoder
 */
#include "DFRobot_RotaryEncoder.h"

DFRobot_RotaryEncoder::DFRobot_RotaryEncoder()
{
}


int DFRobot_RotaryEncoder::begin(void)
{
  uint8_t idBuf[2];
  if(0 == readReg(ROTARY_ENCODER_PID_MSB_REG, idBuf, sizeof(idBuf))){   // Judge whether the data bus is successful
    DBG("ERR_DATA_BUS");
    return ERR_DATA_BUS;
  }

  DBG("real sensor id="); DBG( (uint16_t)idBuf[0] << 8 | (uint16_t)idBuf[1], HEX );
  if( 0x01F6 != ( (uint16_t)idBuf[0] << 8 | (uint16_t)idBuf[1] ) ){   // Judge whether the chip version matches
    DBG("ERR_IC_VERSION");
    return ERR_IC_VERSION;
  }

  delay(200);
  DBG("begin ok!");
  return NO_ERR;
}

void DFRobot_RotaryEncoder::readBasicInfo(uint16_t* pbuf)
{
  uint8_t tempBuf[8] = {0};
  readReg(ROTARY_ENCODER_PID_MSB_REG, tempBuf, sizeof(tempBuf));

  pbuf[0] = (uint16_t)tempBuf[0] << 8 | (uint16_t)tempBuf[1];   // PID
  pbuf[1] = (uint16_t)tempBuf[2] << 8 | (uint16_t)tempBuf[3];   // VID
  pbuf[2] = (uint16_t)tempBuf[4] << 8 | (uint16_t)tempBuf[5];   // version
  pbuf[3] = (uint16_t)tempBuf[7];   // IIC addr
}

uint16_t DFRobot_RotaryEncoder::getEncoderValue(void)
{
  uint8_t CountValue[2] = {0};
  readReg(ROTARY_ENCODER_COUNT_MSB_REG, CountValue, sizeof(CountValue));

  return (uint16_t)CountValue[0] << 8 | (uint16_t)CountValue[1];
}

void DFRobot_RotaryEncoder::setEncoderValue(uint16_t value)
{
  if((0x0000 <= value) && (0x3FF >= value)){
    uint8_t tempBuf[2] = {0};
    tempBuf[0] = (value & 0xFF00) >> 8;
    tempBuf[1] = value & 0x00FF;
    writeReg(ROTARY_ENCODER_COUNT_MSB_REG, tempBuf, sizeof(tempBuf));
  }
}

uint8_t DFRobot_RotaryEncoder::getGainCoefficient(void) {
  uint8_t RotateGain = 0;
  readReg(ROTARY_ENCODER_GAIN_REG, &RotateGain, sizeof(RotateGain));

  return RotateGain;
}

void DFRobot_RotaryEncoder::setGainCoefficient(uint8_t gainValue) {
  if((0x01 <= gainValue) && (0x33 >= gainValue)){
    writeReg(ROTARY_ENCODER_GAIN_REG, &gainValue, sizeof(gainValue));
  }
}

bool DFRobot_RotaryEncoder::detectButtonDown(void) {
  bool ret = false;
  uint8_t buttonStatus, clearStatus=0x00;

  readReg(ROTARY_ENCODER_KEY_STATUS_REG, &buttonStatus, sizeof(buttonStatus));

  if( 0 != (buttonStatus & 0x01) ){
    writeReg(ROTARY_ENCODER_KEY_STATUS_REG, &clearStatus, sizeof(clearStatus));
    ret = true;
  }

  return ret;
}

/************ Initialization of IIC interfaces reading and writing ***********/

DFRobot_RotaryEncoder_IIC::DFRobot_RotaryEncoder_IIC(uint8_t IIC_addr, TwoWire *pWire)
{
  _deviceAddr = IIC_addr;
  _pWire = pWire;
}

int DFRobot_RotaryEncoder_IIC::begin(void)
{
  _pWire->begin();   // Wire.h（IIC）library function initialize wire library
  delay(50);

  return DFRobot_RotaryEncoder::begin();   // Use the initialization function of the parent class
}

void DFRobot_RotaryEncoder_IIC::writeReg(uint8_t reg, const void* pBuf, size_t size)
{
  if(pBuf == NULL){
    DBG("pBuf ERROR!! : null pointer");
  }
  uint8_t * _pBuf = (uint8_t *)pBuf;

  _pWire->beginTransmission(_deviceAddr);
  _pWire->write(reg);

  for(size_t i = 0; i < size; i++){
    _pWire->write(_pBuf[i]);
  }

  _pWire->endTransmission();
}

size_t DFRobot_RotaryEncoder_IIC::readReg(uint8_t reg, void* pBuf, size_t size)
{
  size_t count = 0;
  if(NULL == pBuf){
    DBG("pBuf ERROR!! : null pointer");
  }
  uint8_t * _pBuf = (uint8_t*)pBuf;

  _pWire->beginTransmission(_deviceAddr);
  _pWire -> write(reg);
  if(0 != _pWire->endTransmission()){
    // Used Wire.endTransmission() to end a slave transmission started by beginTransmission() and arranged by write().
    DBG("endTransmission ERROR!!");
  }else{
    // Master device requests size bytes from slave device, which can be accepted by master device with read() or available()
    _pWire->requestFrom( _deviceAddr, (uint8_t)size );

    while (_pWire->available()){
      _pBuf[count++] = _pWire->read();   // Use read() to receive and put into buf
      // DBG(_pBuf[count-1], HEX);
    }
  }

  return count;
}

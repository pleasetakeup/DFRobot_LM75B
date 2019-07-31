/*!
   @file DFRobot_LM75B.h
   @brief 定义DFRobot_LM75B 类的基础结构，基础方法的实现
   @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
   @licence     The MIT License (MIT)
   @author [fengli](li.feng@dfrobot.com)
   @version  V1.0
   @date  2019-07-28
   @get from https://www.dfrobot.com
   @https://github.com/DFRobot/DFRobot_LM75B
*/

#include <DFRobot_LM75B.h>
int DFRobot_LM75B::begin() 
{
  uint8_t buffer[2];
  _pWire->begin();
  if(readReg(REG_LM75B_TEMP, buffer, 2) != 2){
    DBG("");
    DBG("bus data access error"); DBG("");
    return ERR_DATA_BUS;
  }
  return ERR_OK;
}

float DFRobot_LM75B::getTempC() 
{
  uint16_t temp = 0;
  uint8_t buffer[2] = {0};
  readReg(REG_LM75B_TEMP, buffer, 2);
  //首先判断读到的温度数据是正是负.
  if ((buffer[0] & 1024) == 0) {
    temp = buffer[0] << 8;
    temp = temp | buffer[1];
    temp = temp >> 5;
    return temp * 0.125;
  } else {
    temp = buffer[0] << 3;
    temp = temp | (buffer[1] >> 5);
    temp = temp & 0x3ff;
    temp = 1024 - temp;
    return -(temp * 0.125);
  }               //这样写是否可以
}
/*uint8_t DFRobot_LM75B::getconfReg(){
  uint8_t buffer[2]={0};
  uint8_t write[2]={2,0};
  writeReg(0x01,write,2);
  readReg(0x01, buffer, 2);
  for(uint16_t i = 0;i<2;i++){
    Serial.println(buffer[i]);
  }
  }*/

void DFRobot_LM75B::setTos(float Tos) 
{ 
  // 阈值的范围不能在温度测量范围之外
  if(Tos > 125.0)
    Tos = 125.0;
  else if(Tos < -55.0)
    Tos = -55.0;
  uint8_t write[2] = {0};
  if (Tos >= 0) {
    uint16_t _Tos = Tos * 2 ;
    write[0] = _Tos >> 1;
    write[1] = (_Tos & 1) << 7;
  } else {
    int _Tos = (256 + Tos * 2);
    write[0] = 0x80 | ((uint16_t)_Tos >> 1);
    write[1] = (uint16_t)_Tos << 7;
  }
  writeReg(REG_LM75B_TOS, write, 2);
}
void DFRobot_LM75B::setThyst(float Thyst) 
{ 
  // 滞后值的范围不能在温度测量范围之外
  if(Thyst > 125.0)
    Thyst = 125.0;
  else if(Thyst < -55.0)
    Thyst = -55.0;
  uint8_t write[2] = {0};
  if (Thyst >= 0) {
    uint16_t _Thyst = Thyst * 2 ;
    write[0] = _Thyst >> 1;
    write[1] = (_Thyst & 1) << 7;
  } else {
    int _Thyst = (256 + Thyst * 2);
    write[0] = 0x80 | ((uint16_t)_Thyst >> 1);
    write[1] = (uint16_t)_Thyst << 7;
  }
  writeReg(REG_LM75B_THYST, write, 2);
}
float DFRobot_LM75B::getTosC(void ) 
{
  uint16_t Tos = 0;
  uint8_t buffer[2] = {0};
  readReg(REG_LM75B_TOS, buffer, 2);
    //首先判断读到的温度数据是正是负.
  if ((buffer[0] & 0x80) == 0) {
    Tos = buffer[0];
    Tos = Tos << 1 ;
    Tos = Tos | (buffer[1] >> 7);
    return Tos / 2.0;
  } else {
    Tos = buffer[0] << 1;
    Tos = Tos | (buffer[1] >> 7);
    Tos = Tos & 0xff;
    Tos = 256 - Tos;
    return -(Tos / 2.0);
  }
}
float DFRobot_LM75B::getThystC() 
{
  uint16_t Thyst = 0;
  uint8_t buffer[2] = {0};
  readReg(REG_LM75B_THYST, buffer, 2);
    //首先判断读到的温度数据是正是负.
  if ((buffer[0] & 0x80) == 0) {
    Thyst = buffer[0];
    Thyst = Thyst << 1 ;
    Thyst = Thyst | (buffer[1] >> 7);
    return Thyst / 2.0;
  } else {
    Thyst = buffer[0] << 1;
    Thyst = Thyst | (buffer[1] >> 7);
    Thyst = Thyst & 0xff;
    Thyst = 256 - Thyst;
    return -(Thyst / 2.0);
  }
}
DFRobot_LM75B::eOSPolarityMode_t DFRobot_LM75B::getOSPolarityMode() 
{
  uint8_t buffer[1] = {0};
  readReg(REG_LM75B_CONF, buffer, 1);
  return (DFRobot_LM75B::eOSPolarityMode_t)((buffer[0] & 0x04) >> 2);
}
void DFRobot_LM75B::setOSPolarityMode(DFRobot_LM75B::eOSPolarityMode_t epolarityMode) 
{
  DFRobot_LM75B::sMode_t configuration;
  uint8_t buffer[1] = {0};
  readReg(REG_LM75B_CONF, buffer, 1);
  memcpy(&configuration, buffer, sizeof(configuration));
  configuration.sOS_POL = epolarityMode;
  writeReg(REG_LM75B_CONF, &configuration, 1);
}
DFRobot_LM75B::eQueueValue_t DFRobot_LM75B::getQueueValue() 
{
  uint8_t buffer[1] = {0};
  readReg(REG_LM75B_CONF, buffer, 1);
  if ((buffer[0] & 0x18) == 0)
    return (DFRobot_LM75B::eQueueValue_t)1;
  else
    return (DFRobot_LM75B::eQueueValue_t)((buffer[0] & 0x18) >> 2);
}

void DFRobot_LM75B::setQueueValue(DFRobot_LM75B::eQueueValue_t value) 
{
  DFRobot_LM75B::sMode_t configuration;
  uint8_t buffer[1] = {0};
  readReg(REG_LM75B_CONF, buffer, 1);
  memcpy(&configuration, buffer, sizeof(configuration));
  configuration.sOS_F_QUE = value>>1;
  writeReg(REG_LM75B_CONF, &configuration, 1);
}
DFRobot_LM75B::eOSMode_t DFRobot_LM75B::getOSMode() 
{
  uint8_t buffer[1] = {0};
  readReg(REG_LM75B_CONF, buffer, 1);
  return (DFRobot_LM75B::eOSMode_t)((buffer[0] & 0x02) >> 1);
}
void DFRobot_LM75B::setOSMode(DFRobot_LM75B::eOSMode_t osMode) 
{
  DFRobot_LM75B::sMode_t configuration;
  uint8_t buffer[1] = {0};
  readReg(REG_LM75B_CONF, buffer, 1);
  //先读取此时conf寄存器里面的值
  memcpy(&configuration, buffer, sizeof(configuration));
  //将OS模式代表的位的数值改变成osMode.
  configuration.sOS_COMP_INT = osMode;
  //将改好的配置写入conf寄存器.
  writeReg(REG_LM75B_CONF, &configuration, 1);
}

DFRobot_LM75B::eShutDownMode_t DFRobot_LM75B::getShutDownMode()
{
  uint8_t buffer[1] = {0};
  readReg(REG_LM75B_CONF, buffer, 1);
  //将寄存器内代表hutDown模式的位的数据提取出来
  return (DFRobot_LM75B::eShutDownMode_t)(buffer[0] & 0x01);
}

void DFRobot_LM75B::setShutDownMode(DFRobot_LM75B::eShutDownMode_t ShutDownMode) 
{
  DFRobot_LM75B::sMode_t configuration;
  uint8_t buffer[1] = {0};
  readReg(REG_LM75B_CONF, buffer, 1);
  memcpy(&configuration, buffer, sizeof(configuration));
  configuration.sSHUTDOWN = ShutDownMode;
  writeReg(REG_LM75B_CONF, &configuration, 1);
}

void DFRobot_LM75B::writeReg(uint8_t reg, void* pBuf, size_t size) 
{
  if (pBuf == NULL) {
    DBG("pBuf ERROR!! : null pointer");
  }
  uint8_t * _pBuf = (uint8_t *)pBuf;
  _pWire->beginTransmission(_address);
  _pWire->write(reg);
  for (uint16_t i = 0; i < size; i++) {
    _pWire->write(_pBuf[i]);
  }
  _pWire->endTransmission();
}
uint8_t DFRobot_LM75B::readReg(uint8_t reg, void* pBuf, size_t size) 
{
  if (pBuf == NULL) {
    DBG("pBuf ERROR!! : null pointer");
  }
  uint8_t * _pBuf = (uint8_t *)pBuf;
  _pWire->beginTransmission(_address);
  _pWire->write(reg);
  if ( _pWire->endTransmission() != 0) {
    return 0;
  }
  delay(10);
  _pWire->requestFrom(_address, size);
  for (uint16_t i = 0; i < size; i++) {
    _pBuf[i] = _pWire->read();
  }
  _pWire->endTransmission();
  return size;
}
/*!
   @file thermostat.ino
   @brief 恒温器，让设备保持在一定的温度范围.
   @n 实验现象：在开始之前我们会设置阈值温度Tos和滞后温度Thyst，而让温度保持在这个范围内
   @n 我们模拟外部环境，当串口打印超过阈值温度Tos时降温，在温度低于滞后温度Thyst时升温，达到
   @n 一个恒温装置的效果

   @copyright  Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
   @licence     The MIT License (MIT)
   @author [fengli](li.feng@dfrobot.com)
   @version  V1.0
   @date  2019-07-30
   @get from https://www.dfrobot.com
   @https://github.com/DFRobot/DFRobot_LM75B
*/
#include <DFRobot_LM75B.h>

DFRobot_LM75B lm75b(&Wire, 0x48);
#define OS   (4)
void setup(void) {
  Serial.begin(115200);
  //检测IIC是否能正常通信
  while (lm75b.begin() != 0) {
    Serial.println("IIC初始化失败，请检测连线是否正确");
    delay(1000);
  }
  pinMode(OS, INPUT);
  /**
       @brief 设置阈值温度
       @param 温度值，单位是摄氏度，需满足Tos%0.5 == 0 ；
       @n 范围是 -55°C 到 +125°C
  */
  lm75b.setTos(/*阈值温度=*/35);
  /**
       @brief 设置滞后温度
       @param 温度值，单位是摄氏度，需满足Thyst%0.5 == 0 ；
       @n 范围是 -55°C 到 +125°C,Thyst 必须小于 Tos 的值.
  */
  lm75b.setThyst(/*滞后温度=*/30.5);
  /*!
    设置故障队列数,只有满足故障队列数，OS才会产生中断
    typedef enum {
    eValue1 = 1,
    eValue2 = 2,
    eValue3 = 4,
    eValue4 = 6
    } eQueueValue_t;
  */
  lm75b.setQueueValue(lm75b.eValue4);
}

void loop(void) {

  if (digitalRead(OS) == 0) {
    Serial.println("环境温度超过阈值温度，请降温");
  }
  if (digitalRead(OS) == 1) {
    Serial.println("环境温度低于滞后温度，请升温");
  }
  delay(2000);

}
/*!
   @file Hightemp_alarm.ino
   @brief 高温报警.
   @n 实验现象：在开始之前我们会设置阈值温度Tos和滞后温度Thyst，芯片工作状态，
   @n OS引脚输出模式，故障队列。当温度超过这个值时串口就会有信息提示，或者也可
   @n 以在arduino上加一个蜂鸣器来提醒温度超过阈值

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
  //检测IIC是否能正常通信.
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
  lm75b.setTos(/*阈值温度=*/33);
  /**
       @brief 设置滞后温度
       @param 温度值，单位是摄氏度，需满足Thyst%0.5 == 0 ；
       @n 范围是 -55°C 到 +125°C,Thyst 必须小于 Tos 的值.
  */
  lm75b.setThyst(/*滞后温度=*/32.5);
  /*!
    设置芯片工作模式
    typedef enum {
    eNormal = 0, //<在此模式下，数据采集周期为100ms,其中10ms用于数据转换，需要电流为300mA，另外90ms处于idle状态，需要电流为10uA>
    eShutdown = 1 //<在此模式下，数据采集停止，但IIC通信不受影响，寄存器也可以正常读写>
    } eShutDownMode_t;
  */
  lm75b.setShutDownMode(eNormal);
  /*!
    设置设置OS引脚的模式
    typedef enum {
    eComparator = 0, //<OS口输出采用比较器模式，OS becomes active when the Temp exceeds the Tth(ots), and is reset when the Temp drops below the Thysh>
    eInterrupt = 1 //<在此模式下，OS口输出采用中断模式,Once the OS output has been activated by crossing Tth(ots) and then reset, it can be activated again only when the Temp drops below the Thys>
    } eOSMode_t;
  */
  lm75b.setOSMode(eComparator);
  /*!
        设置故障队列数,Fault queue is defined as the number of faults that must occur consecutively to activate
    the OS output
    typedef enum {
    eValue1 = 1,
    eValue2 = 2,
    eValue3 = 4,
    eValue4 = 6
    } eQueueValue_t;
  */
  lm75b.setQueueValue(eValue4);
}

void loop(void) {
  //检测OS的状态来判断温度是否超过设定值
  while (digitalRead(OS) == 0) {
    Serial.println("环境温度超过阈值温度，请注意");
    delay(5000);
  }
}
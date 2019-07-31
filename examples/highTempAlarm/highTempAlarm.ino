/*!
   @file highTempAlarm.ino
   @brief 高温报警.
   @n 实验现象：在开始之前我们会设置阈值温度Tos和滞后温度Thyst，芯片工作状态，
   @n OS引脚输出模式，故障队列。当温度超过阈值温度Tos时串口就会有信息提示，或者也可
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
  lm75b.setShutDownMode(lm75b.eNormal);
   /*!
      The OS output active state can be selected as HIGH or LOW by programming bit B2
      (OS_POL) of register Conf
       typedef enum {
       eActive_LOW = 0,  <在此模式下，OS的active状态为低电平>
       eActive_HIGH = 1  <在此模式下，OS的active状态为高电平>
       } eOSPolarityMode_t;
     当温度值大于阈值温度，若满足则OS输出为active状态，active状态默认为低电平。
  */
  lm75b.setOSPolarityMode(lm75b.eActive_LOW);
  /*!
    设置设置OS引脚的模式
    typedef enum {
    eComparator = 0, //<OS口输出采用比较器模式，OS becomes active when the Temp exceeds the Tth(ots), and is reset 
                       when the Temp drops below the Thysh>
    eInterrupt = 1 //<在此模式下，OS口输出采用中断模式,Once the OS output has been activated by crossing Tth(ots) 
                       and then reset, it can be activated again only when the Temp drops below the Thys>
    } eOSMode_t;
  */
  lm75b.setOSMode(lm75b.eComparator);
  /*!
    只有满足故障队列数，OS才会产生中断
    故障队列数：温度寄存器存储的温度值在每次转换完成之后，会自动与阈值温度和滞后温度相比较。
    当选择eValue1，只需满足一次温度值大于阈值温度,若满足则OS输出为active状态；
    当选择eValue2，需满足二次温度值大于阈值温度,若满足则OS输出为active状态。
    以此类推。
    the OS output
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
  //检测OS的状态来判断温度是否超过设定值
    /*!
         默认设置 device operation mode selection：(0*)normal
                  OS operation mode selection    ：(0*)OS comparator
                  OS polarity selection          ：(0*)OS active LOW
                  OS fault queue programming     ：(00*)queue value = 1
                  reserved                       ： 000*
  */
  //因为 polarity 选择的是active LOW模式，所以当温度值大于阈值温度，OS输出为低电平
  while (digitalRead(OS) == 0) {
    Serial.println("环境温度超过阈值温度，请注意");
    delay(5000);
  }
}
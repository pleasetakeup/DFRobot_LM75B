/*!
   @file readTemp.ino
   @brief 读取环境温度.
   @n 实验现象：我们在开始前读取一次芯片的默认设置，然后每隔一秒读取一次温度
   @n 芯片内每100ms采集一次温度数据，读取温度和温度采集与转换并不相互影响.

   @copyright  Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
   @licence     The MIT License (MIT)
   @author [fengli](li.feng@dfrobot.com)
   @version  V1.0
   @date  2019-07-30
   @get from https://www.dfrobot.com
   @https://github.com/DFRobot/DFRobot_LM75B
*/
#include <DFRobot_LM75B.h>
/**
    @brief 构造函数
    @param pWire I2C总线指针对象，构造设备，可传参数也可不传参数，默认Wire
    @param addr 7位I2C地址,由前三位决定地址的值，取值(0x48/0x49/0x5A/0x5B/0x5C/0x5D/0x5E/0x5F)默认0x48
    @n IIC地址由构成如下图所示
      6  5  4  3  2  1   0
      1  0  0  1  A2 A1  A0
    @n 地址的定义如下表所示,可以通过硬件上的焊盘来改变IIC地址：默认为0x48
      1  0  0  1  | A2 A1 A0
      1  0  0  1  | 1  1  1       0x5F
      1  0  0  1  | 1  1  0       0x5E
      1  0  0  1  | 1  0  1       0x5D
      1  0  0  1  | 1  0  0       0x5C
      1  0  0  1  | 0  1  1       0x5B
      1  0  0  1  | 0  1  0       0x5A
      1  0  0  1  | 0  0  1       0x49
      1  0  0  1  | 0  0  0       0x48
*/
DFRobot_LM75B lm75b(&Wire, 0x48);

void setup(void) {
  Serial.begin(115200);
  //检测IIC是否能正常通信
  while (lm75b.begin() != 0) {
    Serial.println("IIC初始化失败，请检测连线是否正确");
    delay(1000);
  }
  //芯片的设置在每次断电然后从新上电后都会重置
  Serial.println("**--------------------default state--------------------**");
  Serial.print("芯片工作模式: ");
  /**
       正常模式：在此模式下，芯片正常工作，每100ms采集一次温度数据并存储到温度寄存器
       关断模式：在此模式下，数据采集转换停止，但寄存器能正常读写.
  */
  Serial.print(lm75b.getShutDownMode());
  Serial.println(" (0:正常模式/1：关断模式)");
  Serial.print("OS模式: ");
  /**
       比较器模式：此模式下，OS引脚在环境温度超过Tos设定温度时，产生低电平，然后在低于Thyst恢复高电平，然后一直循环.
       中断断模式：此模式下，OS在第一次环境温度超过Tos设定温度时被激活，然后产生一个脉冲信号，等到温度低于Thyst才会
               产生下一个脉冲信号，然后一直循环
  */
  Serial.print(lm75b.getOSMode());
  Serial.println(" (0:比较器模式/1：中断模式)");
  /*!
      The OS output active state can be selected as HIGH or LOW by programming bit B2
      (OS_POL) of register Conf
       typedef enum {
       eActive_LOW = 0,  <在此模式下，OS的active状态为低电平>
       eActive_HIGH = 1  <在此模式下，OS的active状态为高电平>
       } eOSPolarityMode_t;
     当温度值大于阈值温度，则OS输出为active状态，active状态默认为低电平。
  */
  Serial.print("OS极性: ");
  Serial.print(lm75b.getOSPolarityMode());
  Serial.println(" (0:active状态为低电平/1：active状态为高电平)");
  /*!
    只有满足故障队列数，OS才会产生中断
    故障队列数：温度寄存器存储的温度值在每次转换完成之后，会自动与阈值温度和滞后温度相比较。
    当选择eValue1，只需满足一次温度值大于阈值温度。若满足则OS输出为active状态；
    当选择eValue2，需满足二次温度值大于阈值温度。若满足则OS输出为active状态。
    以此类推。
   */
  Serial.print("OS故障队列: ");
  Serial.println(lm75b.getQueueValue());
  //用户设定值，环境温度超出此值时引起OS状态改变
  Serial.print("阈值温度: ");
  Serial.print(lm75b.getTosC());
  Serial.println("°C");
  //用户设定的滞后温度，低于此值时也会引起OS状态改变
  Serial.print("滞后温度: ");
  Serial.print(lm75b.getThystC());
  Serial.println("°C");
  Serial.println("**-----------------------------------------------------**");
}

void loop(void) {
  Serial.print("环境温度: ");
  Serial.print(/*温度=*/lm75b.getTempC());
  Serial.println("°C");
  // 设定的延时应大于100ms；
  delay(1000);
}
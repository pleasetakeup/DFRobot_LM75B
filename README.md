# DFRobot_LM75B
LM75A 是一个使用了内置带隙温度传感器和Σ-△模数转换技术的温度-数字转换器。它可提供一个过热检测输出,<br>
并有两个set-pointer寄存器用来存储可编程的过热关断和滞后限制来定义过热输出时的温度值，并且可以定义故障队列来防止噪音干扰.<br>
LM55B器件通过2线的串行IIC 总线接口与控制器通信,LM75A 有3个可选的逻辑地址管脚，使得同一总线上可同时连接8个器件而不发生地址冲突<br>
LM75A 可配置成不同的工作条件。它可设置成在正常工作模式下周期性地对环境温度进行监控或进入关断模式来将器件功耗降至最低.<br>
  分辨率：0.125°C <br>
  精度  ：正负 2 °C <br>
  功耗  :工作电流典型值为250uA，掉电模式为3.5uA<br>
  工作电压：2.8V～5.5V<br>
![正反面svg效果图](https://github.com/ouki-wang/DFRobot_Sensor/raw/master/resources/images/SEN0245svg1.png)

## 产品链接（链接到英文商城）
    SKU：LM75B 温度传感器
   
## Table of Contents

* [Summary](#summary)
* [Installation](#installation)
* [Methods](#methods)
* [Compatibility](#compatibility)
* [History](#history)
* [Credits](#credits)

## Summary

   1.读取环境温度<br>
   2.在串口打印芯片的配置信息<br>
   3.自己定义温度阈值，达到高温报警的效果<br>
   4.自己定义阈值和滞后限制值来达到恒温器的效果<br>

## Installation

To use this library, first download the library file, paste it into the \Arduino\libraries directory, then open the examples folder and run the demo in the folder.

## Methods

```C++
    
/**
  *@brief 构造函数
  *@param pWire I2C总线指针对象，构造设备，可传参数也可不传参数，默认Wire
  *@param addr 7位I2C地址,由前三位决定地址的值，取值(0x48/0x49/0x5A/0x5B/0x5C/0x5D/0x5E/0x5F)默认0x48
  *@n IIC地址由构成如下图所示
         6  5  4  3  2  1   0
         1  0  0  1  A2 A1  A0
  *@n 地址的定义如下表所示：默认为0x48
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
DFRobot_LM75B(TwoWire *pWire = &Wire, uint8_t address = 0x48) {
      _pWire = pWire;
      _address = address;
    };

/**
  *@brief 初始化函数
  *@return 返回0表示初始化成功，返回其他值表示初始化失败，返回错误码
  */
int begin();

/**
  *@brief 获取温度值.
  *@return 返回温度值，单位是摄氏度.
  *@n 范围是 -55°C 到 +125°C
  */
float getTempC();

/**
  *@brief 获取阈值温度.
  *@param 返回温度值，单位是摄氏度.
  *@n 范围是 -55°C 到 +125°C.
  */
float getTosC(void );

/**
  *@brief 获取阈值温度.
  *@param 返回温度值，单位是摄氏度.
  *@n 范围是 -55°C 到 +125°C.
  */
float getThystC();

/**
  *@brief 设置阈值温度
  *@param 温度值，单位是摄氏度，需满足Tos% 0.5 == 0 ；
  *@n 范围是 -55°C 到 +125°C
  */
void setTos(float Tos);

/**
  *@brief 设置滞后温度
  *@param 温度值，单位是摄氏度，需满足Thyst% 0.5 == 0 ；
  *@n 范围是 -55°C 到 +125°C,Thyst 必须小于 Tos 的值.
  */
void setThyst(float Thyst);

/**
  *@brief 获取故障队列的值.
  *@return 返回故障队列的值.
  */
eQueueValue_t getQueueValue();

/**
  *@brief 设置故障队列的值.
  *@param eQueueValue_t类型的值，代表故障队列数
  */
void setQueueValue(eQueueValue_t value);

/**
  *@brief 得到芯片的工作模式的模式.
  *@return 0或1 .
  *@n 0：Normal(正常模式)
  *@n 1：interrupt(关断模式)
*/
eShutDownMode_t getShutDownMode();

/**
  *@brief 设置芯片的工作模式.
  *@param eQueueValue_t类型的值，代表芯片工作模式
  */
void setShutDownMode(eShutDownMode_t ShutDownMode);

/**
  *@brief 得到OS引脚的active状态是高电平还是低电平.
  *@return 0或1 .
  *@n 0：LOW(active LOW)
  *@n 1：HIGH(active HIGH)
  */
eOSPolarityMode_t getOSPolarityMode();

/**
  *@brief 设置OS引脚的active状态是高电平还是低电平..
  *@param eOSPolarityMode_t类型的值，代表OS引脚的极性
  */
void setOSPolarityMode(eOSPolarityMode_t polarityMode);

/**
  *@brief 得到OS引脚的工作模式.
  *@return 0或1 .
  *@n 0：OS comparator(比较器模式)
  *@n 1：OS interrupt(中断模式)
  */
eOSMode_t getOSMode();

/**
  * @brief 设置OS引脚的工作模式..
  *@param eOSMode_t类型的值，代表OS引脚的工作模式.
  */
void setOSMode(eOSMode_t OSMode);
```

## Compatibility

MCU                | Work Well    | Work Wrong   | Untested    | Remarks
------------------ | :----------: | :----------: | :---------: | -----
Arduino Uno        |      √       |              |             | 
Mega2560        |      √       |              |             | 
Leonardo        |      √       |              |             | 
ESP32        |      √       |              |             | 
micro:bit        |      √       |              |             | 


## History

- data 2019-7-31
- version V0.1


## Credits

Written by fengli(li.feng@dfrobot.com), 2019.7.31 (Welcome to our [website](https://www.dfrobot.com/))






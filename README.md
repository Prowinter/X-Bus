主控采用国产兆易创新GD32E230G4U6TR芯片，尺寸3.5*2.5，成本极低，8通道输出，支持SBUS,PWM输出。另外还有多余的闲置IO口，可方便后续自定义开发。




> [GitHub仓库链接](https://github.com/Prowinter/X-Bus)
>
> [Futaba S-BUS controlled by mbed ](https://os.mbed.com/users/Digixx/notebook/futaba-s-bus-controlled-by-mbed/)

![](/4.DOC/images/model.gif)





## Sbus?

S.BUS是**FUTABA**提出的舵机控制总线，全称Serial Bus，别名S-BUS或SBUS，也称 Futaba S.BUS。





## Why Sbus?

PPM传输的是模拟信号，存在模拟信号所存在的通病，且根据PPM协议，PPM最多可传递八个通道。对于需要多通道的设备，PPM力不从心。Sbus则采用数字信号传递信号，最多可提供16位的比例通道和2位的数字通道。





## Sbus 协议要求

协议规定，信号必须反向，可是做硬件反向或作软件反向，最简单的方式是使用三极管的硬件反向。协议为 25 字节长，每 14 毫秒（模拟模式）或 7 毫秒（高速模式）发送一次。**一个字节 = 1 个起始位 + 8 个数据位 + 1 个奇偶校验位 + 2 个停止位 (8E2)，波特率 = 100'000 bit/s ，高位优先**。







## Sbus协议解析

由上文介绍，Sbus协议一个字节共有12位，由1 个起始位 + 8 个数据位 + 1 个奇偶校验位 + 2 个停止位 (8E2)组成，且波特率为100'000 bit/s 。 由于波特率限制为100'000 bit/s ，不方便使用软件模拟，可使用串口进行配置。配置时需要配置为，九位数据位，两位停止位和偶校验，一共12个位。即可正常通信。**每个通道数据占用11位`2^11=2048`**，数据解析和重构时需注意，以下为GD32串口初始化配置：

```c
    usart_deinit(USART0);
    usart_word_length_set(USART0, USART_WL_9BIT);
    usart_stop_bit_set(USART0, USART_STB_2BIT);
    usart_parity_config(USART0, USART_PM_EVEN);
    usart_baudrate_set(USART0, 100000U);
    usart_receive_config(USART0, USART_RECEIVE_ENABLE);
    usart_transmit_config(USART0, USART_TRANSMIT_ENABLE);
    usart_enable(USART0);
```





## NRF24L01

无线通信部分采用NRF24L01模组，该模块状态模式较为复杂，具体状态模式需要弄清楚以下状态图。理解收发模式状态跳转过程即可方便编程。详细编程指导参考该芯片数据手册即可。具体代码可以参考仓库内对于文件内容。
![](/4.DOC/images/state_diagram.png)





## 反向电路

三极管型号采用的SS8050 NPN型三极管
![](/4.DOC/images/hardware.png)
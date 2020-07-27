# 2.6	Zynq PL中断请求


## 实验要求

在PL端通过开关产生中断，PS端接受中断后通过串口打印出产生中断的开关号，以此了解Zynq的中断原理。

*参考《Zynq SoC修炼秘籍》*


## Zynq中断介绍

查阅ug585-Zynq-7000-TRM(Technical Reference Manual)的中断部分，了解到系统级的中断框图：

![system-level](https://raw.githubusercontent.com/wxiang357/Image/master/system_level.png)

* 中断类型：软件中断（Software Generated Interrupt, SGI，中断号0-15）（16–26 reserved）；私有外设中断（Private Peripheral Interrupt, PPI，中断号27-31）；共享外设中断（Shared Peripheral Interrupt, SPI，中断号32-95）；
* 私有外设中断（PPI）：每个CPU都有一组PPI，包括全局定时器、私有看门狗定时器、私有定时器和来自PL的IRQ（中断请求）/FIQ（快速中断请求）；
* 软件中断（SGI）：被路由到一个或者两个CPU上，通过写ICDSGIR寄存器产生SGI；
* 共享外设中断（SPI）：由PS和PL上的各种I/O控制器和存储器控制器产生，这些中断信号被路由到相应CPU；
* 通用中断控制器（GIC）是核心资源，用于集中管理从PS和PL产生的中断信号的资源集合。控制器可以使能、关使能、屏蔽中断源和改变中断源的优先级，并且会将中断送到对应的CPU中，CPU通过私有总线访问这些寄存器；
* 中断控制器（Interrupt Controller CPU，ICC）和中断控制器分配器（Interrupt Controller Distributor，ICD）是GIC寄存器子集；

下图展示了中断控制器的结构，比如PPI只能通过相应CPU Distributor发送到各自CPU，而本例子中PL到PS的中断经过ICD控制器分发后同时进入CPU0和CPU1.
![20200727203245](https://raw.githubusercontent.com/wxiang357/Image/master/20200727203245.png)

对Zynq的96个中断需要通过寄存器进行管理，可以根据51单片机推断需要什么控制寄存器：首先需要全局使能（屏蔽），随后具体到每一个中断ID的使能，以及优先级，还需要指定中断信号发送给哪个处理器。以下寄存器是主要考虑的：

* ICDDCR：开启或关闭中断（全局）；
* ICDICFR：配置中断触发模式（2bit），可以是01（高电平触发）或11（上升沿触发），最低为始终是1，因为只有一个CPU处理SPI中断；
* ICDIPR：中断优先级设置（8bit），数值越低优先级别越高，ICCPMR为接受中断的优先级门限；
* ICDIPTR：处理器选择，第0位为高选择CPU0，第一位为高选择CPU1
* ICDICER：中断屏蔽寄存器（1bit）；
* ICDISER：中断使能寄存器（1bit）；
* ICDICPR：清除中断标志位（1bit）；
* ICCIAR：通过读取该寄存器得到当前中断请求是哪个ID号发出的（10bit），要求哪个处理器执行的（3bit），读操作同时清除了读到中断号的标志位；
* ICCPMR：可执行中断优先级门限，默认是F0，即0-EF的优先级都被允许中断；
* ICCICR：指示FIQ或IRQ。

PL到PS有20个中断可以使用，本例子用到了16个SPI中的两个，中断号为61、62.
![20200727204850](https://raw.githubusercontent.com/wxiang357/Image/master/20200727204850.png)
![20200727205258](https://raw.githubusercontent.com/wxiang357/Image/master/20200727205258.png)


## 硬件电路搭建

vivado版本：2018.3
开发板：zedboard

![20200727211739](https://raw.githubusercontent.com/wxiang357/Image/master/20200727211739.png)

注意勾选使能中断：

![20200727212306](https://raw.githubusercontent.com/wxiang357/Image/master/20200727212306.png)

**利用tcl脚本还原block design：**

在vivado中进入`vivado/proj`文件夹，`source system.tcl`即可还原block design，随后添加`vivado/constraints`文件夹中的约束文件。


## 程序分析

SDK提供了大量的API，需要在bsp中查找函数和说明。

在standalone包含的boot.S中定义了中断表的位置：


```
.globl MMUTable
.global _prestart
.global _boot
.global __stack
.global __irq_stack
.global __supervisor_stack
.global __abort_stack
.global __fiq_stack
.global __undef_stack
.global _vector_table
```

_vector_table在asm_vector.S中可以找到：

```
.globl _vector_table

.section .vectors
_vector_table:
	B	_boot
	B	Undefined
	B	SVCHandler
	B	PrefetchAbortHandler
	B	DataAbortHandler
	NOP	/* Placeholder for address exception vector*/
	B	IRQHandler
	B	FIQHandler
```

这个中断表在C程序中是一个结构数组，在standalone/xil_exception.c中定义，下标为5的位置存放中断时希望执行的函数句柄：

```
XExc_VectorTableEntry XExc_VectorTable[XIL_EXCEPTION_ID_LAST + 1] =
{
	{Xil_ExceptionNullHandler, NULL},
	{Xil_UndefinedExceptionHandler, NULL},
	{Xil_ExceptionNullHandler, NULL},
	{Xil_PrefetchAbortHandler, NULL},
	{Xil_DataAbortHandler, NULL},
	{Xil_ExceptionNullHandler, NULL},
	{Xil_ExceptionNullHandler, NULL},
};
```

关键函数分析如下：

![20200727222508](https://raw.githubusercontent.com/wxiang357/Image/master/20200727222508.png)

其他内容参考米联客教程。

## 测试结果

拨动SW0和SW1两个开关，从串口中观察到开关号被打印，说明中断被正确触发并处理。

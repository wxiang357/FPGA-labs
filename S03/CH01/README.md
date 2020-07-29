# 3.1	DMA Loop环路测试

## 实验要求

SDK驱动DMA向DDR发送数据，随后通过DMA读取数据，对比读取和发送的数据是否一致。

参考[MiZ-S02-SoC-base.pdf](../../reference/MiZ-S02-SoC-base.pdf)

## AXI DMA原理

AXI DMA为内存与AXI4-Stream外设之间提供高带宽的直接存储访问，其可选的scatter/gather功能可将CPU从数据搬移任务中解放出来。在ZYNQ中，AXI DMA就是FPGA访问DDR3的桥梁，该过程受ARM的监控和管理，详细内容参考[ZYNQ AXIDMA详解（一）](https://www.cnblogs.com/batianhu/p/9293972.html)。

AXI DMA工作模式分为两种，分别是Direct Register Mode和Scatter/Gather Mode。Direct Register Mode具备DMA的基本功能，除了控制寄存器和状态寄存器之外，给出源（目的）地址和传输长度之后就可以开启一次传输。Direct Register Mode的特点（也是缺点）是配置完一次寄存器之后只能完成存储器连续地址空间的读写，如果有需求往不同地址空间搬运数据的话，那就需要重新配置寄存器开启一次新的传输。本例子采用这种工作模式。

## 硬件电路

vivado版本：2018.3
开发板：zedboard

S2MM通路是向内存写数据通路，MM2S是读数据通路。

![20200728211314](https://raw.githubusercontent.com/wxiang357/Image/master/20200728211314.png)

选取Zedboard对应的DDR型号：

![20200728211646](https://raw.githubusercontent.com/wxiang357/Image/master/20200728211646.png)

选择一路HP通道，作为PL访问DDR通路：

![20200728211845](https://raw.githubusercontent.com/wxiang357/Image/master/20200728211845.png)

勾选PL到PS的中断源：

![20200728212323](https://raw.githubusercontent.com/wxiang357/Image/master/20200728212323.png)

VDMA配置：

![20200728212032](https://raw.githubusercontent.com/wxiang357/Image/master/20200728212032.png)

## SDK程序分析

首先初始化中断设置：

* `DMA_Intr_Init`初始化DMA，主要包括了`XAxiDma_LookupConfig`和`XAxiDma_LookupConfig`；
* `Init_Intr_System`初始化中断，主要包括`XScuGic_LookupConfig`，`XScuGic_LookupConfig`；
* `Setup_Intr_Exception`注册基本中断函数并使能中断，主要包括`Xil_ExceptionInit`，`Xil_ExceptionRegisterHandler`，`Xil_ExceptionEnable`；
* `DMA_Setup_Intr_System`设置DMA中断，主要包括`XScuGic_SetPriorityTriggerType`，`XScuGic_Connect`，`XScuGic_Enable`；
* `DMA_Intr_Enable`使能DMA中断，主要包括`XAxiDma_IntrEnable`。

了解中断可以参考[PL中断实验](../../S02/CH06/)。


```C++
int init_intr_sys(void)
{
    DMA_Intr_Init(&AxiDma,0);//initial interrupt system
    Init_Intr_System(&Intc); // initial DMA interrupt system
    Setup_Intr_Exception(&Intc);
    DMA_Setup_Intr_System(&Intc,&AxiDma,TX_INTR_ID,RX_INTR_ID);//setup dma interrpt system
    DMA_Intr_Enable(&Intc,&AxiDma);
}
```

在`axi_dma_test`测试函数中，初始化要发送的数组：

```C++
for(Index = 0; Index < MAX_PKT_LEN; Index ++) {
    TxBufferPtr[Index] = Value;
    Value = (Value + 1) & 0xFF;
}
```

为了是PL获得的数据是内存中最新的（缓存一致性问题），需要将Cache的数据刷入内存：

```C++
Xil_DCacheFlushRange((u32)TxBufferPtr, MAX_PKT_LEN);
```

开启数据传输，包括读写操作：

```C++
Status = XAxiDma_SimpleTransfer(&AxiDma,(u32) TxBufferPtr,
		    MAX_PKT_LEN, XAXIDMA_DMA_TO_DEVICE);

if (Status != XST_SUCCESS) {
	return XST_FAILURE;
}

Status = XAxiDma_SimpleTransfer(&AxiDma,(u32) RxBufferPtr,
					MAX_PKT_LEN, XAXIDMA_DEVICE_TO_DMA);

if (Status != XST_SUCCESS) {
	return XST_FAILURE;
}

/*
 * Wait TX done and RX done
 */
while (!TxDone || !RxDone) {
		/* NOP */
}
```

DMA的读写都是针对内存的操作，`XAXIDMA_DMA_TO_DEVICE`代表数据从内存到AXI-Stream设备，本例中是FIFO；`XAXIDMA_DEVICE_TO_DMA`即代表数据从AXI-Stream设备到内存，因此以上代码先读取内存数据到FIFO，再写回内存。

两次数据传输都结束后，进行数据对比，判断是否发生错误，没有错误则成功次数加1，否则直接返回。

## 测试结果

传输大小为2MB，测试100000次通过。

![20200729151420](https://raw.githubusercontent.com/wxiang357/Image/master/20200729151420.png)


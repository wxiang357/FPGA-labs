
# 3.5	DMA lwIP以太网传输

## 实验要求

PS通过GPIO启动PL循环构造16bit的0-1023数据，利用AXI DMA将数据传输到DDR的乒乓缓存中。PL每发完一组数据，DMA便会产生一个中断信号，PS得到中断信号后将DDR缓存的数据通过乒乓操作的方式由TCP协议发送至PC。

## lwIP简介

lwIP(Light Weight Internet Protoco)实现了轻量级的TCP/IP协议栈，可以利用SDK开发环境在Zynq上使用。lwIP库提供了两种API，具体参考XAPP1026：

* Raw API：没有操作系统时使用的基本API，基于事件驱动，当触发接收和发送等事件时调用多层回调函数；
* Socket API：需要支持多线程操作系统的支持，牺牲了一部分效率，函数有更好的封装。

SDK提供了和lwIP相关的程序模板，本实验需要搭建TCP Client，使用Raw API。

![20200805144723](https://raw.githubusercontent.com/wxiang357/Image/master/20200805144723.png)

## 硬件电路

![20200805145320](https://raw.githubusercontent.com/wxiang357/Image/master/20200805145320.png)

DMA只需要配置写通道，注意启用PS部分的以太网接口:

![20200805145852](https://raw.githubusercontent.com/wxiang357/Image/master/20200805145852.png)

PL部分通过AXI Stream协议向FIFO循环发送数据。

## lwIP库配置

参考[lwIP库的配置与使用](https://blog.csdn.net/FPGADesigner/article/details/88689771)

在BSP Settings中勾选lwip库，在2018.3中对应lwip202，和lwip141有所不同。

![20200805152537](https://raw.githubusercontent.com/wxiang357/Image/master/20200805152537.png)

随后进行参数设置，将use_axieth_on_zynq，use_emaclite_on_zynq设为0：

![20200805153742](https://raw.githubusercontent.com/wxiang357/Image/master/20200805153742.png)

为了提高传输效率进行以下设置：

![20200805155318](https://raw.githubusercontent.com/wxiang357/Image/master/20200805155318.png)

![20200805155409](https://raw.githubusercontent.com/wxiang357/Image/master/20200805155409.png)

![20200805155440](https://raw.githubusercontent.com/wxiang357/Image/master/20200805155440.png)

![20200805160114](https://raw.githubusercontent.com/wxiang357/Image/master/20200805160114.png)

使用Raw API时一般有类似下方的代码结构：

```C++
int main() 
{
    struct netif *netif, server_netif; 
    ip_addr_t ipaddr, netmask, gw;
    //板子的MAC地址
    unsigned char mac_ethernet_address[] = 
        {0x00, 0x0a, 0x35, 0x00, 0x01, 0x02};
    
    lwip_init();
    
    //把网络接口添加到netif_list, 并设为默认
    if (!xemac_add(netif, &ipaddr, &netmask, 
        &gw, mac_ethernet_address, EMAC_BASEADDR)) {
        printf(“Error adding N/W interface\n\r”); 
        return -1;
    }
    netif_set_default(netif);

    platform_enable_interrupts();   //使能中断
    netif_set_up(netif);  //指定网络是否打开
    start_application();  //启动应用程序，设置回调
    
    
    while (1) {
        xemacif_input(netif); // 接收数据包
        transfer_data();  //执行应用程序的特定功能
    }
}
```

## SDK程序分析

`init_intr_sys`主要进行定时器和DMA的中断设置，其中定时器设置每250ms触发一次中断，用于超时重传。DMA的中断回调函数中将`packet_trans_done`置1，表示PL产生的数据已经写到DDR的缓冲区，在主循环中检测到`packet_trans_done`为1则开始TCP发送。

```C++
int init_intr_sys(void)
{
    DMA_Intr_Init(&AxiDma,0);
    Timer_init(&Timer,TIMER_LOAD_VALUE,0);
    Init_Intr_System(&Intc);
    Setup_Intr_Exception(&Intc);
    DMA_Setup_Intr_System(&Intc,&AxiDma,0,RX_INTR_ID);
    Timer_Setup_Intr_System(&Intc,&Timer,TIMER_IRPT_INTR);
    DMA_Intr_Enable(&Intc,&AxiDma);
}
```

在`tcp_send_init`中首先创建用于TCP控制的PCB（TCP protocol control block）数据结构，随后建立TCP连接，并设置回调函数。

注意在lwip202中类型`ip_addr`由`ip_addr_t`代替：

```C++
int tcp_send_init()
{
    struct tcp_pcb *pcb;
    ip_addr_t ipaddr;
    err_t err;
    u16_t port;

    /* create new TCP PCB structure */
    pcb = tcp_new();
    if (!pcb) {
        xil_printf("txperf: Error creating PCB. Out of Memory\r\n");
        return -1;
    }

    /* connect to iperf tcp server */
    IP4_ADDR(&ipaddr,  192, 168,   1, 209);		/* iperf server address */

    port = 7;					/* iperf default port */

    tcp_client_connected = 0;
    first_trans_start = 0;
    packet_trans_done = 0;
    packet_index = 0;

    err = tcp_connect(pcb, &ipaddr, port, tcp_connected_callback);
    if (err != ERR_OK) {
        xil_printf("txperf: tcp_connect returned error: %d\r\n", err);
        return err;
    }

    return 0;
}
```

`tcp_connected_callback`在成功建立TCP连接后调用，其中设置发送完成的回调函数`tcp_sent_callback`：

```C++
tcp_sent(tpcb, tcp_sent_callback);
```

`tcp_sent_callback`中将传输次数加1。

最后在主循环的`send_received_data`函数中将乒乓缓存中的数据发送到上位机，并设置下一次DMA传输的地址。在乒乓操作中，两个缓冲区交替进行数据接收。

## 测试结果

上板运行后通过网络调试助手观察数据接收情况，同时可以查看网络流量。

![eth](https://raw.githubusercontent.com/wxiang357/Image/master/eth.PNG)
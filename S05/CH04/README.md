# 5.4 网口传输的视频图像处理系统

## 实验要求

在本地通过OpenCV读取视频或图像，通过socket发送图像数据。在PS端建立TCP Server接收数据，通过硬件完成图像处理后在屏幕上显示。

**说明：** 本实验以[S03_CH05](../../S03/CH05/)和[S05_CH03](../CH03/)为基础，思考图像和视频处理通路。

## 硬件搭建

![20200805182853](https://raw.githubusercontent.com/wxiang357/Image/master/20200805182853.png)

其中rgb2vga使用Digilent提供的IP，整个系统和[S05_CH03](../CH03/)基本相同。

## 视频图像系统框架

本实验搭建了简单的视频图像系统，为研究复杂系统打好基础。

视频系统可以使用三缓冲或乒乓缓冲，适用于读写同时进行并且需要协调读写速率，内存划分如下所示，考虑到显示的方便和可扩展，显示区域被单独分为一部分，待显示的内容需要搬运到这里；为了方便调整图像格式以及配置图像处理模块，缓冲区中的数据经过初步处理后存放到图像处理模块的读区域中，这样图像处理模块只需要读写固定区域，同时图像数据有预处理的机会。

![20200805191249](https://raw.githubusercontent.com/wxiang357/Image/master/20200805191249.png)

为了防止读写速率不同造成同时读写同一块缓冲区，可以将每个缓冲区分为两个状态：状态0只允许写操作，状态1只允许读操作，也就是写完才可以读，读完才可以写。在写操作等待读操作完成期间，所有数据需要被丢弃，同时需要注意丢弃的数据必须是一整帧，这样保证了下一帧的完整性。

三缓冲区需要的全局变量如下：

```C++
u8 frame_buffer[3][IMAGE_SIZE];
enum STATE {STATE0, STATE1} buffer_state[3];
int write_index, read_index;
```

只进行图像处理可以将三缓冲区换成一个缓冲区。


## lwIP库配置

参考[lwIP库的配置与使用](https://blog.csdn.net/FPGADesigner/article/details/88689771)

在BSP Settings中勾选lwip库，在2018.3中对应lwip202，和lwip141有所不同。

![20200805152537](https://raw.githubusercontent.com/wxiang357/Image/master/20200805152537.png)

随后进行参数设置，将use_axieth_on_zynq，use_emaclite_on_zynq设为0：

![20200805153742](https://raw.githubusercontent.com/wxiang357/Image/master/20200805153742.png)

为了提高传输效率进行以下设置：

![20200806135928](https://raw.githubusercontent.com/wxiang357/Image/master/20200806135928.png)

![20200806135955](https://raw.githubusercontent.com/wxiang357/Image/master/20200806135955.png)

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

## TCP Server

建立SDK自带的lwIP echo demo，`start_application`函数绑定、监听端口，并设置回调函数：

```C++
int start_application()
{
    struct tcp_pcb *pcb;
    err_t err;
    unsigned port = 7;

    /* create new TCP PCB structure */
    pcb = tcp_new_ip_type(IPADDR_TYPE_ANY);
    if (!pcb) {
        xil_printf("Error creating PCB. Out of Memory\n\r");
        return -1;
    }

    /* bind to specified @port */
    err = tcp_bind(pcb, IP_ANY_TYPE, port);
    if (err != ERR_OK) {
        xil_printf("Unable to bind to port %d: err = %d\n\r", port, err);
        return -2;
    }

    /* we do not need any arguments to callback functions */
    tcp_arg(pcb, NULL);

    /* listen for connections */
    pcb = tcp_listen(pcb);
    if (!pcb) {
        xil_printf("Out of memory while tcp_listen\n\r");
        return -3;
    }

    /* specify callback to use for incoming connections */
    tcp_accept(pcb, accept_callback);

    xil_printf("TCP echo server started @ port %d\n\r", port);

    return 0;
}
```

其中`accept_callback`作为接受连接后的回调函数，又进一步设置了回调函数`recv_callback`：

```C++
/* set the receive callback for this connection */
    tcp_recv(newpcb, recv_callback);
```

在`recv_callback`中将接收的数据放到缓冲区中：

```C++
    memcpy(temp_frame + num, (unsigned char *)p->payload, p->len);
    num += p->len;

    if (num >= IMAGE_SIZE) {
        num = 0;
        if (buffer_state[write_index] == STATE0) {
            memcpy(frame_buffer[write_index], temp_frame, IMAGE_SIZE);
            buffer_state[write_index] = STATE1;
            write_index = (write_index + 1) % 3;
        }
    }
```

以上程序接收所有数据，当恰好接收一帧图片并且可以写入当前缓冲区时，将这帧图片写入缓冲区，如果接收一帧图片后发现不能写入则丢弃该帧。

注意在主循环中需要尽量减少操作，以保证足够的网速：

```C++
    /* receive and process packets */
    while (1) {
        if (TcpFastTmrFlag) {
            tcp_fasttmr();
            TcpFastTmrFlag = 0;
        }
        if (TcpSlowTmrFlag) {
            tcp_slowtmr();
            TcpSlowTmrFlag = 0;
        }
        xemacif_input(echo_netif);

        if (buffer_state[read_index] == STATE1) {
            SOBEL_Setup();
            show_img(522,0,DISPLAY_MM2S,(void*)SOBEL_MM2S,512,512,1);
            buffer_state[read_index] = STATE0;
//			xil_printf("read index: %d\n", read_index);
            read_index = (read_index + 1) % 3;
        }

        if (XHls_sobel_IsDone(&sobel)) {
            show_img(0,0,DISPLAY_MM2S,(void*)SOBEL_S2MM,512,512,0);
//			xil_printf("sobel done.\n");
        }
    }
```

## TCP Client

TCP客户端主要通过OpenCV开启摄像头或读取图片，通过socket发送图片数据，这里设置相邻两帧间隔10ms发送：

```python
import socket
import cv2
import numpy
import sys
import time

def send_video():
    address = ('192.168.1.10', 7)
    try:
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.connect(address)

    except socket.error as msg:
        print(msg)
        sys.exit(1)

    capture = cv2.VideoCapture(0)
    ret, frame = capture.read()

    while ret:
        frame = cv2.resize(frame, (512, 512))
        if cv2.waitKey(10) >= 0:
            break
        sock.send(frame)
        ret, frame = capture.read()


if __name__ == '__main__':
    send_video()
```

## 测试结果

使用网线连接开发板和PC，并将开发板连接VGA显示器，运行程序，等待网口自动配置好之后串口将打印信息：

![20200805203209](https://raw.githubusercontent.com/wxiang357/Image/master/20200805203209.png)

运行TCP客户端程序，在显示器观察到摄像头得到的视频帧以及边缘检测后的视频帧。
# 5.3	图像处理实例

## 实验要求

通过HLS开发边缘检测等图像处理IP，并基于VDMA搭建完整的软硬件系统。

参考[MiZ-S05-HLS.pdf](../../reference/MiZ-S05-HLS.pdf)

## HLS实现

图像处理部分采用HLS提供的OpenCV函数来加速软件的OpenCV应用，通过`#include "hls_video.h"`使用可综合的OpenCV函数，这些函数与软件OpenCV函数有类似的接口，具体通过[how_to_accelerate_opencv_applications_using_vivado_hls](https://www.macrogroup.ru/sites/default/files/uploads/files_and_docs/how_to_accelerate_opencv_applications_using_vivado_hls.pdf)了解具体开发流程。

![20200728053221](https://raw.githubusercontent.com/wxiang357/Image/master/20200728053221.png)

首先定义数据类型：

```C++
typedef hls::stream<ap_axiu<32,1,1,1> >               AXI_STREAM;
typedef hls::Scalar<3, unsigned char>                 RGB_PIXEL;
typedef hls::Mat<MAX_HEIGHT, MAX_WIDTH, HLS_8UC3>     RGB_IMAGE;
```

定义函数接口，指明数据传输采用AXI-Stream接口，rows、cols以及模块控制采用AXI-Lite协议，rows和cols变量在模块工作中不发生改变：

```C++
void hls_sobel(AXI_STREAM& INPUT_STREAM, AXI_STREAM& OUTPUT_STREAM, int rows, int cols)
{
//Create AXI streaming interfaces for the core
#pragma HLS INTERFACE axis port=INPUT_STREAM
#pragma HLS INTERFACE axis port=OUTPUT_STREAM

#pragma HLS INTERFACE s_axilite port=rows
#pragma HLS INTERFACE s_axilite port=cols
#pragma HLS INTERFACE s_axilite port=return

#pragma HLS INTERFACE ap_stable port=rows
#pragma HLS INTERFACE ap_stable port=cols

    RGB_IMAGE img_0(rows, cols);
    RGB_IMAGE img_1(rows, cols);
    RGB_IMAGE img_2(rows, cols);
    RGB_IMAGE img_3(rows, cols);
    RGB_IMAGE img_4(rows, cols);
    RGB_IMAGE img_5(rows, cols);
    RGB_IMAGE img_6(rows, cols);
    RGB_IMAGE img_7(rows, cols);
    RGB_IMAGE img_8(rows, cols);
    RGB_IMAGE img_9(rows, cols);
    RGB_PIXEL pix(50, 50, 50);

#pragma HLS dataflow
    hls::AXIvideo2Mat(INPUT_STREAM, img_0);
    hls::Duplicate(img_0, img_1, img_2);

    hls::Sobel<0,1,3>(img_1, img_3);
    hls::Sobel<1,0,3>(img_2, img_4);

    hls::AddWeighted(img_3, 1, img_4, 1, 0, img_5);

    hls::SubS(img_5, pix, img_6);
    hls::Scale(img_6, img_7, 2, 0);
    hls::Erode(img_7, img_8);
    hls::Dilate(img_8, img_9);
    hls::Mat2AXIvideo(img_9, OUTPUT_STREAM);
}
```

在算法方面，将横向和纵向的检测结果相加，得到两个方向的检测效果，随后进行减去阈值，线性变换，腐蚀和膨胀操作。

## 硬件电路搭建

硬件电路采用基本的视频图像处理架构：视频或图像通过处理器的HP接口将数据写入DDR；HLS模块通过VDMA读取内存数据，处理后写回内存；Display Controller完成显示功能。

![20200728055147](https://raw.githubusercontent.com/wxiang357/Image/master/20200728055147.png)

硬件框图如下，其中vdma_1开启读写通道，负责从内存读取数据给HLS模块处理，并将处理后的结果写回内存；vdma_0只开启读通道，负责将内存数据发送给显示模块。Video Timing Controller产生视频显示需要的时序，AXI4-Stream to Video Out将数据流转换为显示需要的格式，可以直接用于VGA的驱动。

**如果要驱动HDMI需要使用Diligent或Zedboard网站提供的IP，** [S05/CH03/repo](./repo)文件夹提供了这两个IP。Zybo开发板采用IO驱动HDMI，因此用`rgb to dvi`IP核，该IP核输出TMDS信号，TMDS原理参考[基于FPGA的高清显示接口驱动](https://www.cnblogs.com/ninghechuan/archive/2018/01/26/8353827.html)；Zedboard采用ADV7511芯片驱动HDMI，需要使用`zed_hdmi_out`IP核。

![20200728055653](https://raw.githubusercontent.com/wxiang357/Image/master/20200728055653.png)

![20200728060035](https://raw.githubusercontent.com/wxiang357/Image/master/20200728060035.png)

![20200728060104](https://raw.githubusercontent.com/wxiang357/Image/master/20200728060104.png)

**恢复block design：**
在vivado Tcl Console中`source system.tcl`，随后手动添加提供的`system_wrapper.v`，以及约束文件。

**注意：为了保证烧写位流时不会发生问题，所有管脚必须被正确分配，否则可能会自动分配到其他位置使jtag无法正常烧写。参考[No devices detected on target](https://forums.xilinx.com/t5/FPGA-Configuration/Labtools-27-3165-error-but-FPGA-is-programmed/td-p/917729)**

## SDK程序分析

基本包括配置和初始化VDMA，HLS IP，随后启动工作。

进行地址定义，配置VDMA寄存器时需要基地址：

```C++
#define DDR_BASEADDR       0x00000000
#define DISPLAY_VDMA       XPAR_AXI_VDMA_0_BASEADDR + 0
#define SOBEL_VDMA         XPAR_AXI_VDMA_1_BASEADDR + 0
```

在DDR中分配3段地址空间，分别存放HLS IP读写地址和显示内容的位置。

```C++
#define SOBEL_S2MM      0x08000000
#define SOBEL_MM2S      0x0A000000
#define DISPLAY_MM2S    0x0C000000
```

配置VDMA寄存器示例，具体参考[MiZ-S02-SoC-base.pdf](../../reference/MiZ-S02-SoC-base.pdf)：

```C++
void SOBEL_VDMA_setting(unsigned int width,unsigned int height,unsigned int s2mm_addr,unsigned int mm2s_addr)
{
        //S2MM
        Xil_Out32(SOBEL_VDMA + 0x30, 0x4); //reset   S2MM VDMA Control Register
        usleep(10);
        Xil_Out32(SOBEL_VDMA + 0x30, 0x0); //genlock
        Xil_Out32(SOBEL_VDMA + 0xAC,   s2mm_addr);//S2MM Start Addresses
        Xil_Out32(SOBEL_VDMA + 0xAC+4, s2mm_addr);
        Xil_Out32(SOBEL_VDMA + 0xAC+8, s2mm_addr);
        Xil_Out32(SOBEL_VDMA + 0xA4, width*4);//S2MM Horizontal Size
        Xil_Out32(SOBEL_VDMA + 0xA8, width*4);//S2MM Frame Delay and Stride
        Xil_Out32(SOBEL_VDMA + 0x30, 0x3);//S2MM VDMA Control Register
        Xil_Out32(SOBEL_VDMA + 0xA0, height);//S2MM Vertical Size  start an S2M

        //MM2S
        Xil_Out32(SOBEL_VDMA + 0x00,0x00000003); // enable circular mode
        Xil_Out32(SOBEL_VDMA + 0x5c,mm2s_addr); // start address
        Xil_Out32(SOBEL_VDMA + 0x60,mm2s_addr); // start address
        Xil_Out32(SOBEL_VDMA + 0x64,mm2s_addr); // start address
        Xil_Out32(SOBEL_VDMA + 0x58,(width*4)); // h offset
        Xil_Out32(SOBEL_VDMA + 0x54,(width*4)); // h size
        Xil_Out32(SOBEL_VDMA + 0x50,height); // v size
}
```

以下函数将两部分内存中的数据搬运到`DISPLAY_MM2S`内存区域，使处理后的图像占用屏幕左边512x512大小的空间，原图占用右边512x512大小的空间。

```C++
void show_img(u32 x, u32 y, u32 disp_base_addr, const unsigned char * addr, u32 size_x, u32 size_y,u32 type)
{
    u32 i=0;
    u32 j=0;
    u32 r,g,b;
    u32 start_addr=disp_base_addr;
    start_addr = disp_base_addr + 4*x + y*4*DIS_X;
    for(j=0;j<size_y;j++)
    {
        for(i=0;i<size_x;i++)
        {
            if(type==0)
            {
                b = *(addr+(i+j*size_x)*4+2);
                g = *(addr+(i+j*size_x)*4+1); 
                r = *(addr+(i+j*size_x)*4); 
            }
            else
            {
                b = *(addr+(i+j*size_x)*4+1); 
                g = *(addr+(i+j*size_x)*4+2); 
                r = *(addr+(i+j*size_x)*4+3); 
            }
            Xil_Out32((start_addr+(i+j*DIS_X)*4),((r<<16)|(g<<8)|(b<<0)|0x0));
        }
    }
    Xil_DCacheFlush();
}

// ...
show_img(0,0,DISPLAY_MM2S,(void*)SOBEL_S2MM,512,512,0);
show_img(522,0,DISPLAY_MM2S,(void*)SOBEL_MM2S,512,512,1);

```

## 测试结果

![wechat_20200728074704](https://raw.githubusercontent.com/wxiang357/Image/master/wechat_20200728074704.jpg)

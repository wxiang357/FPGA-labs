/*
 * sobel.h
 *
 *  Created on: 2020Äê8ÔÂ2ÈÕ
 *      Author: wx
 */

#ifndef SRC_SOBEL_H_
#define SRC_SOBEL_H_

#include "xaxivdma.h"
#include "xaxivdma_i.h"
#include "xhls_sobel.h"
#include "sleep.h"


#define DDR_BASEADDR       0x00000000
#define DISPLAY_VDMA       XPAR_AXI_VDMA_0_BASEADDR + 0
#define SOBEL_VDMA		   XPAR_AXI_VDMA_1_BASEADDR + 0
#define DIS_X              1280
#define DIS_Y              720
#define SOBEL_ROW		   512
#define SOBEL_COL		   512
#define pi				   3.14159265358
#define COUNTS_PER_SECOND  (XPAR_CPU_CORTEXA9_CORE_CLOCK_FREQ_HZ)/64
#define IMAGE_SIZE         512*512*3

#define SOBEL_S2MM		0x08000000
#define SOBEL_MM2S		0x0A000000
#define DISPLAY_MM2S	0x0C000000

enum STATE {STATE0, STATE1} buffer_state[3];
int write_index, read_index;
int transfer_done;
u8 frame_buffer[3][IMAGE_SIZE];
XHls_sobel sobel;

void Xil_DCacheFlush();

void SOBEL_VDMA_setting(unsigned int width,unsigned int height,unsigned int s2mm_addr,unsigned int mm2s_addr);

void DISPLAY_VDMA_setting(unsigned int width,unsigned height,unsigned int mm2s_addr);

void SOBEL_DDRWR(unsigned int addr,unsigned int cols,unsigned int lows);

void SOBEL_Setup();

void Set_blackground(u32 size_x,u32 size_y,u32 disp_addr);

void show_img(u32 x, u32 y, u32 disp_base_addr, const unsigned char * addr, u32 size_x, u32 size_y,u32 type);

#endif /* SRC_SOBEL_H_ */

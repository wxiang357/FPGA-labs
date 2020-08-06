/*
 * sobel.c
 *
 *  Created on: 2020Äê8ÔÂ2ÈÕ
 *      Author: wx
 */



#include "xaxivdma.h"
#include "xaxivdma_i.h"
#include "xhls_sobel.h"
#include "sleep.h"
#include "sobel.h"


void SOBEL_VDMA_setting(unsigned int width,unsigned int height,unsigned int s2mm_addr,unsigned int mm2s_addr)
{
	    //S2MM
		Xil_Out32(SOBEL_VDMA + 0x30, 0x4); //reset   S2MM VDMA Control Register
	    usleep(10);
	    Xil_Out32(SOBEL_VDMA + 0x30, 0x0); //genlock
	    Xil_Out32(SOBEL_VDMA + 0xAC,   s2mm_addr);//S2MM Start Addresses
//	    Xil_Out32(SOBEL_VDMA + 0xAC+4, s2mm_addr);
//	    Xil_Out32(SOBEL_VDMA + 0xAC+8, s2mm_addr);
	    Xil_Out32(SOBEL_VDMA + 0xA4, width*4);//S2MM Horizontal Size
	    Xil_Out32(SOBEL_VDMA + 0xA8, width*4);//S2MM Frame Delay and Stride
	    Xil_Out32(SOBEL_VDMA + 0x30, 0x3); // start
	    Xil_Out32(SOBEL_VDMA + 0xA0, height);//S2MM Vertical Size  start an S2M

		//MM2S
	    Xil_Out32(SOBEL_VDMA + 0x00,0x00000003); // enable circular mode
	    Xil_Out32(SOBEL_VDMA + 0x5c,mm2s_addr); // start address
//	    Xil_Out32(SOBEL_VDMA + 0x60,mm2s_addr); // start address
//	    Xil_Out32(SOBEL_VDMA + 0x64,mm2s_addr); // start address
	    Xil_Out32(SOBEL_VDMA + 0x58,(width*4)); // h offset
	    Xil_Out32(SOBEL_VDMA + 0x54,(width*4)); // h size
	    Xil_Out32(SOBEL_VDMA + 0x50,height); // v size

}

void DISPLAY_VDMA_setting(unsigned int width,unsigned height,unsigned int mm2s_addr)
{
		Xil_Out32((DISPLAY_VDMA + 0x00), 0x00000003); 		// enable circular mode
		Xil_Out32((DISPLAY_VDMA + 0x05c), mm2s_addr); 	// start address
//		Xil_Out32((DISPLAY_VDMA + 0x060), mm2s_addr); 	// start address
//		Xil_Out32((DISPLAY_VDMA + 0x064), mm2s_addr); 	// start address
		Xil_Out32((DISPLAY_VDMA + 0x058), (width*4)); 		// h offset (640 * 4) bytes
		Xil_Out32((DISPLAY_VDMA + 0x054), (width*4)); 		// h size (640 * 4) bytes
		Xil_Out32((DISPLAY_VDMA + 0x050), height); 			// v size (480)

}

void SOBEL_DDRWR(unsigned int addr,unsigned int rows,unsigned int cols)
{
	u32 i=0;
	u32 j=0;
	u32 r,g,b;
	for(i=0;i<rows;i++)
	{
		for(j=0;j<cols;j++)
		{
			b = frame_buffer[read_index][(j+i*cols)*3+0];
			g = frame_buffer[read_index][(j+i*cols)*3+1];
			r = frame_buffer[read_index][(j+i*cols)*3+2];
			Xil_Out32((addr+(j+i*cols)*4),((r<<24)|(g<<16)|(b<<8)|0x00));
		}
	}
	Xil_DCacheFlush();
}

void SOBEL_DDRWR2(unsigned int addr,unsigned int rows,unsigned int cols) {

}

void SOBEL_Setup()
{
	//const int cols = 512;
	//const int rows = 512;
	XHls_sobel_Set_rows(&sobel, SOBEL_COL);
	XHls_sobel_Set_cols(&sobel, SOBEL_ROW);
	XHls_sobel_DisableAutoRestart(&sobel);
	XHls_sobel_InterruptGlobalDisable(&sobel);
	SOBEL_VDMA_setting(SOBEL_ROW,SOBEL_COL,SOBEL_S2MM,SOBEL_MM2S);
	SOBEL_DDRWR(SOBEL_MM2S,SOBEL_ROW,SOBEL_COL);
	//init_hls_sobel_dma(cols,rows, VIDEO_BASEADDR, HLS_VDMA_MM2S_ADDR);
	//DDRVideoWr(HLS_VDMA_MM2S_ADDR, cols,rows);
	XHls_sobel_Start(&sobel);
}

void Set_blackground(u32 size_x,u32 size_y,u32 disp_addr)
{
	u32 i=0;
	u32 j=0;
	//u32 r,g,b;
	for(j=0;j<size_y;j++)
	{
		for(i=0;i<size_x;i++)
		{
			Xil_Out32((disp_addr+(i+j*size_x)*4),0);
		}
	}
	Xil_DCacheFlush();
}

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


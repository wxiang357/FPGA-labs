/*
 * tcp_transmission.c
 *
 *  Created on: 2016��9��7��
 *      Author: Michael Wang
 */


#include <stdio.h>
#include <string.h>

#include "lwip/err.h"
#include "lwip/tcp.h"
#include "lwipopts.h"
#include "xaxidma.h"
#include "xil_cache.h"
#include "xil_printf.h"
#include "sleep.h"


#define SEND_SIZE (2048)
#define PAKET_LENGTH (2048)

static struct tcp_pcb *connected_pcb = NULL;

volatile unsigned tcp_client_connected = 0;
static int tcp_trans_done = 0;
static unsigned first_trans_start = 0;

static u32_t packet_index = 0;

extern XAxiDma AxiDma;
extern u16 *RxBufferPtr[2];
extern u8_t packet_trans_done;

void send_received_data()
{
#if __arm__
	int copy = 3;
#else
	int copy = 0;
#endif
	err_t err;
	int Status;
	struct tcp_pcb *tpcb = connected_pcb;

	/*initial the first axdma transmission, only excuse once*/
	if(!first_trans_start)
	{
		Status = XAxiDma_SimpleTransfer(&AxiDma, (u32)RxBufferPtr[0],
				(u32)(PAKET_LENGTH), XAXIDMA_DEVICE_TO_DMA);
		if (Status != XST_SUCCESS)
		{
			xil_printf("axi dma failed! 0 %d\r\n", Status);
			return;
		}
		/*set the flag, so this part of code will not excuse again*/
		first_trans_start = 1;
	}

	/*if the last axidma transmission is done, the interrupt triggered, then start TCP transmission*/
	if(packet_trans_done)
	{

		if (!connected_pcb)
			return;

		/* if tcp send buffer has enough space to hold the data we want to transmit from PL, then start tcp transmission*/
		if (tcp_sndbuf(tpcb) > SEND_SIZE)
		{
			/*transmit received data through TCP*/
			err = tcp_write(tpcb, RxBufferPtr[packet_index & 1], SEND_SIZE, copy);
			if (err != ERR_OK) {
				xil_printf("txperf: Error on tcp_write: %d\r\n", err);
				connected_pcb = NULL;
				return;
			}
			err = tcp_output(tpcb);
			if (err != ERR_OK) {
				xil_printf("txperf: Error on tcp_output: %d\r\n",err);
				return;
			}

			packet_index++;
			/*clear the axidma done flag*/
			packet_trans_done = 0;

			/*initial the other axidma transmission when the current transmission is done*/
			Status = XAxiDma_SimpleTransfer(&AxiDma, (u32)RxBufferPtr[(packet_index + 1)&1],
						(u32)(PAKET_LENGTH), XAXIDMA_DEVICE_TO_DMA);
			if (Status != XST_SUCCESS)
			{
				xil_printf("axi dma %d failed! %d \r\n", (packet_index + 1), Status);
				return;
			}

		}
	}
}

/*this fuction just used to count the tcp transmission times*/
static err_t
tcp_sent_callback(void *arg, struct tcp_pcb *tpcb, u16_t len)
{

	tcp_trans_done ++;
	return ERR_OK;
}


static err_t
tcp_connected_callback(void *arg, struct tcp_pcb *tpcb, err_t err)
{
	xil_printf("txperf: Connected to iperf server\r\n");

	/* store state */
	connected_pcb = tpcb;

	/* set callback values & functions */
	tcp_arg(tpcb, NULL);
	tcp_sent(tpcb, tcp_sent_callback);

	tcp_client_connected = 1;

	/* initiate data transfer */
	return ERR_OK;
}


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




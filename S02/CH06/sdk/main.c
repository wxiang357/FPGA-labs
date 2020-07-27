#include <stdio.h>
#include "xscugic.h"
#include "xil_exception.h"

#define INT_CFG0_OFFSET 0x00000C00

// Parameter definitions
#define SW1_INT_ID              61
#define SW2_INT_ID              62
#define INTC_DEVICE_ID          XPAR_PS7_SCUGIC_0_DEVICE_ID
#define INT_TYPE_RISING_EDGE    0x03
#define INT_TYPE_HIGHLEVEL      0x01
#define INT_TYPE_MASK           0x03

static XScuGic INTCInst;

static void SW_intr_Handler(void *param);
static int IntcInitFunction(u16 DeviceId);

static void SW_intr_Handler(void *param)
{
    int sw_id = (int)param;
    printf("SW%d int\n\r", sw_id);
}

void IntcTypeSetup(XScuGic *InstancePtr, int intId, int intType)
{
    int mask;

    intType &= INT_TYPE_MASK;
    mask = XScuGic_DistReadReg(InstancePtr, INT_CFG0_OFFSET + (intId/16)*4);
    mask &= ~(INT_TYPE_MASK << (intId%16)*2);
    mask |= intType << ((intId%16)*2);
    XScuGic_DistWriteReg(InstancePtr, INT_CFG0_OFFSET + (intId/16)*4, mask);
}

int IntcInitFunction(u16 DeviceId)
{
    XScuGic_Config *IntcConfig;
    int status;

    // Interrupt controller initialisation
    IntcConfig = XScuGic_LookupConfig(DeviceId);
    status = XScuGic_CfgInitialize(&INTCInst, IntcConfig, IntcConfig->CpuBaseAddress);
    if(status != XST_SUCCESS) return XST_FAILURE;

    // Call to interrupt setup
    Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,
                                 (Xil_ExceptionHandler)XScuGic_InterruptHandler,
                                 &INTCInst);
    Xil_ExceptionEnable();

    // Connect SW1~SW3 interrupt to handler
    status = XScuGic_Connect(&INTCInst,
                             SW1_INT_ID,
                             (Xil_ExceptionHandler)SW_intr_Handler,
                             (void *)1);
    if(status != XST_SUCCESS) return XST_FAILURE;

    status = XScuGic_Connect(&INTCInst,
                             SW2_INT_ID,
                             (Xil_ExceptionHandler)SW_intr_Handler,
                             (void *)2);
    if(status != XST_SUCCESS) return XST_FAILURE;


    // Set interrupt type of SW1~SW3 to rising edge
    //IntcTypeSetup(&INTCInst, SW1_INT_ID, INT_TYPE_RISING_EDGE); // changed by wx
    //IntcTypeSetup(&INTCInst, SW2_INT_ID, INT_TYPE_RISING_EDGE); // changed by wx
    XScuGic_SetPriorityTriggerType(&INTCInst, SW1_INT_ID, 0xa0, 0x03);
    XScuGic_SetPriorityTriggerType(&INTCInst, SW2_INT_ID, 0xa0, 0x03);

    // Enable SW1~SW2 interrupts in the controller
    XScuGic_Enable(&INTCInst, SW1_INT_ID);
    XScuGic_Enable(&INTCInst, SW2_INT_ID);

    return XST_SUCCESS;
}

int main(void)
{
    print("PL int test\n\r");
    IntcInitFunction(INTC_DEVICE_ID);
    while(1);
    return 0;
}



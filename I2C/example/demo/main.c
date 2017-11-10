/*-----------------------------------------------------------------------------------*/
/* Nuvoton Technology Corporation confidential                                      */
/*                                                                                   */
/* Copyright (c) 2008 by Nuvoton Technology Corporation                             */
/* All rights reserved                                                               */
/*                                                                                   */
/*-----------------------------------------------------------------------------------*/
 
#ifdef ECOS 
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "kapi.h"
#include "diag.h"
#include "wbio.h"
#else 
#include <stdio.h>
#include <string.h>
#include "wblib.h"
#endif

#include "w55fa93_i2c.h"
#include "W55FA93_VideoIn.h"

#define DBG_PRINTF		sysprintf
//#define DBG_PRINTF(...)


#define RETRY		10  /* Programming cycle may be in progress. Please refer to 24LC64 datasheet */


int main (VOID)
{
	WB_UART_T uart;	
	UINT32 u32ExtFreq;	
	UINT8  u8DeviceID;
	UINT8 u8ID;	
	INT32 rtval;
	INT j;
	
	sysUartPort(1);		
	uart.uiFreq =sysGetExternalClock()*1000;	//use APB clock
	uart.uiBaudrate = 115200;
	uart.uiDataBits = WB_DATA_BITS_8;
	uart.uiStopBits = WB_STOP_BITS_1;
	uart.uiParity = WB_PARITY_NONE;
	uart.uiRxTriggerLevel = LEVEL_1_BYTE;
	sysInitializeUART(&uart);

	DBG_PRINTF("\nI2C demo read OV7670 sensor ID...\n");

	sysSetSystemClock(eSYS_UPLL, 	//E_SYS_SRC_CLK eSrcClk,	
						192000,		//UINT32 u32PllKHz, 	
						192000,		//UINT32 u32SysKHz,
						192000,		//UINT32 u32CpuKHz,
						  96000,		//UINT32 u32HclkKHz,
						  48000);		//UINT32 u32ApbKHz	

	sysEnableCache(CACHE_WRITE_BACK);

	/* init timer */	
	u32ExtFreq = sysGetExternalClock();    	/* KHz unit */	
	sysSetTimerReferenceClock (TIMER0, 
								u32ExtFreq*1000);	/* Hz unit */
	sysStartTimer(TIMER0, 
					100, 
					PERIODIC_MODE);

	videoIn_Init(TRUE, 0, 24000, eVIDEOIN_SNR_CCIR601);

	videoIn_Open(48000, 24000);								/* For sensor clock output */

	i2cInit();

	rtval = i2cOpen();
	if(rtval < 0)
	{
		DBG_PRINTF("Open I2C error!\n");	
		goto exit_test;
	}

	u8DeviceID = 0x42;
	i2cIoctl(I2C_IOC_SET_DEV_ADDRESS, (u8DeviceID>>1), 0);  
	i2cIoctl(I2C_IOC_SET_SPEED, 100, 0);	

	i2cIoctl(I2C_IOC_SET_SUB_ADDRESS, 0x0A, 1);	
	j = RETRY;
	while(j-- > 0) 
	{
		if(i2cRead_OV(&u8ID, 1) == 1)
			break;
	}
	if(j <= 0)
		DBG_PRINTF("Read ERROR [%x]!\n", 0x0A);
	DBG_PRINTF("Sensor ID0 = 0x%x\n", u8ID);

	i2cIoctl(I2C_IOC_SET_SUB_ADDRESS, 0x0B, 1);	
	j = RETRY;
	while(j-- > 0) 
	{
		if(i2cRead_OV(&u8ID, 1) == 1)
			break;
	}
	if(j <= 0)
		DBG_PRINTF("Read ERROR [%x]!\n", 0x0B);
	DBG_PRINTF("Sensor ID0 = 0x%x\n", u8ID);

	i2cIoctl(I2C_IOC_SET_SUB_ADDRESS, 0x1C, 1);	
	j = RETRY;
	while(j-- > 0) 
	{
		if(i2cRead_OV(&u8ID, 1) == 1)
			break;
	}
	if(j < 0)
		DBG_PRINTF("Read ERROR [%x]!\n", 0x1C);		
	DBG_PRINTF("Sensor ID0 = 0x%x\n", u8ID);
	
	i2cIoctl(I2C_IOC_SET_SUB_ADDRESS, 0x1D, 1);	
	j = RETRY;
	while(j-- > 0) 
	{
		if(i2cRead_OV(&u8ID, 1) == 1)
			break;
	}
	if(j < 0)
		DBG_PRINTF("Read ERROR [%x]!\n", 0x1D);
	DBG_PRINTF("Sensor ID0 = 0x%x\n", u8ID);

	i2cIoctl(I2C_IOC_SET_SUB_ADDRESS, 0xD7, 1);	
	j = RETRY;
	while(j-- > 0) 
	{
		if(i2cRead_OV(&u8ID, 1) == 1)
			break;
	}
	if(j < 0)
		DBG_PRINTF("Read ERROR [%x]!\n", 0xD7);
	DBG_PRINTF("Sensor ID0 = 0x%x\n", u8ID);

	i2cIoctl(I2C_IOC_SET_SUB_ADDRESS, 0x6A, 1);	
	j = RETRY;
	while(j-- > 0) 
	{
		if(i2cRead_OV(&u8ID, 1) == 1)
			break;
	}
	if(j < 0)
		DBG_PRINTF("Read ERROR [%x]!\n", 0x6A);
	DBG_PRINTF("Sensor ID0 = 0x%x\n", u8ID);

exit_test:	
	return 0;
}

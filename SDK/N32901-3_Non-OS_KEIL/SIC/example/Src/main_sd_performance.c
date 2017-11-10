/****************************************************************************
 *
 **************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "wbio.h"
#include "wbtypes.h"
//#include "WBChipDef.h"
//#include "usbd.h"
#include "wbio.h"
#include "wblib.h"
#include "wbtypes.h"

#include "w55fa93_reg.h"
#include "w55fa93_sic.h"
#include "w55fa93_GNAND.h"
#include "nvtfat.h"

//#include "nvtfat.h"

__align (32) UINT8 g_ram0[512*16*16];
__align (32) UINT8 g_ram1[512*16*16];
UINT32 u32SecCnt;
UINT32 u32backup[10];

extern PDISK_T *pDisk_SD0;
extern PDISK_T *pDisk_SD1;
extern PDISK_T *pDisk_SD2;


int volatile gTotalSectors_SD = 0, gTotalSectors_SM = 0;


NDRV_T _nandDiskDriver0 = 
{
	nandInit0,
	nandpread0,
	nandpwrite0,
	nand_is_page_dirty0,
	nand_is_valid_block0,
	nand_ioctl,
	nand_block_erase0,
	nand_chip_erase0,
	0
};
NDISK_T *ptMassNDisk;

void initPLL(void)
{
}


int count = 0;
int STATUS;
//void main(void)
int main(void)
{
	int ii, jj;
	unsigned int tick_bw, tick_aw, tick_w;
	unsigned int tick_br, tick_ar, tick_r;	
	
//	InitUART();
//	g_SetupTOK =0;
//	g_SetupPKT=0;
//	g_OutTOK = 0;
//	g_SetupInTOK=0;
//	g_TXD=0;
//	g_RXD = 0;
	 
//	DrvSIO_printf("TEST %d\n",count);
	
	initPLL();
	
	sysSetTimerReferenceClock(TIMER0, 12000000); 			//External Crystal
	sysStartTimer(TIMER0, 100, PERIODIC_MODE);				/* 100 ticks/per sec ==> 1tick/10ms */
	sysSetLocalInterrupt(ENABLE_IRQ);	
	
	/* initialize FMI (Flash memory interface controller) */
	
#ifndef OPT_FPGA_DEBUG
//    sicIoctl(SIC_SET_CLOCK, 372000, 0, 0);  	/* clock from PLL */
    sicIoctl(SIC_SET_CLOCK, 192000, 0, 0);  	/* clock from PLL */    
#else    
    sicIoctl(SIC_SET_CLOCK, 27000, 0, 0);  		/* clock from FPGA clock in */
#endif    
    sicOpen();
    
//#define SD_0_TEST
//#define SD_1_TEST
#define SD_2_TEST

#ifdef SD_0_TEST
	if (sicSdOpen0()<=0)
#endif
#ifdef SD_1_TEST
	if (sicSdOpen1()<=0)
#endif
#ifdef SD_2_TEST
	if (sicSdOpen2()<=0)
#endif
	{
		printf("Error in initializing SD card !! \n");						
		while(1);
	}			
	else
	{
#ifdef SD_0_TEST	
		printf("SD total sector is %4x !! --\n", pDisk_SD0 -> uTotalSectorN);	
#endif		
#ifdef SD_1_TEST	
		printf("SD total sector is %4x !! --\n", pDisk_SD1 -> uTotalSectorN);	
#endif		
#ifdef SD_2_TEST	
		printf("SD total sector is %4x !! --\n", pDisk_SD2 -> uTotalSectorN);	
#endif		
	}

	for(ii=0; ii<512*16*16; ii++)
		g_ram0[ii] = rand();
//		g_ram0[ii] = ii*(ii/512 + ii);

//#define SD_PERFORMANCE 
#ifdef SD_PERFORMANCE 

#define ACCESS_SIZE		10 	// MB

	u32SecCnt = 4;
	while(1)
	{
		tick_bw = sysGetTicks(TIMER0);	
		for (jj = 1000; jj < ACCESS_SIZE*1000*2+1000; jj+=u32SecCnt)	// write 100MB test
		{
			sicSdWrite0(jj, u32SecCnt, (UINT32)g_ram0);					
		}			
		tick_aw = sysGetTicks(TIMER0);	

		
		tick_br = sysGetTicks(TIMER0);					
		for (jj = 1000; jj < ACCESS_SIZE*1000*2+1000; jj+=u32SecCnt)	// read 100MB test
		{
			sicSdRead0(jj, u32SecCnt, (UINT32)g_ram1);							
		}			
		tick_ar = sysGetTicks(TIMER0);							
		tick_r = tick_ar - tick_br;		

		tick_w = tick_aw - tick_bw;		
		
		printf("========================================\n");
		printf("*** write SD test *** \n");		
		printf("write 10MB test per %d sectors !!!\n", u32SecCnt);									
		printf("write speed --- %d KB\n", (ACCESS_SIZE*1000*100/tick_w));									

		printf("*** read SD test *** \n");				
		printf("read 10MB test per %d sectors !!!\n", u32SecCnt);									
		printf("read speed --- %d KB\n", (ACCESS_SIZE*1000*100/tick_r));									
		printf("========================================\n");		

		u32SecCnt+= 4;		
	}
	
		
#else
	while(1)
	{
		u32backup[0] = g_ram0[0];
#ifdef SD_0_TEST			
		for (jj = 1000; jj < pDisk_SD0->uTotalSectorN -0x100; jj+=u32SecCnt)
#endif		
#ifdef SD_1_TEST			
		for (jj = 1000; jj < pDisk_SD1->uTotalSectorN -0x100; jj+=u32SecCnt)
#endif		
#ifdef SD_2_TEST			
		for (jj = 1000; jj < pDisk_SD2->uTotalSectorN -0x100; jj+=u32SecCnt)
#endif		
		{
			u32SecCnt = rand()&(0xFF);
			if (u32SecCnt==0)
				u32SecCnt = 1;
	//		u32SecCnt = 0x33;		
	
#ifdef SD_0_TEST	
			sicSdWrite0(jj, u32SecCnt, (UINT32)g_ram0);					
			
		    memset(g_ram1, 0x11, u32SecCnt);		
			sicSdRead0(jj, u32SecCnt, (UINT32)g_ram1);							
#endif			

#ifdef SD_1_TEST	
			sicSdWrite1(jj, u32SecCnt, (UINT32)g_ram0);					
			
		    memset(g_ram1, 0x11, u32SecCnt);		
			sicSdRead1(jj, u32SecCnt, (UINT32)g_ram1);							
#endif			

#ifdef SD_2_TEST	
			sicSdWrite2(jj, u32SecCnt, (UINT32)g_ram0);					
			
		    memset(g_ram1, 0x11, u32SecCnt);		
			sicSdRead2(jj, u32SecCnt, (UINT32)g_ram1);							
#endif			

	        if(memcmp(g_ram0, g_ram1, u32SecCnt*512) != 0)
	        {
				printf("data compare ERROR at sector No. !! -- %4x \n", jj);
	        	while(1);
			}			        	
			printf("data compare OK at sector No. !! -- %4x \n", jj);					
		}			
	}
#endif	
}



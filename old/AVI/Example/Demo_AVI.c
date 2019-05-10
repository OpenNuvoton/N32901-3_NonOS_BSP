#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#pragma import(__use_no_semihosting_swi)

#include "wbio.h"
#include "wblib.h"

#include "w55fa93_vpost.h"
#include "w55fa93_sic.h"
#include "nvtfat.h"
#include "AviLib.h"
#include "spu.h"


UINT32 StorageForNAND(void);

//#pragma import(__use_no_semihosting_swi)
#define VPOST_FRAME_BUFSZ		(320*240*2) //(640*480*2)


static __align(256) UINT8  _VpostFrameBufferPool[VPOST_FRAME_BUFSZ];
static UINT8   *_VpostFrameBuffer;

#define STORAGE_SD 	1   // comment to work NAND, uncomment to use SD
//#define NAND_2		1   // comment to use 1 disk foor NAND, uncomment to use 2 disk

#ifndef STORAGE_SD

static NDISK_T ptNDisk;
static NDRV_T _nandDiskDriver0 =
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

#define NAND1_1_SIZE	 32	/* MB unit */

void AudioChanelControl(void)
{

}

UINT32 StorageForNAND(void)
{

	UINT32 block_size, free_size, disk_size; 
	UINT32 u32TotalSize;

	
    fsAssignDriveNumber('C', DISK_TYPE_SMART_MEDIA, 0, 1);
#ifdef NAND_2    
    fsAssignDriveNumber('D', DISK_TYPE_SMART_MEDIA, 0, 2);                      
#endif    
	     	
	
   	/* For detect VBUS stable */ 
   	sicOpen();
    sicIoctl(SIC_SET_CLOCK, 192000, 0, 0);  /* clock from PLL */   	
	

	/* Initialize GNAND */
	if(GNAND_InitNAND(&_nandDiskDriver0, &ptNDisk, TRUE) < 0) 
	{
		sysprintf("GNAND_InitNAND error\n");
		goto halt;
	}	
	
	if(GNAND_MountNandDisk(&ptNDisk) < 0) 
	{
		sysprintf("GNAND_MountNandDisk error\n");
		goto halt;		
	}
	
	/* Get NAND disk information*/ 
	u32TotalSize = ptNDisk.nZone* ptNDisk.nLBPerZone*ptNDisk.nPagePerBlock*ptNDisk.nPageSize;
	sysprintf("Total Disk Size %d\n", u32TotalSize);	
	/* Format NAND if necessery */
#ifdef NAND_2	
	if ((fsDiskFreeSpace('C', &block_size, &free_size, &disk_size) < 0) || 
	    (fsDiskFreeSpace('D', &block_size, &free_size, &disk_size) < 0)) 			    
	    	{   
	    		sysprintf("unknow disk type, format device .....\n");	
		    	if (fsTwoPartAndFormatAll((PDISK_T *)ptNDisk.pDisk, NAND1_1_SIZE*1024, (u32TotalSize- NAND1_1_SIZE*1024)) < 0) {
					sysprintf("Format failed\n");					
				goto halt;
	    	}
	    	fsSetVolumeLabel('C', "NAND1-1\n", strlen("NAND1-1"));
			fsSetVolumeLabel('D', "NAND1-2\n", strlen("NAND1-2"));	
	}
#endif	
	
	AudioChanelControl();
	
	/* Initial SPU in advance for linux set volume issue */ 	
	spuOpen(eDRVSPU_FREQ_8000);
	spuDacOn(1);
//	spuIoctl(SPU_IOCTL_SET_VOLUME, 0x20, 0x20);	
halt:	
	sysprintf("systen exit\n");
    return 0; 	
     
}

#endif

void  avi_play_control(AVI_INFO_T *aviInfo)
{
	static INT	last_time;
	int    frame_rate;
	
	if (aviInfo->uPlayCurTimePos != 0)
		frame_rate = ((aviInfo->uVidFramesPlayed - aviInfo->uVidFramesSkipped) * 100) / aviInfo->uPlayCurTimePos;
	
	if (aviInfo->uPlayCurTimePos - last_time > 100)
	{
		sysprintf("%02d:%02d / %02d:%02d  Vid fps: %d / %d\n", 
			aviInfo->uPlayCurTimePos / 6000, (aviInfo->uPlayCurTimePos / 100) % 60,
			aviInfo->uMovieLength / 6000, (aviInfo->uMovieLength / 100) % 60,
			frame_rate, aviInfo->uVideoFrameRate);
		last_time = aviInfo->uPlayCurTimePos;
	}
}


int main()
{
    WB_UART_T 	uart;
	LCDFORMATEX lcdformatex;
	CHAR		suFileName[128];
	INT			nStatus;

	
	/* CACHE_ON	*/
	sysEnableCache(CACHE_WRITE_BACK);

	/*-----------------------------------------------------------------------*/
	/*  CPU/AHB/APH:  200/100/50                                             */
	/*-----------------------------------------------------------------------*/
	
    sysSetSystemClock(eSYS_UPLL, 	//E_SYS_SRC_CLK eSrcClk,	
						192000,		//UINT32 u32PllKHz, 	
						192000,		//UINT32 u32SysKHz,
						192000,		//UINT32 u32CpuKHz,
						192000/2,		//UINT32 u32HclkKHz,
						192000/4);		//UINT32 u32ApbKHz	

	/*-----------------------------------------------------------------------*/
	/*  Init UART, N,8,1, 115200                                             */
	/*-----------------------------------------------------------------------*/
		sysUartPort(1);	
		
	uart.uiFreq = 12000000;					//use XIN clock
    uart.uiBaudrate = 115200;
    uart.uiDataBits = WB_DATA_BITS_8;
    uart.uiStopBits = WB_STOP_BITS_1;
    uart.uiParity = WB_PARITY_NONE;
	uart.uiRxTriggerLevel = LEVEL_1_BYTE;
    sysInitializeUART(&uart);
	sysprintf("UART initialized.\n");

	_VpostFrameBuffer = (UINT8 *)((UINT32)_VpostFrameBufferPool | 0x80000000);

	/*-----------------------------------------------------------------------*/
	/*  Init timer                                                           */
	/*-----------------------------------------------------------------------*/
	sysSetTimerReferenceClock (TIMER0, 60000000);
	sysStartTimer(TIMER0, 100, PERIODIC_MODE);

	/*-----------------------------------------------------------------------*/
	/*  Init FAT file system                                                 */
	/*-----------------------------------------------------------------------*/
	sysprintf("fsInitFileSystem.\n");
	fsInitFileSystem();
#ifdef STORAGE_SD	

	/*-----------------------------------------------------------------------*/
	/*  Init SD card                                                         */
	/*-----------------------------------------------------------------------*/
	sicIoctl(SIC_SET_CLOCK, 200000, 0, 0);
	sicOpen();
	sysprintf("total sectors (%x)\n", sicSdOpen0());
	
	spuOpen(eDRVSPU_FREQ_8000);
	spuDacOn(1);
#else
    StorageForNAND();
#endif	

#if 0
	/*-----------------------------------------------------------------------*/
	/*                                                                       */
	/*  Direct RGB555 AVI playback 	               							 */
	/*                                                                       */
	/*-----------------------------------------------------------------------*/
	lcdformatex.ucVASrcFormat = DRVVPOST_FRAME_RGB555;
    vpostLCMInit(&lcdformatex, (UINT32 *)_VpostFrameBuffer);

	fsAsciiToUnicode("c:\\sample.avi", suFileName, TRUE);	

   	if (aviPlayFile(suFileName, 0, 0, DIRECT_RGB555, avi_play_control) < 0)
		sysprintf("Playback failed, code = %x\n", nStatus);
	else
		sysprintf("Playback done.\n");

#endif
#if 1
	/*-----------------------------------------------------------------------*/
	/*                                                                       */
	/*  Direct RGB565 AVI playback 	                                         */
	/*                                                                       */
	/*-----------------------------------------------------------------------*/
	lcdformatex.ucVASrcFormat = DRVVPOST_FRAME_RGB565;
    vpostLCMInit(&lcdformatex, (UINT32 *)_VpostFrameBuffer);

	fsAsciiToUnicode("c:\\sample.avi", suFileName, TRUE);	

   	if (aviPlayFile(suFileName, 0, 0, DIRECT_RGB565, avi_play_control) < 0)
		sysprintf("Playback failed, code = %x\n", nStatus);
	else
		sysprintf("Playback done.\n");
#endif
#if 0
	/*-----------------------------------------------------------------------*/
	/*                                                                       */
	/*  Direct YUV422 AVI playback 	                                         */
	/*                                                                       */
	/*-----------------------------------------------------------------------*/
	lcdformatex.ucVASrcFormat = DRVVPOST_FRAME_YCBYCR;
    vpostLCMInit(&lcdformatex, (UINT32 *)_VpostFrameBuffer);

	fsAsciiToUnicode("c:\\sample.avi", suFileName, TRUE);	

   	if (aviPlayFile(suFileName, 0, 0, DIRECT_YUV422, avi_play_control) < 0)
		sysprintf("Playback failed, code = %x\n", nStatus);
	else
		sysprintf("Playback done.\n");
#endif	

#ifndef STORAGE_SD
	GNAND_UnMountNandDisk(&ptNDisk);
   	fmiSMClose(0);		
	sicClose();
#endif
	
	while(1);		
}




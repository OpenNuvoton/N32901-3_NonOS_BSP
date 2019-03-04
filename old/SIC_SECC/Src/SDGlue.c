/*-----------------------------------------------------------------------------------*/
/* Nuvoton Technology Corporation confidential                                       */
/*                                                                                   */
/* Copyright (c) 2008 by Nuvoton Technology Corporation                              */
/* All rights reserved                                                               */
/*                                                                                   */
/*-----------------------------------------------------------------------------------*/
/* 
 * Driver for FMI devices
 * SD layer glue code
 *
 */

#ifdef ECOS
	#include "stdlib.h"
	#include "string.h"
	#include "drv_api.h"
	#include "diag.h"
	#include "wbtypes.h"
	#include "wbio.h"
#else
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include "wblib.h"
#endif

#include "w55fa93_reg.h"
#include "w55fa93_sic.h"

#include "fmi.h"
#include "nvtfat.h"

//#define CD_GPE6

#define NO_SD_CARD_DETECT


DISK_DATA_T SD_DiskInfo0;
DISK_DATA_T SD_DiskInfo1;
DISK_DATA_T SD_DiskInfo2;


FMI_SD_INFO_T *pSD0 = NULL;
FMI_SD_INFO_T *pSD1 = NULL;
FMI_SD_INFO_T *pSD2 = NULL;

UINT8 pSD0_offset = 0;
UINT8 pSD1_offset = 0;
UINT8 pSD2_offset = 0;

PDISK_T *pDisk_SD0 = NULL;
PDISK_T *pDisk_SD1 = NULL;
PDISK_T *pDisk_SD2 = NULL;

extern INT  fsPhysicalDiskConnected(PDISK_T *pDisk);


static INT  sd_disk_init(PDISK_T  *lDisk)
{
	return 0;
}


static INT  sd_disk_ioctl(PDISK_T *lDisk, INT control, VOID *param)
{
	return 0;
}

static INT  sd_disk_read(PDISK_T *pDisk, UINT32 sector_no, INT number_of_sector, UINT8 *buff)
{
	int status;	

	fmiSD_CardSel(0);
	
	// enable SD
	outpw(REG_FMICR, FMI_SD_EN);
	status = fmiSD_Read_in(pSD0, sector_no, number_of_sector, (unsigned int)buff);

	if (status != Successful)
		return status;

	return FS_OK;
}

static INT  sd_disk_write(PDISK_T *pDisk, UINT32 sector_no, INT number_of_sector, UINT8 *buff, BOOL IsWait)
{
	int status;

	fmiSD_CardSel(0);
	
	// enable SD
	outpw(REG_FMICR, FMI_SD_EN);
	status = fmiSD_Write_in(pSD0, sector_no, number_of_sector, (unsigned int)buff);

	if (status != Successful)
		return status;

	return FS_OK;
}

static INT  sd_disk_init0(PDISK_T  *lDisk)
{
	return 0;
}

static INT  sd_disk_ioctl0(PDISK_T *lDisk, INT control, VOID *param)
{
	return 0;
}

static INT  sd_disk_read0(PDISK_T *pDisk, UINT32 sector_no, INT number_of_sector, UINT8 *buff)
{
	return sd_disk_read(pDisk, sector_no, number_of_sector, buff);
}

static INT  sd_disk_write0(PDISK_T *pDisk, UINT32 sector_no, INT number_of_sector, UINT8 *buff, BOOL IsWait)
{
	return sd_disk_write(pDisk, sector_no, number_of_sector, buff, IsWait);	
}

static INT  sd_disk_init1(PDISK_T  *lDisk)
{
	return 0;
}

static INT  sd_disk_ioctl1(PDISK_T *lDisk, INT control, VOID *param)
{
	return 0;
}

static INT  sd_disk_read1(PDISK_T *pDisk, UINT32 sector_no, INT number_of_sector, UINT8 *buff)
{
	int status;	

	fmiSD_CardSel(1);
	
	// enable SD
	outpw(REG_FMICR, FMI_SD_EN);
	status = fmiSD_Read_in(pSD1, sector_no, number_of_sector, (unsigned int)buff);

	if (status != Successful)
		return status;

	return FS_OK;
}

static INT  sd_disk_write1(PDISK_T *pDisk, UINT32 sector_no, INT number_of_sector, UINT8 *buff, BOOL IsWait)
{
	int status;

	fmiSD_CardSel(1);
	
	// enable SD
	outpw(REG_FMICR, FMI_SD_EN);
	status = fmiSD_Write_in(pSD1, sector_no, number_of_sector, (unsigned int)buff);

	if (status != Successful)
		return status;

	return FS_OK;
}

static INT  sd_disk_init2(PDISK_T  *lDisk)
{
	return 0;
}

static INT  sd_disk_ioctl2(PDISK_T *lDisk, INT control, VOID *param)
{
	return 0;
}

static INT  sd_disk_read2(PDISK_T *pDisk, UINT32 sector_no, INT number_of_sector, UINT8 *buff)
{
	int status;	

	fmiSD_CardSel(2);
	
	// enable SD
	outpw(REG_FMICR, FMI_SD_EN);
	status = fmiSD_Read_in(pSD2, sector_no, number_of_sector, (unsigned int)buff);

	if (status != Successful)
		return status;

	return FS_OK;
}

static INT  sd_disk_write2(PDISK_T *pDisk, UINT32 sector_no, INT number_of_sector, UINT8 *buff, BOOL IsWait)
{
	int status;

	fmiSD_CardSel(2);
	
	// enable SD
	outpw(REG_FMICR, FMI_SD_EN);
	status = fmiSD_Write_in(pSD2, sector_no, number_of_sector, (unsigned int)buff);

	if (status != Successful)
		return status;

	return FS_OK;
}

STORAGE_DRIVER_T  _SDDiskDriver = 
{
	sd_disk_init,
	sd_disk_read,
	sd_disk_write,
	sd_disk_ioctl
};

STORAGE_DRIVER_T  _SD0DiskDriver = 
{
	sd_disk_init0,
	sd_disk_read0,
	sd_disk_write0,
	sd_disk_ioctl0
};

STORAGE_DRIVER_T  _SD1DiskDriver = 
{
	sd_disk_init1,
	sd_disk_read1,
	sd_disk_write1,
	sd_disk_ioctl1
};

STORAGE_DRIVER_T  _SD2DiskDriver = 
{
	sd_disk_init2,
	sd_disk_read2,
	sd_disk_write2,
	sd_disk_ioctl2
};

static int sd0_ok = 0;
static int sd1_ok = 0;
static int sd2_ok = 0;

INT  fmiSD_CardSel(INT cardSel)
{
	int status;
	if (cardSel==0)
	{
		outpw(REG_GPEFUN, inpw(REG_GPEFUN)&(~0x0000FFF0) | 0x0000aaa0);	// SD0_CLK/CMD/DAT0_3 pins selected
//		outpw(REG_SDCR, inpw(REG_SDCR) & (~SDCR_SDPORT));					// SD_0 port selected 		
		status = inpw(REG_SDCR) & (~SDCR_SDPORT);
		outpw(REG_SDCR, status);
	}
	else if (cardSel==1)
	{
		outpw(REG_GPBFUN, inpw(REG_GPDFUN)&(~0x00000FFF) | 0x00000AAA); 
		outpw(REG_SDCR, inpw(REG_SDCR) & (~0x60000000) | 0x20000000);	   // SD_1 port selected 
	}
	else if (cardSel==2)
	{
		outpw(REG_GPEFUN, inpw(REG_GPEFUN)&(~0x000F0000) | 0x00050000);  // SD2_CLK/CMD pins selected
		outpw(REG_GPDFUN, inpw(REG_GPDFUN)&(~0x0003FC00) | 0x00015400);  // SD2_DAT0_3 pins selected	
		outpw(REG_SDCR, inpw(REG_SDCR) & (~0x60000000) | 0x40000000);	   // SD_2 port selected 
	}
	else
		return 1;		// wrong SD card select	
		
	return 0;		
}

INT  fmiInitSDDevice(INT cardSel)   /* int sd_init_onedisk(INT i) */
{
	PDISK_T *pDisk;
	DISK_DATA_T* pSDDisk;	
	FMI_SD_INFO_T *pSD_temp = NULL;	
	
	int volatile rate, i;

	//Reset FMI
	outpw(REG_FMICR, FMI_SWRST);		// Start reset FMI controller.
	while(inpw(REG_FMICR)&FMI_SWRST);	
	
	// enable SD
	outpw(REG_FMICR, FMI_SD_EN);
    outpw(REG_SDCR, inpw(REG_SDCR) | SDCR_SWRST); 	// SD software reset 
    while(inpw(REG_SDCR) & SDCR_SWRST);
    
    outpw(REG_SDCR, inpw(REG_SDCR) & ~0xFF);        // disable SD clock ouput
	
	if (cardSel == 0)
	{
		if(sd0_ok == 1)
			return(0);
		pSDDisk	= &SD_DiskInfo0;		
	}		
	else if (cardSel == 1)
	{
		if(sd1_ok == 1)
			return(0);
		pSDDisk	= &SD_DiskInfo1;
	}		
	else if (cardSel == 2)
	{
		if(sd2_ok == 1)
			return(0);
		pSDDisk	= &SD_DiskInfo2;
	}		

	// enable SD-0/1/2 pin?
	if (fmiSD_CardSel(cardSel))
		return FMI_NO_SD_CARD;				

	//Enable SD-0 card detectipn pin 
	if (cardSel==0)
	{
		outpw(REG_GPAFUN, inpw(REG_GPAFUN) | MF_GPA1);	// GPA1 for SD-0 card detection
		outpw(REG_SDIER, inpw(REG_SDIER) | SDIER_CDSRC);	// SD card detection source from GPIO but not DAT3
	}

	// Disable FMI/SD host interrupt
	outpw(REG_FMIIER, 0);

//	outpw(REG_SDCR, (inpw(REG_SDCR) & ~SDCR_SDNWR) | (0x01 << 24));		// set SDNWR = 1
	outpw(REG_SDCR, (inpw(REG_SDCR) & ~SDCR_SDNWR) | (0x09 << 24));		// set SDNWR = 9
	outpw(REG_SDCR, (inpw(REG_SDCR) & ~SDCR_BLKCNT) | (0x01 << 16));	// set BLKCNT = 1
	outpw(REG_SDCR, inpw(REG_SDCR) & ~SDCR_DBW);		// SD 1-bit data bus	

	pSD_temp = malloc(sizeof(FMI_SD_INFO_T)+4);
	if (pSD_temp == NULL)
		return FMI_NO_MEMORY;

	memset((char *)pSD_temp, 0, sizeof(FMI_SD_INFO_T)+4);

	if (cardSel==0)
	{
		pSD0_offset = (UINT32)pSD_temp %4;
		pSD0 = (FMI_SD_INFO_T *)((UINT32)pSD_temp + pSD0_offset);
	}
	else if	(cardSel==1)
	{
		pSD1_offset = (UINT32)pSD_temp %4;
		pSD1 = (FMI_SD_INFO_T *)((UINT32)pSD_temp + pSD1_offset);
	}
	else if	(cardSel==2)
	{
		pSD2_offset = (UINT32)pSD_temp %4;
		pSD2 = (FMI_SD_INFO_T *)((UINT32)pSD_temp + pSD2_offset);
	}

	outpw(REG_SDIER, inpw(REG_SDIER) | SDIER_CDSRC); 	// select GPIO detect
	outpw(REG_SDIER, inpw(REG_SDIER) | SDIER_CD_IEN);	// enable card detect interrupt

	if (cardSel==0)
	{
#ifdef NO_SD_CARD_DETECT
		pSD0->bIsCardInsert = TRUE;
#else
		if (inpw(REG_SDISR) & SDISR_CD_Card)	// CD pin status
		{
			pSD0->bIsCardInsert = FALSE;
			if (pSD0 != NULL)
			{
				free((FMI_SD_INFO_T *)((UINT32)pSD0 - pSD0_offset));
				pSD0 = 0;
			}
			free((FMI_SD_INFO_T *)((UINT32)pSD0 - pSD0_offset));
			return FMI_NO_SD_CARD;
		}
		else
		{
			pSD0->bIsCardInsert = TRUE;
		}
#endif /*NO_SD_CARD_DETECT*/

		if (fmiSD_Init(pSD0) < 0)
			return FMI_SD_INIT_ERROR;

		/* divider */
		if (pSD0->uCardType == 3)
			rate = _fmi_uFMIReferenceClock / 20000;
		else
			rate = _fmi_uFMIReferenceClock / SD_FREQ;

		if (pSD0->uCardType == 3)
		{
	//		if ((_fmi_uFMIReferenceClock % 20000) == 0)
	//			rate = rate - 1;
			if ((_fmi_uFMIReferenceClock % 20000) > 0)			
				rate ++;
				
		}
		else
		{
	//		if ((_fmi_uFMIReferenceClock % SD_FREQ) == 0)
	//			rate = rate - 1;
			if ((_fmi_uFMIReferenceClock % SD_FREQ) > 0)			
				rate ++;
		}
	}
	else if (cardSel==1)					
	{
		// SD-1 no card detect
		pSD1->bIsCardInsert = TRUE;		
		
		// SD-1 initial
		if (fmiSD_Init(pSD1) < 0)
			return FMI_SD_INIT_ERROR;

		/* divider */
		if (pSD1->uCardType == 3)
			rate = _fmi_uFMIReferenceClock / 20000;
		else
			rate = _fmi_uFMIReferenceClock / 20000;

		if (pSD1->uCardType == 3)
		{
	//		if ((_fmi_uFMIReferenceClock % 20000) == 0)
	//			rate = rate - 1;
			if ((_fmi_uFMIReferenceClock % 20000) > 0)			
				rate ++;
		}
		else
		{
	//		if ((_fmi_uFMIReferenceClock % SD_FREQ) == 0)
	//			rate = rate - 1;
			if ((_fmi_uFMIReferenceClock % SD_FREQ) > 0)			
				rate ++;
		}
	}
	else if (cardSel==2)					
	{
		// SD-2 no card detect
		pSD2->bIsCardInsert = TRUE;				
		
		// SD-2 initial
		if (fmiSD_Init(pSD2) < 0)
			return FMI_SD_INIT_ERROR;

		/* divider */
		if (pSD2->uCardType == 3)
			rate = _fmi_uFMIReferenceClock / 20000;
		else
			rate = _fmi_uFMIReferenceClock / SD_FREQ;

		if (pSD2->uCardType == 3)
		{
		//	if ((_fmi_uFMIReferenceClock % 20000) == 0)
		//		rate = rate - 1;		
			if ((_fmi_uFMIReferenceClock % 20000) > 0)			
				rate ++;
		}
		else
		{
		//	if ((_fmi_uFMIReferenceClock % SD_FREQ) == 0)
		//		rate = rate - 1;
			if ((_fmi_uFMIReferenceClock % SD_FREQ) > 0)
				rate ++; 
		}
	}

	for(i=0; i<100; i++);
//	outpw(REG_CLKDIV2, (inpw(REG_CLKDIV2) & ~SD_S) | (0x02 << 19)); 	// SD clock from APLL

	outpw(REG_CLKDIV2, (inpw(REG_CLKDIV2) & ~SD_S) | (0x03 << 19)); 	// SD clock from UPLL
	
	outpw(REG_CLKDIV2, (inpw(REG_CLKDIV2) & ~SD_N0) | (0x01 << 16)); 	// SD clock divided by 2
	if (rate % 2)
	{
		rate /= 2;								
		rate &= 0xFF;
	}					
	else
	{
		rate /= 2;				
		rate &= 0xFF;					
		rate--;					
	}
	outpw(REG_CLKDIV2, (inpw(REG_CLKDIV2) & ~SD_N1) | (rate << 24)); 	// SD clock divider		

	for(i=0; i<1000; i++);

	/* init SD interface */
	if (cardSel==0)
	{			
		fmiGet_SD_info(pSD0, pSDDisk);
		if (fmiSelectCard(pSD0))
			return FMI_SD_SELECT_ERROR;
	}			
	else if (cardSel==1)
	{		
		fmiGet_SD_info(pSD1, pSDDisk);
		if (fmiSelectCard(pSD1))
			return FMI_SD_SELECT_ERROR;
	}			
	else if (cardSel==2)
	{		
		fmiGet_SD_info(pSD2, pSDDisk);
		if (fmiSelectCard(pSD2))
			return FMI_SD_SELECT_ERROR;
	}			

	/* 
	 * Create physical disk descriptor 
	 */
	pDisk = malloc(sizeof(PDISK_T));
	if (pDisk == NULL)
		return FMI_NO_MEMORY;
	memset((char *)pDisk, 0, sizeof(PDISK_T));

	/* read Disk information */
	pDisk->szManufacture[0] = '\0';
//	strcpy(pDisk->szProduct, (char *)SD_DiskInfo.product);
//	strcpy(pDisk->szSerialNo, (char *)SD_DiskInfo.serial);
	strcpy(pDisk->szProduct, (char *)pSDDisk->product);
	strcpy(pDisk->szSerialNo, (char *)pSDDisk->serial);


	pDisk->nDiskType = DISK_TYPE_SD_MMC;

	pDisk->nPartitionN = 0;
	pDisk->ptPartList = NULL;
	
	pDisk->nSectorSize = 512;
//	pDisk->uTotalSectorN = SD_DiskInfo.totalSectorN;
//	pDisk->uDiskSize = SD_DiskInfo.diskSize;
	pDisk->uTotalSectorN = pSDDisk->totalSectorN;
	pDisk->uDiskSize = pSDDisk->diskSize;

	/* create relationship between UMAS device and file system hard disk device */

//	pDisk->ptDriver = &_SDDiskDriver;
	if (cardSel==0)
		pDisk->ptDriver = &_SD0DiskDriver;
	else if (cardSel==1)
		pDisk->ptDriver = &_SD1DiskDriver;
	else if (cardSel==2)
		pDisk->ptDriver = &_SD2DiskDriver;

#ifdef DEBUG
	printf("SD disk found: size=%d MB\n", (int)pDisk->uDiskSize / 1024);
#endif

	if (cardSel==0)
	{
		pDisk_SD0 = pDisk;
	}			
	else if (cardSel==1)
	{
		pDisk_SD1 = pDisk;
	}			
	else if (cardSel==2)
	{
		pDisk_SD2 = pDisk;
	}			
	

	fsPhysicalDiskConnected(pDisk);

	if (cardSel == 0)
		sd0_ok = 1;	
	else if (cardSel == 1)
		sd1_ok = 1;	
	else if (cardSel == 2)
		sd2_ok = 1;	
	
	return pDisk->uTotalSectorN;
} 

INT  fmiSD_Read(UINT32 uSector, UINT32 uBufcnt, UINT32 uDAddr)
{
	int status=0;

	// enable SD
	outpw(REG_FMICR, FMI_SD_EN);
	status = fmiSD_Read_in(pSD0, uSector, uBufcnt, uDAddr);

	return status;
}

INT  fmiSD_Write(UINT32 uSector, UINT32 uBufcnt, UINT32 uSAddr)
{
	int status=0;

	// enable SD
	outpw(REG_FMICR, FMI_SD_EN);
	status = fmiSD_Write_in(pSD0, uSector, uBufcnt, uSAddr);

	return status;
}

#ifdef OPT_FA93
	VOID sicSdClose_sel(INT cardSel)
	{
		if (cardSel==0)
		{
			sd0_ok = 0;
			if (pSD0 != NULL)
			{
				free((FMI_SD_INFO_T *)((UINT32)pSD0 - pSD0_offset));
				pSD0 = 0;
			}
			if (pDisk_SD0 != NULL)
			{
				fsUnmountPhysicalDisk(pDisk_SD0);
				free(pDisk_SD0);
				pDisk_SD0 = NULL;
			}
		}
		else if (cardSel==1)
		{
			sd1_ok = 0;
			if (pSD1 != NULL)
			{
				free((FMI_SD_INFO_T *)((UINT32)pSD1 - pSD1_offset));
				pSD1 = 0;
			}
			if (pDisk_SD1 != NULL)
			{
				fsUnmountPhysicalDisk(pDisk_SD1);
				free(pDisk_SD1);
				pDisk_SD1 = NULL;
			}
		}
		else if (cardSel==2)
		{
			sd2_ok = 0;
			if (pSD2 != NULL)
			{
				free((FMI_SD_INFO_T *)((UINT32)pSD2 - pSD2_offset));
				pSD2 = 0;
			}
			if (pDisk_SD2 != NULL)
			{
				fsUnmountPhysicalDisk(pDisk_SD2);
				free(pDisk_SD2);
				pDisk_SD2 = NULL;
			}
		}
	}

	VOID sicSdClose(void)
	{
		sicSdClose_sel(0);
	}
	
	VOID sicSdClose0(void)
	{
		sicSdClose_sel(0);
	}

	VOID sicSdClose1(void)
	{
		sicSdClose_sel(1);	
	}

	VOID sicSdClose2(void)
	{
		sicSdClose_sel(2);	
	}
	
#else
	VOID sicSdClose(void)
	{
		sd_ok = 0;
		if (pSD0 != NULL)
		{
			free((FMI_SD_INFO_T *)((UINT32)pSD0 - pSD0_offset));
			pSD0 = 0;
		}
		if (pDisk_SD0 != NULL)
		{
			fsUnmountPhysicalDisk(pDisk_SD0);
			free(pDisk_SD0);
			pDisk_SD0 = NULL;
		}
	}
#endif

/*-----------------------------------------------------------------------------------*/
/* Function:                                                                         */
/*   sicSdOpen                                                                       */
/*                                                                                   */
/* Parameters:                                                                       */
/*   sdPortNo				SD port number(port0 or port1).							 */
/*                                                                                   */                                                                           
/* Returns:                                                                          */
/*   >0						Total sector.											 */
/*   FMI_NO_SD_CARD			No SD card insert.										 */
/*   FMI_SD_INIT_ERROR		Card initial and identify error.						 */
/*   FMI_SD_SELECT_ERROR	Select card from identify mode to stand-by mode error.	 */
/*                                                                                   */                                                                           
/* Side effects:                                                                     */
/*   None.                                                                           */
/*                                                                                   */
/* Description:                                                                      */
/*   This function initial SD from identification to stand-by mode.                  */
/*                                                                                   */
/*-----------------------------------------------------------------------------------*/

#if 1
	INT sicSdOpen_sel(INT cardSel)
	{
		return fmiInitSDDevice(cardSel);
	}

	INT sicSdOpen(void)
	{
		return fmiInitSDDevice(0);	
	}
	
	INT sicSdOpen0(void)
	{
		return fmiInitSDDevice(0);	
	}
	
	INT sicSdOpen1(void)
	{
		return fmiInitSDDevice(1);	
	}
	
	INT sicSdOpen2(void)
	{
		return fmiInitSDDevice(2);	
	}
	
#else
	INT sicSdOpen(void)
	{
		return fmiInitSDDevice();
	}
#endif



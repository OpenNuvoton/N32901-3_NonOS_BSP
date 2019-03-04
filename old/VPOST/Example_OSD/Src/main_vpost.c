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

#include "w55fa93_vpost.h"
#include "w55fa93_reg.h"
#include "w55fa93_osd.h"

__align (32) UINT8 g_ram0[512*16*16];
__align (32) UINT8 g_ram1[512*16*16];
UINT32 u32SecCnt;
UINT32 u32backup[10];


__align(32) UINT8 Vpost_Frame[]=
{
//	#include "roof_800x600_rgb565.dat"		// for SVGA size test
//	#include "sea_800x480_rgb565.dat"		
//	#include "roof_800x480_rgb565.dat"			
//	#include "roof_720x480_rgb565.dat"		// for D1 size test
//	#include "lake_720x480_rgb565.dat"		// for D1 size test
//	#include "mountain_640x480_rgb565.dat"	// for VGA size test	
//	#include "river_480x272_rgb565.dat"
//	#include "roof_320x240_rgb565.dat"	
#ifdef N32903
	#include "roof_320x240_yuv422.dat"	
#endif

#ifdef N32901
	#include "roof_320x240_yuv422.dat"	
#endif
	
//	#include "roof_320x240_rgbx888.dat"
#ifdef FA93SDN	
//    #include "nuvoton_320x240_rgb565.dat"		
	#include "river_480x272_rgb565.dat"	
#endif
    
};


__align(32) UINT8 OSD_FrameRGB565[] = 
{
#ifdef FA93SDN
	#include "roof_320x240_rgb565.dat"
#endif

#ifdef N32903
	#include "roof_320x240_yuv422.dat"	
#endif

#ifdef N32901
	#include "roof_320x240_yuv422.dat"	
#endif	
	
};

LCDFORMATEX lcdFormat;
OSDFORMATEX osdFormat;

int volatile gTotalSectors_SD = 0, gTotalSectors_SM = 0;


void initPLL(void)
{
}


int count = 0;
int STATUS;

int main(void)
{
	UINT32 OSDX1End, OSDY1End, XBarOffset, YBarOffset,result;
//	InitUART();
//	g_SetupTOK =0;
//	g_SetupPKT=0;
//	g_OutTOK = 0;
//	g_SetupInTOK=0;
//	g_TXD=0;
//	g_RXD = 0;
	 
//	DrvSIO_printf("TEST %d\n",count);

	
	initPLL();
	
//	lcdFormat.ucVASrcFormat = DRVVPOST_FRAME_RGB565;
//	lcdFormat.ucVASrcFormat = DRVVPOST_FRAME_YCBYCR;
//	lcdFormat.ucVASrcFormat = DRVVPOST_FRAME_RGBx888;

//	lcdFormat.nScreenWidth = 800;
//	lcdFormat.nScreenHeight = 600;

//	lcdFormat.nScreenWidth = 640;
//	lcdFormat.nScreenHeight = 480;
	
//	lcdFormat.nScreenWidth = 720;
//	lcdFormat.nScreenHeight = 480;

#ifdef FA93SDN		
	lcdFormat.ucVASrcFormat = DRVVPOST_FRAME_RGB565;   // for FA93SDN
	lcdFormat.nScreenWidth = 480;
	lcdFormat.nScreenHeight = 272;
#endif

#ifdef N32903
	lcdFormat.ucVASrcFormat = DRVVPOST_FRAME_YCBYCR;
	lcdFormat.nScreenWidth = 320;
	lcdFormat.nScreenHeight = 240;
#endif

#ifdef N32901
	lcdFormat.ucVASrcFormat = DRVVPOST_FRAME_YCBYCR;	
	lcdFormat.nScreenWidth = 320;
	lcdFormat.nScreenHeight = 240;
#endif	
	  
	vpostLCMInit(&lcdFormat, (UINT32*)Vpost_Frame);

#ifdef FA93SDN	
	// OSD Demo 	
	osdFormat.ucOSDSrcFormat = DRVVPOST_OSD_RGB565;
#endif

#ifdef N32903
	osdFormat.ucOSDSrcFormat = DRVVPOST_FRAME_YCBYCR;
#endif

#ifdef N32901
	osdFormat.ucOSDSrcFormat = DRVVPOST_FRAME_YCBYCR;
#endif	
	
	osdFormat.nOSDScreenWidth = 320;
	osdFormat.nOSDScreenHeight = 240;	
	osdFormat.nOSDX1StartPixel = 0;		
	osdFormat.nOSDY1StartPixel = 0;		
	vpostOSDInit(&osdFormat, (UINT32*)OSD_FrameRGB565 );


	
	
	vpostOSDControl(eOSD_SHOW, 0);
	
	do{
	
		vpostOSDControl(eOSD_SET_TRANSPARENT, 0xF800);	// Red Transparent
		sysDelay(100);
		
		vpostOSDControl(eOSD_SET_TRANSPARENT, 0x07E0);	// Green Transparent
		sysDelay(100);	
		
		vpostOSDControl(eOSD_SET_TRANSPARENT, 0x001F);	// Blue Transparent
		sysDelay(100);	
	
		vpostOSDControl(eOSD_CLEAR_TRANSPARENT, 0x0);	// Disable Transparent	
		
		vpostGetBarXY(&OSDX1End, &OSDY1End,&XBarOffset,&YBarOffset);
		sysDelay(100);		
		result = vpostSpitOSD(OSDX1End+10, OSDY1End+10, XBarOffset+10, YBarOffset+10);
		
		if (result)
		{
			vpostOSDControl(eOSD_HIDE,0);
			vpostSpitOSD(1, 1, 1, 1);	// Merge 2 OSD Bar again
			vpostOSDControl(eOSD_SHOW, 0);			
		}			
			
		
	} while(1);
	
	while(1);

}



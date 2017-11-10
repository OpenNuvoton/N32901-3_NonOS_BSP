/***************************************************************************
 *                                                                         									     *
 * Copyright (c) 2008 Nuvoton Technolog. All rights reserved.              					     *
 *                                                                         									     *
 ***************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include "wblib.h"
#include "nvtfat.h"
#include "AviLib.h"
#include "W55FA93_reg.h"
#include "W55FA93_Vpost.h"
#include "USB.h"
#include "Font.h"
#include "nvtloader.h"
#include "emuProduct.h"

EMU_EDMA(
	S_DEMO_FONT*	ptFont,
	UINT32 			u32FrameBufAddr	
)
{
	INT	   	nStatus, u32Ypos=0;
	char 	Array1[64];
	
	Font_ClrFrameBuffer(u32FrameBufAddr);
	sprintf(Array1, "EDMA Test...");
	DemoFont_PaintA(ptFont,					
						0,					
						u32Ypos,			
						Array1);
							
}
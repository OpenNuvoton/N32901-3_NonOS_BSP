/***************************************************************************
 *                                                                         *
 * Copyright (c) 2008 Nuvoton Technolog. All rights reserved.              *
 *                                                                         *
 ***************************************************************************/
 
/****************************************************************************
* FILENAME
*   wb_timer.c
*
* VERSION
*   1.0
*
* DESCRIPTION
*   The timer related function of Nuvoton ARM9 MCU
*
* HISTORY
*   2008-06-25  Ver 1.0 draft by Min-Nan Cheng
*
* REMARK
*   None
 **************************************************************************/

#include "wblib.h"
#include "stdlib.h"

/* nv memory function, user could modify it */
void *nv_malloc(UINT32 u32byteno)
{
  	return (void *)malloc(u32byteno);
}

void nv_free(void *buffer)
{
    free(buffer);
}



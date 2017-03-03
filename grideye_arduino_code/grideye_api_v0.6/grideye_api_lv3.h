/*******************************************************************************
 * Copyright(C) by 2015 Panasonic Corporation.
 ******************************************************************************/
#ifndef	__GRIDEYE_API_LV3_H
#define	__GRIDEYE_API_LV3_H

/*******************************************************************************
	include file
*******************************************************************************/
#include	"grideye_api_common.h"

/*******************************************************************************
	public method definition
*******************************************************************************/
BOOL	bAMG_PUB_ODT_Initialize( UCHAR, UCHAR, ULONG, USHORT );
BOOL	bAMG_PUB_ODT_SetPrm( UCHAR, UCHAR );
BOOL	bAMG_PUB_ODT_Execute( short*, short*, short*, UCHAR*, UCHAR*, USHORT* );

BOOL	bAMG_PUB_OTR_Initialize( UCHAR, UCHAR, UCHAR, UCHAR, UCHAR, ULONG, USHORT );
BOOL	bAMG_PUB_OTR_SetPrm( UCHAR, short );
BOOL	bAMG_PUB_OTR_Execute   ( short*, short*, short*, UCHAR, UCHAR*, USHORT* );
BOOL	bAMG_PUB_OTR_GetResultByLabel(UCHAR, UCHAR, short* );
BOOL	bAMG_PUB_OTR_GetOutput( UCHAR*, UCHAR* );
BOOL	bAMG_PUB_BGT_SetPrm(UCHAR, short );
void	vAMG_PUB_BGT_UpdateBackTemp( USHORT, UCHAR*, short*, short* );

#endif	/* __GRIDEYE_API_LV3_H */

/*******************************************************************************
 * Copyright(C) by 2015 Panasonic Corporation.
 ******************************************************************************/

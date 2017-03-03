#ifndef	__GRIDEYE_API_LV1_H
#define	__GRIDEYE_API_LV1_H


/*******************************************************************************
	include file
*******************************************************************************/
#include	"grideye_api_common.h"


/*******************************************************************************
	public method definition
*******************************************************************************/
BOOL	 bAMG_PUB_I2C_Read( UCHAR, UCHAR, UCHAR, UCHAR* );
short	shAMG_PUB_TMP_ConvThermistor( UCHAR[2] );
short	shAMG_PUB_TMP_ConvTemperature( UCHAR[2] );
void	 vAMG_PUB_TMP_ConvTemperature64( UCHAR*, short* );
short	shAMG_PUB_CMN_ConvFtoS( float );
float	 fAMG_PUB_CMN_ConvStoF( short );


#endif	/* __GRIDEYE_API_LV1_H */

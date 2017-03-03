/*******************************************************************************
	include file
*******************************************************************************/
#include	"grideye_api_common.h"


/*******************************************************************************
	macro definition
*******************************************************************************/

/* Grid-EYE's number of pixels */
#define		SNR_SZ_X			(8)
#define		SNR_SZ_Y			(8)
#define		SNR_SZ				(SNR_SZ_X * SNR_SZ_Y)


/*******************************************************************************
	public method
 ******************************************************************************/

/*------------------------------------------------------------------------------
	Read data form I2C bus.
------------------------------------------------------------------------------*/
BOOL bAMG_PUB_I2C_Read( UCHAR ucI2cAddr, UCHAR ucRegAddr, UCHAR ucSize, UCHAR* ucDstAddr )
{
	#if			defined(MCU_TEST)
	{
		extern UCHAR I2C_Read( UCHAR, UCHAR, UCHAR*, UCHAR );
		return( I2C_Read( ucI2cAddr, ucRegAddr, ucDstAddr, ucSize ) );
	}
	#else	/* !defined(MCU_TEST) */
	{
		return( 1 );
	}
	#endif	/*  defined(MCU_TEST) */
}

/*------------------------------------------------------------------------------
	Convert thermistor register value.
------------------------------------------------------------------------------*/
short shAMG_PUB_TMP_ConvThermistor( UCHAR aucRegVal[2] )
{
	short shVal = ((short)(aucRegVal[1] & 0x07) << 8) | aucRegVal[0];

	if( 0 != (0x08 & aucRegVal[1]) )
	{
		shVal *= -1;
	}

	shVal *= 16;

	return( shVal );
}

/*------------------------------------------------------------------------------
	Convert temperature register value for 1 pixel.
------------------------------------------------------------------------------*/
short shAMG_PUB_TMP_ConvTemperature( UCHAR aucRegVal[2] )
{
	short shVal = ((short)(aucRegVal[1] & 0x07) << 8) | aucRegVal[0];

	if( 0 != (0x08 & aucRegVal[1]) )
	{
		shVal -= 2048;
	}

	shVal *= 64;

	return( shVal );
}

/*------------------------------------------------------------------------------
	Convert temperature register value for 64 pixel.
------------------------------------------------------------------------------*/
void vAMG_PUB_TMP_ConvTemperature64( UCHAR* pucRegVal, short* pshVal )
{
	UCHAR ucCnt = 0;

	for( ucCnt = 0; ucCnt < SNR_SZ; ucCnt++ )
	{
		pshVal[ucCnt] = shAMG_PUB_TMP_ConvTemperature( pucRegVal + ucCnt * 2 );
	}
}

/*------------------------------------------------------------------------------
	Convert value.
------------------------------------------------------------------------------*/
short shAMG_PUB_CMN_ConvFtoS( float fVal )
{
	return( ( fVal > 0 ) ? (short)(fVal * 256 + 0.5) : (short)(fVal * 256 - 0.5) );
}

/*------------------------------------------------------------------------------
	Convert value.
------------------------------------------------------------------------------*/
float fAMG_PUB_CMN_ConvStoF( short shVal )
{
	return( (float)shVal / 256 );
}

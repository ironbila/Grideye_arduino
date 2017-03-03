/*******************************************************************************
	include file
*******************************************************************************/
#include	"grideye_api_common.h"
#include	"grideye_api_lv1.h"


/*******************************************************************************
	macro definition
*******************************************************************************/
#define		TRUE				(1)
#define		FALSE				(0)

/* Grid-EYE's I2C slave address */
#define		GRIDEYE_ADR_GND		(0xD0)	/* AD_SELECT pin connect to GND		*/
#define		GRIDEYE_ADR_VDD		(0xD2)	/* AD_SELECT pin connect to VDD		*/
#define		GRIDEYE_ADR			GRIDEYE_ADR_GND

/* Grid-EYE's register address */
#define		GRIDEYE_REG_THS00	(0x0E)	/* head address of thermistor  resister	*/
#define		GRIDEYE_REG_TMP00	(0x80)	/* head address of temperature resister	*/

/* Grid-EYE's register size */
#define		GRIDEYE_REGSZ_THS	(0x02)	/* size of thermistor  resister		*/
#define		GRIDEYE_REGSZ_TMP	(0x80)	/* size of temperature resister		*/

/* Grid-EYE's number of pixels */
#define		SNR_SZ_X			(8)
#define		SNR_SZ_Y			(8)
#define		SNR_SZ				(SNR_SZ_X * SNR_SZ_Y)


/*******************************************************************************
	variable value definition
*******************************************************************************/
short	g_shThsTemp;					/* thermistor temperature			*/
short	g_ashRawTemp[SNR_SZ];			/* temperature of 64 pixels			*/


/*******************************************************************************
	method
 ******************************************************************************/

/*------------------------------------------------------------------------------
	Read temperature from Grid-EYE.
------------------------------------------------------------------------------*/
BOOL bReadTempFromGridEYE( void )
{
	UCHAR aucThsBuf[GRIDEYE_REGSZ_THS];
	UCHAR aucTmpBuf[GRIDEYE_REGSZ_TMP];

	/* Get thermistor register value. */
	if( FALSE == bAMG_PUB_I2C_Read( GRIDEYE_ADR, GRIDEYE_REG_THS00, GRIDEYE_REGSZ_THS, aucThsBuf ) )
	{
		return( FALSE );
	}

	/* Convert thermistor register value. */
	g_shThsTemp = shAMG_PUB_TMP_ConvThermistor( aucThsBuf );

	/* Get temperature register value. */
	if( FALSE == bAMG_PUB_I2C_Read( GRIDEYE_ADR, GRIDEYE_REG_TMP00, GRIDEYE_REGSZ_TMP, aucTmpBuf ) )
	{
		return( FALSE );
	}

	/* Convert temperature register value. */
	vAMG_PUB_TMP_ConvTemperature64( aucTmpBuf, g_ashRawTemp );

	return( TRUE );
}

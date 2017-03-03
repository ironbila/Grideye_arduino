/*******************************************************************************
 * Copyright(C) by 2015 Panasonic Corporation.
 ******************************************************************************/

/*******************************************************************************
	include file
*******************************************************************************/
#include	"grideye_api_common.h"
#include	"grideye_api_lv1.h"
#include	"grideye_api_lv2.h"
#include	"grideye_api_lv3.h"

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

/* Setting size of human detection */
#define		IMG_SZ_X			(SNR_SZ_X * 2 - 1)
#define		IMG_SZ_Y			(SNR_SZ_Y * 2 - 1)
#define		IMG_SZ				(IMG_SZ_X * IMG_SZ_Y)
#define		OUT_SZ_X			(2)
#define		OUT_SZ_Y			(2)
#define		OUT_SZ				(OUT_SZ_X * OUT_SZ_Y)

/* Parameters of human detection */
#define		TEMP_FRAME_NUM		(8)
#define		TEMP_MEDIAN_FILTER	(2)
#define		TEMP_SMOOTH_COEFF	(0.1f)
#define		DIFFTEMP_THRESH		(0.3f)
#define		DETECT_MARK			((UCHAR)0xFF)
#define		LABELING_THRESH		(3)
#define		OUTPUT_THRESH		(6)
#define		BKUPDT_COEFF		(0.1f)
#define		HUMAN_JUDGE_AREA	(4)
#define		HUMAN_JUDGE_FRAME	(1)

/* Parameters of background update */
#define		BKUPDT_ALLUP_THRESH	(0.9f)	/* The ratio of the number of pixels that detected the object	*/
#define		BKUPDT_ALLUP_TEMP	(0.1f)	/* Uniform update temperature			*/


/* memory size of object detection */
#define		SIZE_OF_ODT			(IMG_SZ+0x20)

/* memory size of object tracking */
#define		TRACKING_MAX		(5)
#define		SIZE_OF_OTR			((TRACKING_MAX+1) * 0x138 )

/*******************************************************************************
	variable value definition
*******************************************************************************/
ULONG	g_ulFrameNum = 0;
short	g_a2shRawTemp[TEMP_FRAME_NUM][SNR_SZ];
short	g_ashSnrAveTemp[SNR_SZ];
short	g_ashAveTemp   [IMG_SZ];
short	g_ashBackTemp  [IMG_SZ];
short	g_ashDiffTemp  [IMG_SZ];
UCHAR	g_aucDetectImg [IMG_SZ];
UCHAR	g_aucOutputImg [OUT_SZ];
USHORT	g_ausWork      [IMG_SZ];
UCHAR	g_aucODTbuf[SIZE_OF_ODT];
UCHAR	g_aucOTRbuf[SIZE_OF_OTR];

/*******************************************************************************
	method
 ******************************************************************************/

/*------------------------------------------------------------------------------
	Sample program of human detection.
------------------------------------------------------------------------------*/
BOOL bAMG_PUB_SMP_InitializeHumanDetectLv3Sample( void )
{
	USHORT	usCnt = 0;

	/* Initialize data */
	g_ulFrameNum = 0;
	for( usCnt = 0; usCnt < SNR_SZ; usCnt++ )
	{
		UCHAR ucFrameCnt = 0;
		for( ucFrameCnt = 0; ucFrameCnt < TEMP_FRAME_NUM; ucFrameCnt++ )
		{
			g_a2shRawTemp[ucFrameCnt][usCnt] = 0;
		}
		g_ashSnrAveTemp[usCnt] = 0;
	}
	for( usCnt = 0; usCnt < IMG_SZ; usCnt++ )
	{
		g_ashAveTemp   [usCnt] = 0;
		g_ashBackTemp  [usCnt] = 0;
		g_ashDiffTemp  [usCnt] = 0;
		g_aucDetectImg [usCnt] = 0;
	}
	for( usCnt = 0; usCnt < OUT_SZ; usCnt++ )
	{
		g_aucOutputImg [usCnt] = 0;
	}

	/* Initialize object detection. */
	if( FALSE == bAMG_PUB_ODT_Initialize( IMG_SZ_X, IMG_SZ_Y, (ULONG)g_aucODTbuf, (USHORT)SIZE_OF_ODT ) )
	{
		return( FALSE );
	}

	/* Initialize object tracking. */
	if( FALSE == bAMG_PUB_OTR_Initialize( IMG_SZ_X, IMG_SZ_Y, OUT_SZ_X, OUT_SZ_Y, TRACKING_MAX, (ULONG)g_aucOTRbuf, (USHORT)SIZE_OF_OTR ) )
	{
		return( FALSE );
	}
	
	/* Set of human detection parameters. */
	if( FALSE == bAMG_PUB_ODT_SetPrm( 0, 102 ))
	{
		return( FALSE );
	}
	if( FALSE == bAMG_PUB_ODT_SetPrm( 1, LABELING_THRESH ))
	{
		return( FALSE );
	}
	
	/* Set of object tracking parameters. */
	if( FALSE == bAMG_PUB_OTR_SetPrm( 0, HUMAN_JUDGE_AREA ))
	{
		return( FALSE );
	}
	if( FALSE == bAMG_PUB_OTR_SetPrm( 1, HUMAN_JUDGE_FRAME ))
	{
		return( FALSE );
	}
	if( FALSE == bAMG_PUB_OTR_SetPrm( 2, TRUE ))
	{
		return( FALSE );
	}
	
	/* Set of Update background parameters. */
	if( FALSE == bAMG_PUB_BGT_SetPrm(0,shAMG_PUB_CMN_ConvFtoS(BKUPDT_ALLUP_THRESH)))
	{
		return( FALSE );				/* Program NG */
	}
	if( FALSE == bAMG_PUB_BGT_SetPrm(1,shAMG_PUB_CMN_ConvFtoS(BKUPDT_ALLUP_TEMP)))
	{
		return( FALSE );				/* Program NG */
	}

	return( TRUE );
}

/*------------------------------------------------------------------------------
	Sample program of human detection.
------------------------------------------------------------------------------*/
BOOL bAMG_PUB_SMP_ExecuteHumanDetectLv3Sample( void )
{
	USHORT	usCnt = 0;
	UCHAR	ucDetectNum = 0;
	UCHAR	ucApproximateNum;

	/* Get temperature register value. */
	if( FALSE == bAMG_PUB_I2C_Read( GRIDEYE_ADR, GRIDEYE_REG_TMP00, GRIDEYE_REGSZ_TMP, (UCHAR*)g_ausWork ) )
	{
		return( FALSE );				/* Communication NG */
	}

	/* Convert temperature register value. */
	vAMG_PUB_TMP_ConvTemperature64( (UCHAR*)g_ausWork, g_a2shRawTemp[g_ulFrameNum % TEMP_FRAME_NUM] );

	/* Increment number of measurement. */
	g_ulFrameNum++;
	if( TEMP_FRAME_NUM > g_ulFrameNum )
	{
		return( FALSE );				/* Initial process */
	}

	/* Calculate average. */
	for( usCnt = 0; usCnt < SNR_SZ; usCnt++ )
	{
		short shAveTemp = shAMG_PUB_CMN_CalcAve( &g_a2shRawTemp[0][usCnt], TEMP_FRAME_NUM, SNR_SZ, TEMP_MEDIAN_FILTER, (BOOL*)g_ausWork );
		if( TEMP_FRAME_NUM == g_ulFrameNum )
		{
			g_ashSnrAveTemp[usCnt] = shAveTemp;
		}
		else
		{
			g_ashSnrAveTemp[usCnt] = shAMG_PUB_CMN_CalcIIR( g_ashSnrAveTemp[usCnt], shAveTemp, shAMG_PUB_CMN_ConvFtoS(TEMP_SMOOTH_COEFF) );
		}
	}

	/* Linear interpolation. */
	if( FALSE == bAMG_PUB_IMG_LinearInterpolationSQ15( g_ashSnrAveTemp, g_ashAveTemp ) )
	{
		return( FALSE );				/* Program NG */
	}

	/* Initialize background temperature. */
	if( TEMP_FRAME_NUM == g_ulFrameNum )
	{
		for( usCnt = 0; usCnt < IMG_SZ; usCnt++ )
		{
			g_ashBackTemp[usCnt] = g_ashAveTemp[usCnt];
		}
		return( FALSE );				/* Initial process */
	}

	/* Object detection. */
	if( FALSE == bAMG_PUB_ODT_Execute( g_ashAveTemp, g_ashBackTemp, g_ashDiffTemp, &ucDetectNum, g_aucDetectImg ,g_ausWork))
	{
		return( FALSE );				/* Program NG */
	}

	/* Object tracking. */
	if( FALSE == bAMG_PUB_OTR_Execute( g_ashAveTemp, g_ashBackTemp, g_ashDiffTemp, ucDetectNum, g_aucDetectImg, g_ausWork))
	{
		return( FALSE );				/* Program NG */
	}

	/* Update output image. */
	if( FALSE == bAMG_PUB_OTR_GetOutput( g_aucOutputImg, &ucApproximateNum ))
	{
		return( FALSE );				/* Program NG */
	}

	/* Update background temperature. */
	if( FALSE == bAMG_PUB_IMG_ImageDilation1( IMG_SZ_X, IMG_SZ_Y, g_aucDetectImg, (UCHAR*)g_ausWork ) )
	{
		return( FALSE );				/* Program NG */
	}

	vAMG_PUB_BGT_UpdateBackTemp( IMG_SZ, (UCHAR*)g_ausWork, g_ashDiffTemp, g_ashBackTemp );

	return( TRUE );						/* human detection OK */
}

/*******************************************************************************
 * Copyright(C) by 2015 Panasonic Corporation.
 ******************************************************************************/

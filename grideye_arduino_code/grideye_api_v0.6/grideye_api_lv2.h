#ifndef	__GRIDEYE_API_LV2_H
#define	__GRIDEYE_API_LV2_H


/*******************************************************************************
	include file
*******************************************************************************/
#include	"grideye_api_common.h"


/*******************************************************************************
	public method definition
*******************************************************************************/
short	shAMG_PUB_CMN_CalcAve( short*, USHORT, UCHAR, UCHAR, BOOL* );
short	shAMG_PUB_CMN_CalcIIR( short, short, short );
short	shAMG_PUB_CMN_CalcIIR_f( short, short, float );

void	vAMG_PUB_IMG_ConvertFlipX    ( UCHAR, UCHAR, short*, short* );
void	vAMG_PUB_IMG_ConvertFlipY    ( UCHAR, UCHAR, short*, short* );
BOOL	bAMG_PUB_IMG_ConvertFlipXY   ( UCHAR, UCHAR, short*, short* );
BOOL	bAMG_PUB_IMG_ConvertRotate90 ( UCHAR, UCHAR, short*, short* );
void	vAMG_PUB_IMG_ConvertRotate180( UCHAR, UCHAR, short*, short* );
BOOL	bAMG_PUB_IMG_ConvertRotate270( UCHAR, UCHAR, short*, short* );

BOOL	bAMG_PUB_IMG_LinearInterpolationSQ15( short*, short* );
BOOL	bAMG_PUB_IMG_LinearInterpolation( UCHAR, UCHAR, short*, short* );

BOOL	bAMG_PUB_IMG_ImageDilation1( UCHAR, UCHAR, UCHAR*, UCHAR* );
BOOL	bAMG_PUB_IMG_ImageDilation2( UCHAR, UCHAR, UCHAR, UCHAR*, UCHAR* );

void	vAMG_PUB_ODT_CalcDiffImage   ( USHORT, short*, short*, short* );
void	vAMG_PUB_ODT_CalcDetectImage1( USHORT, short*, short,  UCHAR, UCHAR* );
void	vAMG_PUB_ODT_CalcDetectImage2( USHORT, short*, short*, UCHAR, UCHAR* );

UCHAR	ucAMG_PUB_ODT_CalcDataLabeling8( UCHAR, UCHAR, UCHAR, USHORT, UCHAR*, USHORT* );

BOOL	bAMG_PUB_FEA_CalcArea        ( USHORT,       UCHAR, UCHAR*,         USHORT* );
BOOL	bAMG_PUB_FEA_CalcRectangle   ( UCHAR, UCHAR, UCHAR, UCHAR*,         UCHAR*  );
BOOL	bAMG_PUB_FEA_CalcMinTemp     ( USHORT,       UCHAR, UCHAR*, short*, short*  );
BOOL	bAMG_PUB_FEA_CalcMaxTemp     ( USHORT,       UCHAR, UCHAR*, short*, short*  );
BOOL	bAMG_PUB_FEA_CalcAveTemp     ( USHORT,       UCHAR, UCHAR*, short*, short*  );
BOOL	bAMG_PUB_FEA_CalcStdDevTemp  ( USHORT,       UCHAR, UCHAR*, short*, USHORT* );
BOOL	bAMG_PUB_FEA_CalcStdDevTemp_f( USHORT,       UCHAR, UCHAR*, short*, float*  );
BOOL	bAMG_PUB_FEA_CalcCenterTemp  ( UCHAR, UCHAR, UCHAR, UCHAR*, short*, short*  );
BOOL	bAMG_PUB_FEA_CalcCenterTemp_f( UCHAR, UCHAR, UCHAR, UCHAR*, short*, float*  );

BOOL	bAMG_PUB_BGT_UpdateBackTemp( USHORT, UCHAR*, short*, short, short* );

BOOL	bAMG_PUB_HDT_JudgeHuman( USHORT, USHORT );

BOOL	bAMG_PUB_OUT_CalcOutImage  ( UCHAR, UCHAR, UCHAR, UCHAR, short*, UCHAR* );
BOOL	bAMG_PUB_OUT_CalcOutImage_f( UCHAR, UCHAR, UCHAR, UCHAR, float*, UCHAR* );


#endif	/* __GRIDEYE_API_LV2_H */

/*
 * Hw_grid_eye.h
 *
 * Created: 2015/5/21 11:23:26
 *  Author: Sam
 */ 
#ifndef HW_GRID_EYE_H_
#define HW_GRID_EYE_H_

//*****************************************************************************
//
//! \addtogroup GRID_EYE_Component Grid Eye
//! \brief Component GRID_EYE
//! @{
//
//*****************************************************************************
//*****************************************************************************
//
//! \addtogroup GRID_EYE_ENUMERATION Enumeration
//! \brief The configure macro for GRID_EYE
//! @{
//
//*****************************************************************************
//*****************************************************************************
//
//! \addtogroup GRID_EYE_Registers Grid Eye Registers Address
//! @{
//
//*****************************************************************************
#define GE_POWER_CTL_REG         0x00 ///< Setting operating mode of device
#define GE_RESET_REG             0x01 ///< Writing to reset software. 
#define GE_FPSC_REG              0x02 ///< Setting Frame Rate
#define GE_INT_CTL_REG           0x03 ///< Setting Interrupt Function.
#define GE_STAT_REG              0x04 ///< Indicate Overflow Flag and Interrupt Flag
#define GE_SCLR_REG              0x05 ///< Status Clear Register
#define GE_AVE_REG               0x07 ///< Setting moving average Output Mode
#define GE_INTHL_REG             0x08 ///< Interrupt upper value£¨Upper level£©
#define GE_INTHH_REG             0x09 ///< Interrupt upper value£¨Upper level£©
#define GE_INTLL_REG             0x0A ///< Interrupt lower value£¨Lower level£©
#define GE_INTLH_REG             0x0B ///< Interrupt lower value£¨upper level£©
#define GE_IHYSL_REG             0x0C ///< Interrupt hysteresis value£¨Lower level£©
#define GE_IHYSH_REG             0x0D ///< Interrupt hysteresis value£¨Lower level£©
#define GE_TTHL_REG              0x0E ///< Thermistor Output Value£¨Lower level£©
#define GE_TTHH_REG              0x0F ///< Thermistor Output Value£¨Upper level£©
#define GE_INT0_REG              0x10 ///< Pixel 1¡«8 Interrupt Result 
#define GE_INT1_REG              0x11 ///< Pixel 9¡«16 Interrupt Result  
#define GE_INT2_REG              0x12 ///< Pixel 17¡«24 Interrupt Result 
#define GE_INT3_REG              0x13 ///< Pixel 25¡«32 Interrupt Result 
#define GE_INT4_REG              0x14 ///< Pixel 33¡«40 Interrupt Result
#define GE_INT5_REG              0x15 ///< Pixel 41¡«48 Interrupt Result  
#define GE_INT6_REG              0x16 ///< Pixel 49¡«56 Interrupt Result
#define GE_INT7_REG              0x17 ///< Pixel 57¡«64 Interrupt Result

#define GE_PIXEL_BASE            0x80 ///< Pixel 1 Output Value (Lower Level)
#define GE_PIXEL_ADDR(n)         (GE_PIXEL_BASE + n)

//*****************************************************************************
//
//! @}
//
//*****************************************************************************
//*****************************************************************************
//
//! \addtogroup GRID_EYE_Registers Grid Eye Registers bit filed
//! @{
//
//*****************************************************************************
typedef union
{
	struct  
	{
		uint8_t  PCTL:8;                  ///< bit:  0.. 7 Setting operation mode bits
	}PCTL_b;
	uint8_t  PCTL;                        
}
GE_PCTL;

typedef union
{
	struct  
	{
		uint8_t  RST:8;                   ///< bit:  0.. 7 Setting operation mode bits
	}RST_b;
	uint8_t  RST;                         
}
GE_RST;


typedef union
{
	struct
	{
		uint8_t  FPS:1;                   ///< bit:  0     Setting Frame Mode 
	}FPS_b;
	uint8_t  FPS;                      
}
GE_FPSC;


typedef union
{
	struct  
	{
		uint8_t  INTEN:1;                 ///< bit:  0     enable interrupt output
		uint8_t  INTMOD:1;                ///< bit:  1     Setting interrupt Mode
	}INIC_b;
	uint8_t INIC;
}
GE_INTC;

typedef union
{
	struct  
	{
		uint8_t  _reserved0:1;            ///< bit:  0     reserved0
		uint8_t  INTF:1;                  ///< bit:  1     Interrupt Outbreak
		uint8_t  OVF_IRS:1;               ///< bit:  2     Temperature Output Overflow
		uint8_t  OVF_THS:1;               ///< bit:  3     Thermistor Temperature Output Overflow
	}STAT_b;
	uint8_t STAT;
}
GE_STAT;

typedef union
{
	struct  
	{
		uint8_t  _reserved0:1;            ///< bit:  0     reserved0
		uint8_t  INTCLR:1;                ///< bit:  1     Interrupt Flag Clear
		uint8_t  OVS_CLR:1;               ///< bit:  2     Temperature Output Overflow Flag Clear
		uint8_t  OVT_CLR:1;               ///< bit:  3     Thermistor Temperature Output Overflow Flag Clear
	}SCLR_b;
	uint8_t SCLR;
}
GE_SCLR;

typedef union
{
	struct
	{
		uint8_t  _reserved0:5;            ///< bit:  0.. 4 reserved0
		uint8_t  MAMOD:1;                 ///< bit:  5     Twice moving average Output Mode
	}AVE_b;
	uint8_t AVE;
}
GE_AVE;




//*****************************************************************************
//
//! @}
//
//*****************************************************************************
//*****************************************************************************
//
//! \addtogroup GRID_EYE_Registers Grid Eye Registers special macro define
//! @{
//
//*****************************************************************************
#define GE_PCTL_NORMAL_MODE            0x00 
#define GE_PCTL_SLEEEP_MODE            0x10 
#define GE_PCTL_STAND_BY_60S_MODE      0x20
#define GE_PCTL_STAND_BY_10S_MODE      0x21

#define GE_RST_FLAG_RST                0x30
#define GE_RST_INITIAL_RST             0x3F


#define GE_FPSC_1FPS                   0x01
#define GE_FPSC_10FPS                  0x00
//*****************************************************************************
//
//! @}
//
//*****************************************************************************
//*****************************************************************************
//
//! @}
//
//*****************************************************************************
//*****************************************************************************
//
//! @}
//
//*****************************************************************************

#endif /* HW_GRID_EYE_H_ */
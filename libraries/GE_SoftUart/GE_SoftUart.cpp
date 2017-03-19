#include "Arduino.h"
#include "GE_SoftUart.h"
#include "GE_SoftUartParse.h"

/******************************************************************************
  Description：
    RX_BIT_DELAY:
		[1].The best time: 122  Effective area:(115 ~ 128)
	TX_BIT_DELAY:
		[1].The best time: 102  Effective area:(95 ~ 109) 
******************************************************************************/
#define RX_BIT_DELAY   122                            /* A Bit of delay time */
#define TX_BIT_DELAY   102                            /* A Bit of delay time */

#define RX_PIN         ((PIOC->PIO_PDSR) & 0x04000000)/* digital pin 4(PC26) */
#define TX_PIN         5                              /* digital pin 5       */

uint8_t  ucRxData = 0;

/* Private functions */
void GE_SoftUartDelay( uint16_t BitDelay );
void GE_SoftUartRxCallBack( void );

/******************************************************************************
  Function：GE_SoftUartInit
  Description：Initialize Software serial port
  Input ：None
  Output：None
  Others：None
******************************************************************************/
void GE_SoftUartInit( void )
{
	pinMode(4, INPUT);
	attachInterrupt( 4, GE_SoftUartRxCallBack, FALLING );
	pinMode(5, OUTPUT);
	digitalWrite(5, HIGH);
}

/******************************************************************************
  Function：GE_SoftUartDelay
  Description：Delay function of the Software serial port
  Input ：
    BitDelay : The number of counts in the delay
  Output：None
  Others：None
******************************************************************************/
void GE_SoftUartDelay( uint16_t BitDelay)
{
	volatile int i;
    for( i = BitDelay; i > 0; i-- )
    {
     
    }
}

/******************************************************************************
  Function：GE_SoftUartRxCallBack
  Description：Used to response from Grid-Eye UART1_TX Pin interrupt
  Input ：None
  Output：None
  Others：None
******************************************************************************/
void GE_SoftUartRxCallBack( void )
{
    volatile uint32_t isr;
    uint8_t i;
	
   GE_SoftUartDelay(RX_BIT_DELAY/4);
   
   for( i = 8; i > 0; i-- )
   {
     GE_SoftUartDelay(RX_BIT_DELAY);

      ucRxData >>= 1;
     if( RX_PIN )
     {
         ucRxData |= 0x80;
     }
     else
     {
         ucRxData &= ~0x80;
      }        
   }
    
  GE_SoftUartDelay(RX_BIT_DELAY);

  if(RX_PIN)
  {
	 GE_MessageGet( GE_MassageGetStrcut( ), ucRxData );
  }

  /* Clear PendingIRQ */
  isr =  PIOC->PIO_ISR;  
}

/******************************************************************************
  Function：GE_SoftUartPutChar
  Description：Software serial port put char function
  Input ：
    ucTxData : Data need to be sent
  Output：None
  Others：None
******************************************************************************/
void GE_SoftUartPutChar( uint8_t ucTxData )
{
	uint8_t i;
	
	/* Start Bit*/
    digitalWrite(TX_PIN, LOW);  
	GE_SoftUartDelay(TX_BIT_DELAY);
	
	/* Write 8 bits */
    for ( i = 8; i > 0; i-- )
    {
        if (ucTxData & 1) 
	    {
			/* send 1 */
		    digitalWrite(TX_PIN, HIGH); 
	    }
        else
	    { 
           /* send 0 */
	       digitalWrite(TX_PIN, LOW);  
	    }
	    GE_SoftUartDelay(TX_BIT_DELAY);
	    ucTxData >>= 1;
    }
	
	/* Stop Bit */
    digitalWrite(TX_PIN, HIGH);  
    GE_SoftUartDelay(TX_BIT_DELAY);
}
#ifndef _GE_SOFTUART_PARSE_H_
#define _GE_SOFTUART_PARSE_H_
#include "Arduino.h"

/* Data statement */
#define GE_RECEIVE_IDLE         0     /* no received data      */
#define GE_RECEIVE_HEADER1      1     /* start receive Header  */
#define GE_RECEIVE_HEADER2      2     /* start receive Header  */
#define GE_RECEIVE_CMDLEN       3     /* start receive Header  */
#define GE_RECEIVE_CMDDATA      4     /* start receive CmdData */

/* Data statement */
#define GE_CMD_UPDATEFRE        '2'   /* Grid-Eye data Update frequency */

/* A serial port receiving character relevant variables */
typedef struct 
{
	uint32_t GE_ReceiveDataNum;       /* the conuter for the databuf */
	uint8_t  GE_ReceiveState;         /* receive state  */
	uint8_t  GE_ReceivecompleteFlg;   /* receive complete flag */
	uint8_t  GE_ReceiveDataLen;       /* The length of the data*/
	uint8_t  GE_ReceiveDataBuf[100];  /* data buffer */
} GE_MeaasgeTypedef;


void GE_CmdParse( void );
void GE_MessageGet( GE_MeaasgeTypedef *pMeaasge, uint8_t TX_data );
uint8_t GE_UpdateFreGet( void );
GE_MeaasgeTypedef* GE_MassageGetStrcut( void );

#endif /*_GE_SOFTUART_PARSE_H_*/
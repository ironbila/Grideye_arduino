#include "GE_SoftUartParse.h"

/* the variable is used to store PC CMD */
GE_MeaasgeTypedef pMeaasge;
uint8_t GE_UpdateFre = 10;
uint8_t  GE_CMDDataBuf[100];          /* CMD data buffer */

/* Private functions */
void GE_MassageBufAdd( GE_MeaasgeTypedef *pMeaasge, uint8_t TX_data );
void GE_MassageBufClear( GE_MeaasgeTypedef *pMeaasge );

/******************************************************************************
  Function：GE_MassageBufAdd
  Description：Used to retrieve the Grid-EYE update frequency
  Input:
      pMeaasge:Grid-EYE message struct pointer
      TX_data :Character of the PC was sent
  Output：None
  Others：None
******************************************************************************/
void GE_MassageBufAdd( GE_MeaasgeTypedef *pMeaasge, uint8_t TX_data )
{	
	pMeaasge->GE_ReceiveDataBuf[pMeaasge->GE_ReceiveDataNum] = TX_data;
	(pMeaasge->GE_ReceiveDataNum)++;
}

/******************************************************************************
  Function：GE_MassageBufClear
  Description：Used to clear pMessage.GE_ReceiveDataBuf[]
  Input:
      pMeaasge:Grid-EYE message struct pointer
  Output:None
  Others:None
******************************************************************************/
void GE_MassageBufClear( GE_MeaasgeTypedef *pMeaasge )
{
	for(int i = 0; i < pMeaasge->GE_ReceiveDataNum; i++)
	{
		pMeaasge->GE_ReceiveDataBuf[i] = 0;
	}
	pMeaasge->GE_ReceiveDataNum = 0;
}

/******************************************************************************
  Function：GE_MassageGetStrcut
  Description：Used to get pMeaasge pointer
  Input ：None
  Output：
    pMeaasge pointer
  Others：None
******************************************************************************/
GE_MeaasgeTypedef* GE_MassageGetStrcut( void )
{
	return (&pMeaasge);
}

/******************************************************************************
  Function：GE_UpdateFreGet
  Description：Used to retrieve the Grid-EYE update frequency
  Input：None
  Output：Grid-EYE update frequency
  Others：None
******************************************************************************/
uint8_t GE_UpdateFreGet( void )
{
	return GE_UpdateFre;
}

/******************************************************************************
  Function：GE_UpdateFreGet
  Description：Used to retrieve the Grid-EYE update frequency
  Input：None
  Output：Grid-EYE update frequency
  Others：None
******************************************************************************/
void GE_CmdParse( void )
{
	if(pMeaasge.GE_ReceivecompleteFlg == 1)
	{
		pMeaasge.GE_ReceivecompleteFlg = 0;
		switch(GE_CMDDataBuf[0])
		{
			case GE_CMD_UPDATEFRE:
			{
				GE_UpdateFre = GE_CMDDataBuf[1];
			}
			break;
			default:
			break;
		}	
	}
}

/******************************************************************************
  Function：GE_MessageGet
  Description：The Grid - Eye hook function,
              Used to Get PC message sent to Grid-Eye board 
  Input:
      pMeaasge:Grid-EYE message struct pointer
      TX_data :Character of the PC was sent
  Output：None
  Others：None
******************************************************************************/
void GE_MessageGet( GE_MeaasgeTypedef *pMeaasge, uint8_t TX_data )
{
	switch (pMeaasge->GE_ReceiveState)
	{
	 	case GE_RECEIVE_IDLE:
	 		{
		 		if(TX_data == '*')
		 		{
			 		pMeaasge->GE_ReceiveState = GE_RECEIVE_HEADER1;
					pMeaasge->GE_ReceiveDataNum = 0;
					pMeaasge->GE_ReceiveDataLen = 0;	
		 		}	 
	 		}
	 		break;
				 
		case GE_RECEIVE_HEADER1:
			{
				if(TX_data == '*')
				{
					pMeaasge->GE_ReceiveState = GE_RECEIVE_HEADER2;
				}
				else
				{
					/* Eliminate interference code, data reset, enter the idle state */
					pMeaasge->GE_ReceiveState = GE_RECEIVE_IDLE;
				}
			}
			break;
				
		case GE_RECEIVE_HEADER2:
			{
				if(TX_data == '*')
				{
					pMeaasge->GE_ReceiveState = GE_RECEIVE_CMDLEN;
				}
				else
				{
					/* Eliminate interference code, data reset, enter the idle state */
					pMeaasge->GE_ReceiveState = GE_RECEIVE_IDLE;
				}
			}
			break;	
						 
		case GE_RECEIVE_CMDLEN:
			{
				if(TX_data == '*')
				{
					pMeaasge->GE_ReceiveState = GE_RECEIVE_CMDLEN;
				}					
				else
				{
					pMeaasge->GE_ReceiveDataLen = TX_data;
					pMeaasge->GE_ReceiveState = GE_RECEIVE_CMDDATA;	
				}
					
			}
			break;
		case GE_RECEIVE_CMDDATA:
			{				
				GE_MassageBufAdd(pMeaasge, TX_data);
					
				/* Is the data receiving */
				if((pMeaasge->GE_ReceiveDataNum == pMeaasge->GE_ReceiveDataLen)) 
				{
					/* Complete CMD parse data reset, enter the idle state */
					for(int i = 0; i < pMeaasge->GE_ReceiveDataNum; i++)
					{
						GE_CMDDataBuf[i] = pMeaasge->GE_ReceiveDataBuf[i];
					}						
					
					pMeaasge->GE_ReceivecompleteFlg = 1;
					pMeaasge->GE_ReceiveState = GE_RECEIVE_IDLE;
				}
				else
				{
					pMeaasge->GE_ReceiveState = GE_RECEIVE_CMDDATA;
				}
			}
			break;
				
		default:
			break;
	}	
}

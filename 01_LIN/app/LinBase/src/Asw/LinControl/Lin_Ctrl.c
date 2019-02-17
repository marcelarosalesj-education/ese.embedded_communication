/****************************************************************************************************/
/**
\file       Lin_Ctrl.c
\brief      
\author     
\version    
\date       
*/
/****************************************************************************************************/

/*****************************************************************************************************
* Include files
*****************************************************************************************************/
/** Own headers */
#include  "Lin_Ctrl.h"
#include "Lin.h"

/*****************************************************************************************************
* Definition of module wide VARIABLEs 
*****************************************************************************************************/

/****************************************************************************************************
* Declaration of module wide FUNCTIONs 
****************************************************************************************************/

/****************************************************************************************************
* Definition of module wide MACROs / #DEFINE-CONSTANTs 
*****************************************************************************************************/

/***************************************************************************************************
* Declaration of module wide TYPEs 
*****************************************************************************************************/

/****************************************************************************************************
* Definition of module wide (CONST-) CONSTANTs 
*****************************************************************************************************/

uint8_t messageBuff1[] = "La tierra es redonda\n\r";
uint8_t messageBuff2[] = "EMBEDDED SYSTEMS RULES";
uint8_t indxLin = 0;
uint8_t rxValLin = 0;
uint8_t enablestate = true;
uint8_t stateTxLin = 0;
uint8_t stateBuffLin = 0;
uint8_t stateBaudLin =0;

/****************************************************************************************************
* Code of module wide FUNCTIONS
*****************************************************************************************************/

void LinCtrl_2ms( void )
{
    /*
    stateTxLin = Uart_SendByte(LIN2_ctrl, messageBuff1[indxLin]);
    if( stateTxLin == 0 )
    {
      indxLin++;
    }
    
    if( indxLin >= sizeof(messageBuff1) )
    {
      indxLin = 0;
    }
   
    rxValLin = Uart_GetByte(LIN2_ctrl);
    
    printf("%c", rxValLin);
    */
}

void LinCtrl_50ms( void )
{
   //stateBaudLin=Uart_SetBaudrate (UART4_ctrl,9600);
}

void LinCtrl_100ms( void )
{

  //stateBuffLin = Uart_SendBuffer(LIN4_ctrl, &messageBuff2[0], sizeof(messageBuff2));
  
  if( stateBuffLin == 0 )
  {
    printf("\n\r-Buffer sent-\n\r");
  }
}

void LinCtrl_TriggerEvent( void )
{
  if( enablestate == true )
  {
    enablestate = false;
  }
  else
  {
    enablestate = true;
  }
  
  
  //Uart_SetRxEnable(LIN2_ctrl, enablestate);
  //Uart_EnableInt(UART2_ctrl, UART_CFG_INT_TXRDY | UART_CFG_INT_RXRDY, enablestate);
  /* Example Code, need to be removed */
  /* These function handlers shall be invoked upon interrupt request */
  /* In your solution this trigger can start a buffer transmission */
}

/*******************************************************************************/

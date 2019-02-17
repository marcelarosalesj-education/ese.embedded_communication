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
#define LIN0_ctrl 0
#define LIN1_ctrl 1
#define LIN2_ctrl 2
#define LIN3_ctrl 3
#define LIN4_ctrl 4

/***************************************************************************************************
* Declaration of module wide TYPEs 
*****************************************************************************************************/

/****************************************************************************************************
* Definition of module wide (CONST-) CONSTANTs 
*****************************************************************************************************/

uint8_t messageBuff1[] = "La tierra es redonda";
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
   LinPduType pduInfo = {
      0,
      LIN_ENHANCED_CS,
      LIN_MASTER_RESPONSE,
      20,
      messageBuff1,
  };
  
  stateBuffLin = Lin_SendFrame(LIN2_ctrl, &pduInfo);
  
  if( stateBuffLin == 0 )
  {
    printf("\n\r-Buffer sent-\n\r");
  }
}

void LinCtrl_50ms( void )
{
   /* Nothing here */
}

void LinCtrl_100ms( void )
{
  LinPduType pduInfo = {
      0,
      LIN_CLASSIC_CS,
      LIN_SLAVE_RESPONSE,
      22,
      messageBuff2,
  };
  
  stateBuffLin = Lin_SendFrame(LIN4_ctrl, &pduInfo);
  
  if( stateBuffLin == 0 )
  {
    printf("\n\r-Buffer sent-\n\r");
  }
}

void LinCtrl_TriggerEvent( void )
{
  /* Nothing here */
}

/*******************************************************************************/

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

uint8_t messageBuffLin1[] = "Primer Mensaje";
uint8_t messageBuffLin2[] = "Segundo Mensaje";
uint8_t stateBuffLin = 0;

/****************************************************************************************************
* Code of module wide FUNCTIONS
*****************************************************************************************************/

void LinCtrl_2ms( void )
{
   LinPduType pduInfo = {
      0,
      LIN_ENHANCED_CS,
      LIN_MASTER_RESPONSE,
      14,
      messageBuffLin1,
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
      15,
      messageBuffLin2,
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

/*******************************************************************************/
/**
\file       Lin.h
\brief      
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   \version       1.0
   \author        
   \description   
   \date          
*/

#ifndef _LIN_H        /*prevent duplicated includes*/
	#define _LIN_H

	/** Standard Types */
  #include "Lin_Types.h"
	#include "Lin_Cfg.h"



  typedef enum
{
    
    IDLE,
    
    SEND_BREAK,
    
    SEND_SYNC,
    
    SEND_PID,
    
    SEND_RESPONSE,
    SEND_IDLE,
  
}LinStateType;
  
  typedef struct 
{
    
    uint16_t LinBreak;
    
    uint8_t LinByte;
    
    uint8_t LinID;
  
}LinSync;



  LinStateType LinState = SEND_IDLE;

  void Lin_Init ( const LinConfigType* Config);
  Std_ReturnType Lin_SendFrame ( uint8_t Channel, LinPduType* PduInfoPtr );
  Std_ReturnType Lin_GetSlaveResponse ( uint8_t Channel, uint8_t** LinSduPtr );
  void Lin_Isr(void);

/*============================================================================*/
#endif /* _LIN_H */
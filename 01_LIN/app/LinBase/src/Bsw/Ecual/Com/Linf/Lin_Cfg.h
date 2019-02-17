/*******************************************************************************/
/**
\file       Uart_Cfg.h
\brief      
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   \version       1.1
   \author        
   \description   
   \date          
*/
#ifndef _LIN_CFG_H
#define _LIN_CFG_H

  #include "Lin.h"

  typedef struct
  {
    uint8_t LinChannelId;
    uint16_t LinChannelBaudrate;
  }LinChannelType;

  typedef struct
  {
    uint8_t LinNumberOfChannels;
    LinChannelType *LinChannel;  
  }LinConfigType;
  
  extern LinConfigType LinChannelConfig[];
    
#endif /* _LIN_CFG_H */
/*******************************************************************************/
/**
\file       Lin_Cfg.c
\brief      
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   \version       1.1
   \author        
   \description   
   \date          
*/

#include "Lin.h"

#define CHANNEL_LIN1      1
#define BAUDRATE_9600     9600

LinChannelType LinCh =   {
    CHANNEL_LIN1,
    BAUDRATE_9600,
  };

LinConfigType LinChannelConfig[] = {
  1,
  &LinCh,
};
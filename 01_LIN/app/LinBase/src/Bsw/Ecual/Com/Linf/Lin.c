/*******************************************************************************/
/**
\file       
\brief      
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   \version       1.0
   \author        
   \description   
   \date          
*/

/** Scheduler function prototypes definitions */
#include "MemAlloc.h"
#include "Uart.h"
#include "Lin.h"

/*****************************************************************************************************
* Defines - 
*****************************************************************************************************/

/*****************************************************************************************************
* Definition of  VARIABLEs - 
*****************************************************************************************************/

uint8_t PduLinPid = 0;
LinFrameDlType SduDataLength = 0;
uint8_t LinNumChannels;
uint8_t LinChannelId;
uint32_t LinBaudrate;
LinStateType LinState;

/*****************************************************************************************************
* Definition of module wide (CONST-) CONSTANTs 
*****************************************************************************************************/

/*****************************************************************************************************
* Code of module wide Private FUNCTIONS
*****************************************************************************************************/

/*****************************************************************************************************
* Code of module wide Public FUNCTIONS
*****************************************************************************************************/

/**
 * \brief  Lin_StateHandler
 *
 *  \param
 */
void Lin_StateHandler( void ){
	switch(LinState){
		case SEND_IDLE:
		  /* Ensure ISR's used are disabled */
		  /* Enabling-Disabling Rx and Tx interrupts has to be handled accordingly */
		  Uart_EnableInt(0, UART_CFG_INT_RXRDY, 0);
		  Uart_EnableInt(0, UART_CFG_INT_TXRDY, 0);
		  break;
		case SEND_BREAK:
		  Uart_SendByte(0,0x00);
		  Uart_EnableInt(0, UART_CFG_INT_TXRDY, 1);
		  LinState = SEND_SYNC;
      Lin_StateHandler();
		  break;
		case SEND_SYNC:
		  Uart_SendByte(0,0x55);
		  LinState = SEND_PID;
      Lin_StateHandler();
		  break;
		case SEND_PID:
		  /* For the Lin project, you might need to calculate the PID */
		  Uart_SendByte(0, PduLinPid);
		  LinState = SEND_RESPONSE;
		  /*
			For the project you need to consider if you will wait for a response or will send a response, there fore LinState should be changed accordingly. 
			e.g. LinState = SEND_RESPONSE or LinState = GET_RESPONSE (additional state to handle RX from slaves)
			If GET_RESPONSE then Tx interrupts should be disabled, and Rx interrupts enabled. Both isr's should call Lin_Isr();
		  */
		  break;   
		case SEND_RESPONSE:
		  
		  /* 
			In your project, the number of data will be sent according to the data lenght from the PDU information, therefore a sub-state machine should be considered, or to handle here each byte sent. e.g.
			If ( LinResponseType == MASTER_RESPONSE)
			{
				If (DataSentCtrlCounter < SduDataLength )
				{
					Uart_SendByte(0,SduData[DataSentCtrlCounter]);
					DataSentCtrlCounter++;
					-> Need to calculate Checksum to be sent
				}
				else
				{
					Uart_SendByte(0,LinChecksum);
					DataSentCtrlCounter = 0;
					LinState = IDLE;
				}
			}
		  */
		  break; 
		default: /* Should not be reached */
		  break;
  }          
}

uint8_t Lin_CalculateChecksum(LinFrameCsModelType CStype, LinFramePidType pid, uint8_t * Data, uint8_t length)
{
    printf(Data);
    printf("\n\r");

    /* Sum all Data */
    uint16_t sum = 0;
    uint8_t index = 0;
    for(index = 0; index < length; index++)
    {
      sum += Data[index];
    }

    /* Add Pid if it is an Enhanced Checksum */
    if(CStype == LIN_CLASSIC_CS)
    {
      /* do nothing */
    }
    else if (CStype == LIN_ENHANCED_CS)
    {
      sum += pid;
    }
    else
    {
      /*Invalid Checksum Type*/
    }

    /* Handle Carry */
    if (sum >= 0x0100 )
    {
      uint8_t low = (sum & 0x00FF);
      uint8_t high = (sum & 0xFF00) >> 8;
      sum = low + high;
    }

    /* One's complement */
    sum = ~sum;

    /* Clean MSB, they're not needed */
    sum = sum & 0x00FF;

    return sum;
}

LinFramePidType Lin_CalculatePID(LinFramePidType id)
{
  uint8_t P0;
  uint8_t P1;
  P0 = ((id >> 0) & 1) ^ ((id >> 1) & 1) ^ ((id >> 2) & 1) ^ ((id >> 4) & 1);
  P1 = ((id >> 1) & 1) ^ ((id >> 3) & 1) ^ ((id >> 4) & 1) ^ ((id >> 5) & 1);
  P1 = ~P1;
  id = id | ((P0 & 1) << 6);
  id = id | ((P1 & 1) << 7);
  printf("the id %d \n\r", id);
  return id;
}

/**
 * \brief  
 *
 *  \param 
 */
void Lin_Init ( const LinConfigType* Config)
{
  LinState = IDLE;
    
  LinNumChannels = Config->LinNumberOfChannels;
  
  UartChannelType UartChannelConfig[LinNumChannels];
  int i=0;
  for(i = 0; i < LinNumChannels; i++) {
     UartChannelConfig[i].ChannelId = Config->LinChannel[i].LinChannelId;;
     UartChannelConfig[i].IsrEn = UART_CFG_INT_OVR_ERROR;
     UartChannelConfig[i].Mode = UART_CFG_MODE_LOOPBACK;
     UartChannelConfig[i].Parity = UART_CFG_PARITY_NO;
     UartChannelConfig[i].Baudrate = Config->LinChannel[i].LinChannelBaudrate;;
     UartChannelConfig[i].TxNotification = NULL;
     UartChannelConfig[i].RxNotification = NULL;
     UartChannelConfig[i].ErrorNotification = NULL;
  }
  
  UartConfigType UartCfg[] = { 
    {
      sizeof(UartChannelConfig)/sizeof(UartChannelType), //number of channels
      UART_CFG_PER_CLK,
      &UartChannelConfig[0]
    }
  } ;
   
  Uart_Init(&UartCfg[0]);
}
     
/**
 * \brief 
 *
 *  \param Channel  
 *  \param PduInfoPtr
 *  \return E_OK: Command successfuly executed.
 */
Std_ReturnType Lin_SendFrame ( uint8_t Channel, LinPduType* PduInfoPtr )
{
    if( LinState == IDLE){
      PduLinPid = PduInfoPtr->Pid;
      SduDataLength = PduInfoPtr->Dl;
      LinFrameCsModelType ChecksumType = PduInfoPtr->Cs;

      uint8_t SduIdx = 0;
      uint8_t SduData[SduDataLength];
      for (SduIdx = 0; SduIdx < SduDataLength; SduIdx++)
      {
        SduData[SduIdx] = PduInfoPtr->SduPtr[SduIdx];
      }

      uint8_t DataSentCtrlCounter = 0;
      LinFramePidType PID;
      uint8_t CS;
      PID = Lin_CalculatePID(PduLinPid);
      printf("PID is %d\n\r", PID);

      printf("Data Length is %d\n\r", SduDataLength);
      CS = Lin_CalculateChecksum(ChecksumType, PID, SduData, SduDataLength); /* Checksum is calculated on the PID, with parity */
      printf("Checksum is %x\n\r", CS);
      LinState = SEND_BREAK;
  	  Lin_StateHandler();
    }
    else{
      /*Command not accepted*/
    }
}


/**
 * \brief 
 *
 *  \param Channel  
 *  \param LinSduPtr
 */
Std_ReturnType Lin_GetSlaveResponse ( uint8_t Channel, uint8_t** LinSduPtr )
{

}

/**
 * \brief 
 *
 */
void Lin_Isr(void)
{
   Lin_StateHandler();   
}


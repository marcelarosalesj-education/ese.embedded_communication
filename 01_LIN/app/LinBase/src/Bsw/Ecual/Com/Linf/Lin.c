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

/* Frame's variables local for the Lin Module */
uint8_t Lin_Channel = 0;
LinFramePidType Lin_ID = 0;
LinFramePidType Lin_PID;
uint8_t Lin_CS = 0;
LinFrameCsModelType Lin_ChecksumType;
LinFrameResponseType Lin_ResponseType;
LinFrameDlType Lin_SduDataLength = 0;
uint8_t Lin_SduData[50];



/* Variables for the Lin Module */
uint8_t LinNumChannels;
LinStateType LinState;
uint8_t DataSentCtrlCounter = 0;
uint8_t wait_for_response = 1;
uint8_t ** Get_LinSduPtr;
Std_ReturnType ReturnCode = 0;


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
            Uart_EnableInt(Lin_Channel, UART_CFG_INT_RXRDY, 0);
            Uart_EnableInt(Lin_Channel, UART_CFG_INT_TXRDY, 0);
            break;
        case SEND_BREAK:
            ReturnCode = Uart_SendByte(Lin_Channel,0x00);
            if(ReturnCode == E_NOT_OK)
            {
                printf("Error with transmission\n\r");
                LinState = SEND_IDLE;
                break;
            }
            LinState = SEND_SYNC;
            Lin_StateHandler();
            break;
        case SEND_SYNC:
            ReturnCode = Uart_SendByte(Lin_Channel,0x55);
            if(ReturnCode == E_NOT_OK)
            {
                printf("Error with transmission\n\r");
                LinState = SEND_IDLE;
                break;
            }
            LinState = SEND_PID;
            Lin_StateHandler();
            break;
        case SEND_PID:
            /*
            For the project you need to consider if you will wait for a
            response or will send a response,
            there fore LinState should be changed accordingly.
            e.g. LinState = SEND_RESPONSE or LinState = GET_RESPONSE
            (additional state to handle RX from slaves)
            If GET_RESPONSE then Tx interrupts should be disabled,
            and Rx interrupts enabled. Both isr's should call Lin_Isr();
            */
            ReturnCode = Uart_SendByte(Lin_Channel, Lin_PID);
            if(ReturnCode == E_NOT_OK)
            {
                printf("Error with transmission\n\r");
                LinState = SEND_IDLE;
                break;
            }

            if(Lin_ResponseType == LIN_MASTER_RESPONSE)
            {
                LinState = SEND_RESPONSE;
            }
            else if(Lin_ResponseType == LIN_SLAVE_RESPONSE)
            {
                LinState = GET_RESPONSE;
                Uart_EnableInt(Lin_Channel, UART_CFG_INT_RXRDY, 1);
                Uart_EnableInt(Lin_Channel, UART_CFG_INT_TXRDY, 0);
            }
            Lin_StateHandler();
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
        case GET_RESPONSE:
        break;
        default: /* Should not be reached */
        break;
  }
}

uint8_t Lin_CalculateChecksum(LinFrameCsModelType CStype, LinFramePidType pid, uint8_t * Data, uint8_t length)
{
    /* Sum all Data */
    uint16_t sum = 0;
    uint8_t index = 0;
    for(index = 0; index < length; index++)
    {
      sum += Data[index];
    }

    /* Add Pid if it is an Enhanced Checksum */
    if (CStype == LIN_ENHANCED_CS)
    {
      sum += pid;
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
     UartChannelConfig[i].TxNotification = Lin_Isr;
     UartChannelConfig[i].RxNotification = Lin_Isr;
     UartChannelConfig[i].ErrorNotification = NULL;
  }
  
  UartConfigType UartCfg[] = {
    {
      sizeof(UartChannelConfig)/sizeof(UartChannelType), //number of channels
      UART_CFG_PER_CLK,
      &UartChannelConfig[0]
    }
  };

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
        uint8_t SduIdx = 0;
        /* Copy variables from structure to global variables of this LIN module */
        Lin_Channel = Channel;
        Lin_ID = PduInfoPtr->Pid;
        Lin_PID = Lin_CalculatePID(Lin_ID);
        Lin_SduDataLength = PduInfoPtr->Dl;
        for (SduIdx = 0; SduIdx < Lin_SduDataLength; SduIdx++)
        {
        Lin_SduData[SduIdx] = PduInfoPtr->SduPtr[SduIdx];
        }
        Lin_ChecksumType = PduInfoPtr->Cs;
        Lin_CS = Lin_CalculateChecksum(Lin_ChecksumType, Lin_PID, Lin_SduData, Lin_SduDataLength); /* Checksum is calculated on the PID, with parity */
        Lin_ResponseType = PduInfoPtr->Drc;

        /* Print information */
        printf("Data is: %s\n\r", Lin_SduData);
        printf("PID is: %d\n\r", Lin_PID);
        printf("Data Length is: %d\n\r", Lin_SduDataLength);
        printf("Checksum is: %x\n\r", Lin_CS);

        LinState = SEND_BREAK;
        Lin_StateHandler();
    } else{
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
    uint8_t rxVal = Uart_GetByte(Channel);
}

/**
 * \brief
 *
 */
void Lin_Isr(void)
{
    Lin_StateHandler();
}


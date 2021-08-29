#ifndef __COMBT_H__
#define __COMBT_H__

/************************************************
 *  		   INCLUDES SECTION
 ***********************************************/
//#include <Stdint.h>				
#include <Arduino.h>
#include "HardwareSerial.h"
#include <HX711.h>
/************************************************
 *   		DEFINES SECTION
 ***********************************************/
#define AdcDataType			uint32_t
#define BUFFER_SIZE			10

#define START_BYTE 			0xAA
#define STOP_BYTE 			0xBB
#define ACKNOWLODGE_BYTE 	0xCC
#define MAX_FRAME_SIZE		7
#define MIN_FRAME_SIZE		4
#define TIMEOUT_MS			100

/************************************************
 *  		TYPEDEF SECTION
 ***********************************************/
typedef enum e_indexFRAME{
	START,
	LENGTH,
	CMDID,
	DATA0,
	DATA1,
	DATA2,
}e_indexFRAME;

typedef enum e_UartErrors{
	NO_ERROR,
	NO_ANSWER,
	WRONG_ANSWER,
	WRONG_ACK,
	WRONG_DATA,
}e_UartErrors;

typedef enum e_UartCmdId{
	NO_CMD,
	GET_CONFIG,			// pido dato 	espero rta de tipo send_config
	SEND_CONFIG,		// envio dato 	espero rta de tipo ack
	SEND_ACK,			// envio 		no espero rta
	SEND_ADC_MEASURE,	// recibo dato 	envio ack
	CHECK_CONNECT,	// recibo dato 	envio ack
}e_UartCmdId;


typedef enum e_UartRxStates{
	IDLERx,
	RECEIVING_PACKET,
}e_UartRxStates;

typedef enum e_UartTxStates{
	IDLETx,
	SENDING_PACKET,
	WAITING,
}e_UartTxStates;

typedef struct{
	e_UartCmdId		currUartCMD		    = NO_CMD;
	e_UartRxStates 	currUartRxSTATE 	= IDLERx;
	e_UartTxStates 	currUartTxSTATE 	= IDLETx;
}t_UartStateMachine;	

/************************************************
 *   		GLOBAL DATA SECTION
 ***********************************************/
extern HX711 LoadCell;
extern HardwareSerial Serial;
extern uint8_t value_frec;
extern uint8_t value_mode;
extern volatile uint8_t bluetoothSt; 

/************************************************
 *   		FUNCTIONS DECLARATION
 ***********************************************/

/**
 * @brief This function runs the state machine.
 *
 * @param <Add InputName> <add description here>
 * @param <Add InputName> <add description here>
 * @param <Add InputName> <add description here>
 * @param <Add InputName> <add description here>
 *
 * @return <Add Return Informaiton here>
 */
void rxUartStateMachine(uint8_t ucReceivedByte);


/**
 * @brief This function runs the state machine.
 *
 * @param <Add InputName> <add description here>
 * @param <Add InputName> <add description here>
 * @param <Add InputName> <add description here>
 * @param <Add InputName> <add description here>
 *
 * @return <Add Return Informaiton here>
 */
void txUartStateMachine();

/**
 * @brief This function runs the state machine.
 *
 * @param <Add InputName> <add description here>
 * @param <Add InputName> <add description here>
 * @param <Add InputName> <add description here>
 * @param <Add InputName> <add description here>
 *
 * @return <Add Return Informaiton here>
 */
void sendADCValue();

#endif


// AT : Ceck the connection.
// AT+NAME : See default name
// AT+ADDR : see default address
// AT+VERSION : See version
// AT+UART : See baudrate
// AT+ROLE: See role of bt module(1=master/0=slave)
// AT+RESET : Reset and exit AT mode
// AT+ORGL : Restore factory settings
// AT+PSWD: see default password


/*	1  	    1	   1 		LNGT        1      
 [ START |LGTH | CMD_ID  | DATA(LGTH) | STOP ]
 */

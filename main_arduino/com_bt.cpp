/************************************************
 *   		INCLUDES
 ***********************************************/
#include "com_bt.h"
#include "menu.h"

/************************************************
 *			GLOBAL VARIABLE SECTION
 ***********************************************/
t_UartStateMachine 	uartSM;
e_UartCmdId		CMDqueue[2] = {NO_CMD, NO_CMD};
uint8_t 		CMDqueue_index = 0;

uint8_t frame_tx[MAX_FRAME_SIZE] = {0};
uint8_t frame_rx[MAX_FRAME_SIZE] = {0};
uint8_t waiting_ans	= false;

AdcDataType adcbuffer[BUFFER_SIZE];
uint8_t tail = 0;
uint8_t head = 0;

uint8_t frec, mode;
uint8_t adcvalue[3] = {0,0,0};


uint32_t timeout = TIMEOUT_MS; uint32_t NERRORES = 0;
extern menu_t PPAL, VIS, PAC, CONF;
extern menu_t *currMenu;

/************************************************
 *   		PRIVATE FUNCTION DEFINITION
 ***********************************************/
void pushCMD(e_UartCmdId cmd){
	if (CMDqueue_index < 2  )
		CMDqueue[CMDqueue_index++] = cmd;
}

e_UartCmdId popCMD(){
	e_UartCmdId aux;
	if (CMDqueue_index == 0) 
		return NO_CMD;

	CMDqueue_index--;
	aux = CMDqueue[CMDqueue_index];
	CMDqueue[CMDqueue_index] = NO_CMD;
	return aux;
}

e_UartErrors processFrame(){
 	if (waiting_ans){
		switch ( uartSM.currUartCMD ) {	/* Check last CMD sent */  
		    case CHECK_CONNECT:
		     	/* ESPERO ACK*/
				if (frame_rx[CMDID] == SEND_ACK)
				{
					if (frame_rx[LENGTH] == 1)
					{ 
						if (frame_rx[DATA0] == ACKNOWLODGE_BYTE)
						{
							waiting_ans = 0;
							bluetoothSt = 0;
							uartSM.currUartTxSTATE	= IDLETx;
							return NO_ERROR;
						}
					}
				}
				return NO_ERROR;
		    break;

		    default:
		    	return NO_ERROR;
		    break;
		}
 	}
 	else{ 
		switch ( frame_rx[CMDID] ){
			case SEND_ADC_MEASURE:
				for(uint8_t i = DATA0; i < DATA0+ frame_rx[LENGTH] ; i++){
					adcvalue[i-DATA0] = frame_rx[i];
				}
				return NO_ERROR;
			break;

	 		default:
	 			return NO_ERROR;
	 		break;
		}
 	}
}

void reset_state_machine(){
	waiting_ans = 0;
	uartSM.currUartTxSTATE	= IDLETx;
	uartSM.currUartRxSTATE	= IDLERx;
	uartSM.currUartCMD	    = NO_CMD;
	timeout 				= TIMEOUT_MS;
}

/************************************************
 *   		FUNCTIONS DEFINITION
 ***********************************************/
void checkBTstatus(){
	pushCMD(CHECK_CONNECT);
}

void setTARE(){
	pushCMD(SET_TARE);
}

void txUartStateMachine(){
	e_UartErrors error;
	uint8_t i;

	if (waiting_ans){
		if (timeout == 0){
			reset_state_machine();
			bluetoothSt = 0;
			NERRORES++;
		}
		else{
			timeout--;
		}
	}

	switch ( uartSM.currUartTxSTATE ){
	    case IDLETx:
			uartSM.currUartCMD = popCMD();		// POP FIRS IN THE LIST
	      	switch ( uartSM.currUartCMD ){	          	
	          	case CHECK_CONNECT:
	          		frame_tx[START] 	= START_BYTE; 	// START BYTE
	          		frame_tx[LENGTH] 	= 0;			// LENGTH PAYLOAD - NO DATA SENT
	          		frame_tx[CMDID] 	= CHECK_CONNECT;  	// COMAND ID
	          		frame_tx[CMDID+1]   = STOP_BYTE;	// STOP BYTE

	          		uartSM.currUartTxSTATE = SENDING_PACKET; 	
	            break;

	            case SET_TARE:
	          		frame_tx[START] 	= START_BYTE; 	// START BYTE
	          		frame_tx[LENGTH] 	= 0;			// LENGTH PAYLOAD - NO DATA SENT
	          		frame_tx[CMDID] 	= SET_TARE;  	// COMAND ID
	          		frame_tx[CMDID+1]   = STOP_BYTE;	// STOP BYTE

	          		uartSM.currUartTxSTATE = SENDING_PACKET; 	
	            break;

	          	default: 
	          		// error me estas pidiendo que mande algo que no se como mandarlo 
	          	break;
			}
	    break;
	    
	    case SENDING_PACKET:
	    	for(i = 0; i < MIN_FRAME_SIZE + frame_tx[LENGTH] ; i++)
	    		Serial.write(frame_tx[i]);
	    	timeout = TIMEOUT_MS;
	    	if (uartSM.currUartCMD == CHECK_CONNECT ){
	    		waiting_ans = true;
	    		uartSM.currUartTxSTATE = WAITING;
	    	}
	    	else{
	    		uartSM.currUartTxSTATE = IDLETx;
	    	}	
	    break;

	    case WAITING:
	    	uartSM.currUartTxSTATE = WAITING;
	    break;
	    
	    default:
	    break;
	}
}

void rxUartStateMachine(uint8_t ucReceived){
	static uint8_t nByte = 0;
		
	switch( uartSM.currUartRxSTATE ){
		case IDLERx:
			nByte = 0;
			if (ucReceived == START_BYTE){
				frame_rx[nByte++] = ucReceived;
				uartSM.currUartRxSTATE = RECEIVING_PACKET;
			}
		break;

	 	case RECEIVING_PACKET:
	 		frame_rx[nByte++] = ucReceived;		
			if (ucReceived == STOP_BYTE){ 
				if (processFrame() != NO_ERROR) 
					NERRORES++;
				uartSM.currUartRxSTATE = IDLERx;
				nByte = 0;
			}
			else if (nByte >= MAX_FRAME_SIZE){
				NERRORES++;
				uartSM.currUartRxSTATE = IDLERx;
				nByte = 0;
			}	
		break;

		default:
		break;
	}
}

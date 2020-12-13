/************************************************
 *   		INCLUDES
 ***********************************************/
#include "com_bt.h"

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


uint32_t timeout = TIMEOUT_MS; uint32_t NERRORES = 0;
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

void pushADC(AdcDataType *ptr){
	uint8_t i;
	adcbuffer[head++] = ( *ptr & 0x00FFFFFF );
	head = head % BUFFER_SIZE;
}

e_UartErrors processFrame(){
	uint8_t l = 0, i = 0;
 	uint8_t adcvalue[3] = {0,0,0};

 	if (waiting_ans){
 		waiting_ans	= false;
		switch ( uartSM.currUartCMD ) {	/* Check last CMD sent */  
			case GET_CONFIG:
				/* ESPERO SEND CONFIG */
				if (frame_rx[CMDID] == SEND_CONFIG){
			    	if (frame_rx[LENGTH] == 2){ // length 2 
			    		frec = frame_rx[DATA0];
			    		mode = frame_rx[DATA1];
			    		pushCMD(SEND_CONFIG);
			    		pushCMD(SEND_ACK);
			    		return NO_ERROR;
			    	}
			    	else{
			    		return WRONG_ANSWER;
			    	}
			    }
			    else{
			    	return WRONG_ANSWER;
			    }	
			break;   

		    case SEND_CONFIG:
		    	/* ESPERO ACK*/
		    	if (frame_rx[CMDID] == SEND_ACK){
			    	if (frame_rx[LENGTH] == 1){ // length 2 
			    		if (frame_rx[DATA0] == ACKNOWLODGE_BYTE)
			    		{
			    			return NO_ERROR;
			    		}
			    		else
			    		{
			    			return WRONG_ACK;
			    		}
			    	}
			    	else{
			    		return WRONG_ANSWER;
			    	}
			    }
			    else{
			    	return WRONG_ANSWER;
			    }
		    break;

		    default:
		    	return WRONG_ANSWER;
		    break;
		}
 	}
 	else{ 
		switch ( frame_rx[CMDID] ){
			case SEND_ADC_MEASURE:
	 			l = frame_rx[LENGTH];

	 			if (sizeof(AdcDataType) >=3){
	 				if (l != 3)
	 					return WRONG_DATA;          			
            	}
            	else if (sizeof(AdcDataType) == 2){
            		if (l == 2)
	 					return WRONG_DATA;          			
            	}
 			
				for(i = DATA0; i < DATA0+l ; i++){
					adcvalue[i-DATA0] = frame_rx[i];
				}
				pushADC((AdcDataType *)adcvalue);
				pushCMD(SEND_ACK);				
				return NO_ERROR;
	 		break;

	 		default:
	 			return WRONG_DATA;
	 		break;
		}
 	}
}

void reset_state_machine(){
	waiting_ans = 0;
	uartSM.currUartSTATE	= IDLE;
	timeout 				= TIMEOUT_MS;
}

/************************************************
 *   		FUNCTIONS DEFINITION
 ***********************************************/
void checkBTstatus(){
	pushCMD(GET_CONFIG);
}

void txUartStateMachine(){
	e_UartErrors error;
	uint8_t i;

	if (waiting_ans){
		if (timeout == 0){
			reset_state_machine();
			NERRORES++;
		}
		else{
			timeout--;
		}
	}

	switch ( uartSM.currUartSTATE ){
	    case IDLE:
			uartSM.currUartCMD = popCMD();		// POP FIRS IN THE LIST
	      	switch ( uartSM.currUartCMD ){
	        	case GET_CONFIG:
	          		frame_tx[START] 	= START_BYTE; 	// START BYTE
	          		frame_tx[LENGTH] 	= 0;			// LENGTH PAYLOAD - NO DATA SENT
	          		frame_tx[CMDID] 	= GET_CONFIG;  	// COMAND ID
	          		frame_tx[CMDID+1] 	= STOP_BYTE;	// STOP BYTE

	          		uartSM.currUartSTATE = SENDING_PACKET; 
	         	break;
	          
	          	case SEND_CONFIG:
	           		frame_tx[START] 	= START_BYTE; 	// START BYTE
	          		frame_tx[LENGTH] 	= 2;			// LENGTH PAYLOAD - FREC + MODE
	          		frame_tx[CMDID] 	= SEND_CONFIG; 	// COMAND ID
	          		frame_tx[DATA0] 	= global_val[FREC];	// PAYLOAD
	          		frame_tx[DATA1] 	= global_val[MODE];	//
	          		frame_tx[DATA1+1]	= STOP_BYTE;	// STOP BYTE

	          		uartSM.currUartSTATE = SENDING_PACKET;		
	            break;
	          	
	          	case SEND_ACK:
	          		frame_tx[START] 	= START_BYTE; 	// START BYTE
	          		frame_tx[LENGTH] 	= 1;			// LENGTH PAYLOAD - NO DATA SENT
	          		frame_tx[CMDID] 	= SEND_ACK;  	// COMAND ID
	          		frame_tx[DATA0]		= ACKNOWLODGE_BYTE;
	          		frame_tx[DATA0+1]	= STOP_BYTE;	// STOP BYTE

	          		uartSM.currUartSTATE = SENDING_PACKET; 	
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
	    	if (uartSM.currUartCMD == SEND_ACK ){
	    		uartSM.currUartSTATE = IDLE;
	    	}
	    	else{
	    		waiting_ans = true;
	    		uartSM.currUartSTATE = IDLE_WAITING;
	    	}	
	    break;
	    
	    case PROCESSING_PACKET:
	    	error = processFrame();
			if (error != NO_ERROR) NERRORES++;
			uartSM.currUartSTATE = IDLE;	
	    break;

	    default:
	    break;
	}
}

void rxUartStateMachine(uint8_t ucReceived){
	static uint8_t nByte = 0;
		
	switch( uartSM.currUartSTATE ){
		case IDLE:
		case IDLE_WAITING:
			nByte = 0;
			if (ucReceived == START_BYTE){
				frame_rx[nByte++] = ucReceived;
				uartSM.currUartSTATE = RECEIVING_PACKET;
			}
		break;

	 	case RECEIVING_PACKET:
	 		frame_rx[nByte++] = ucReceived;		
			if (ucReceived == STOP_BYTE){ 
				uartSM.currUartSTATE = PROCESSING_PACKET;
				nByte = 0;
			}
			else if (nByte >= MAX_FRAME_SIZE){
				NERRORES++;
				uartSM.currUartSTATE = IDLE;
				nByte = 0;
			}	
		break;

		default:
		break;
	}
}

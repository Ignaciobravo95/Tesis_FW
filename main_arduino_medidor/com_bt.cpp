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

AdcDataType adc_value = 33222;

uint32_t timeout = TIMEOUT_MS;	uint32_t NERRORES = 0;
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
 		waiting_ans	= false;
		switch ( uartSM.currUartCMD ) {	 	/* Check last CMD sent */  
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

		    case SEND_ADC_MEASURE:
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
		    break;
		}
 	}
 	else{
 		switch ( frame_rx[CMDID] ){
 			case GET_CONFIG:
 				if (frame_rx[LENGTH] == 0){
 					pushCMD(SEND_CONFIG);
 				}
 				else{
 					return WRONG_DATA;
 				}
 			break;

 			case SEND_CONFIG:
 				if (frame_rx[LENGTH] == 2){
 					value_frec = frame_rx[DATA0];
 					value_mode = frame_rx[DATA1];
 					pushCMD(SEND_ACK);
 				}
 				else{
 					return WRONG_DATA;
 				}
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
void sendADCValue(){
	pushCMD(SEND_ADC_MEASURE);
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
	          	case SEND_CONFIG:
	           		frame_tx[START] 	= START_BYTE; 	// START BYTE
	          		frame_tx[LENGTH] 	= 2;			// LENGTH PAYLOAD - FREC + MODE
	          		frame_tx[CMDID] 	= SEND_CONFIG; 	// COMAND ID
	          		frame_tx[DATA0] 	= value_frec;	// PAYLOAD
	          		frame_tx[DATA1] 	= value_mode;	//
	          		frame_tx[DATA1+1]	= STOP_BYTE;	// STOP BYTE

	          		uartSM.currUartSTATE = SENDING_PACKET; 
	            break;
	          	
	          	case SEND_ADC_MEASURE:
	            	frame_tx[START] 	= START_BYTE; 	// START BYTE
	            	frame_tx[CMDID] 	= SEND_ADC_MEASURE;  // COMAND ID
	            	if (sizeof(AdcDataType) >=3){
	            		frame_tx[LENGTH] 	= 3;			// LENGTH PAYLOAD - 24 BITS ADC
	            		frame_tx[DATA0] 	= adc_value 	   	& 0x000000FF;
	          			frame_tx[DATA1] 	= (adc_value >> 8)  & 0x000000FF;
	          			frame_tx[DATA2] 	= (adc_value >> 16) & 0x000000FF;
	          			frame_tx[DATA2+1]	= STOP_BYTE;		
	            	}
	            	else if (sizeof(AdcDataType) == 2){
	            		frame_tx[LENGTH] 	= 2;			// LENGTH PAYLOAD - 16 BITS ADC
	            		frame_tx[DATA0] 	= adc_value 	   	& 0x00FF;
	          			frame_tx[DATA1] 	= (adc_value >> 8)  & 0x00FF;
	          			frame_tx[DATA1+1]	= STOP_BYTE;		
	            	}	          		

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

/*

FIRMWARE:

- LIBRERIA PARA LA TARJETA SD:
			- RECIBIR UN BUFFER , FORMATEARLO Y ESCUPIRLO EN UN TXT. --------> IVAN

- LIBRERIA PARA LA EEPROM:
			- RECIBIR DATOS, LO ESCRIBE EN EPPROM.				     --------> NACHO		
			- RECUPERA DATOS DE LA EEPROM.

- DEFINIR QUE HACEMOS CON LOS ERRORES DEL COM BT Y LOS DATOS.		

- LECTURA DE LA BATERIA:
			- LEE EL VOLTAJE C/ X TIEMPO.

HARDWARE:

- DEFINIR CIRCUITO CARGA BATERIA:
			- SE HACE EN PLACA SEPARADA?							-----------> EZE
			- SE PUEDE CARGAR MIENTRAS SE USA?

- DEFINIR COMO MEDIMOS LA BATERIA:
			- VER EL CIRCUITO , SWITCH C/ X TIEMPO.					-----------> EZE

- TODO:

	- COMPARTIR REPO CON EL EZE
	- SUBO AL REPO EL PROGRAMA CON EL BT.
	- SUBO AL DRIVE LOS ALTIUM Y LOS PROTEUS

*/


/*

- BATERIA (2.8V hasta 4.2V ) -> PROTECCION (SUBVOLTAJE, CORTO) -> CARGADOR -> CARGADOR
															   -> STEP UP 5V - >LOAD ( ARDUINOS )  

CIRCUITO STEP UP
- TENSION :   STEP  OUT = 5V - IN = 2.8 A 4.2V
- CORRIENTE : MAX(CIRCUITO BASE) BT + ENC + PANTALLA + SD + PASIVOS + MICRO


OPCION A) PLACA GREY SCOTT . YA TIENE STEP UP PROTECCION Y CARGADOR
OPCION B) COMPRAR PLACA CARGADOR Y COMPRAR PLACA STEP UP Y UNIRLOS

*/
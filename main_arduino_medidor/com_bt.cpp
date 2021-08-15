/************************************************
 *   		INCLUDES
 ***********************************************/
#include "com_bt.h"
#include "eepromlib.h"

/************************************************
 *			GLOBAL VARIABLE SECTION
 ***********************************************/
t_UartStateMachine 	uartSM;
e_UartCmdId		CMDqueue[2] = {NO_CMD, NO_CMD};
uint8_t 		CMDqueue_index = 0;

uint8_t frame_tx[MAX_FRAME_SIZE] = {0};
uint8_t frame_rx[MAX_FRAME_SIZE] = {0};
uint8_t waiting_ans	= false;

AdcDataType adc_value = 31200;

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
		switch ( uartSM.currUartCMD ) {	/* Check last CMD sent */  
		    default:
		    	return NO_ERROR;
		    break;
		}
 	}
 	else{ 
		switch ( frame_rx[CMDID] ){
			case CHECK_CONNECT:
				/* ESPERO ACK*/
				if (frame_rx[LENGTH] == 0)
				{ 
					pushCMD(SEND_ACK);
					bluetoothSt = 1;
					return NO_ERROR;
				}
			break;

			case SET_TARE:
				/* RESETEO LA TARA */
				if (frame_rx[LENGTH] == 0)
				{ 
					LoadCell.tare();  //El peso actual es considerado Tara.
					return NO_ERROR;
				}
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
void sendADCValue(){
	pushCMD(SEND_ADC_MEASURE);
	float tmp;
  	tmp = LoadCell.get_value(1);
	adc_value = (tmp > 0) ? (uint32_t)tmp : 0;
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

	switch ( uartSM.currUartTxSTATE ){
	    case IDLETx:
			uartSM.currUartCMD = popCMD();		// POP FIRS IN THE LIST
	      	switch ( uartSM.currUartCMD ){	          	
	          	case SEND_ACK:
	          		frame_tx[START] 	= START_BYTE; 	// START BYTE
	          		frame_tx[LENGTH] 	= 1;			// LENGTH PAYLOAD - NO DATA SENT
	          		frame_tx[CMDID] 	= SEND_ACK;  	// COMAND ID
	          		frame_tx[DATA0]		= ACKNOWLODGE_BYTE;
	          		frame_tx[DATA0+1]	= STOP_BYTE;	// STOP BYTE

	          		uartSM.currUartTxSTATE = SENDING_PACKET; 	
	            break;

				case SEND_ADC_MEASURE:
					frame_tx[START]         = START_BYTE;   // START BYTE
					frame_tx[CMDID]         = SEND_ADC_MEASURE;  // COMAND ID
					if (sizeof(AdcDataType) >=3){
						frame_tx[LENGTH]        = 3;                    // LENGTH PAYLOAD - 24 BITS ADC
						frame_tx[DATA0]         = adc_value         & 0x000000FF;
						frame_tx[DATA1]         = (adc_value >> 8)  & 0x000000FF;
						frame_tx[DATA2]         = (adc_value >> 16) & 0x000000FF;
						frame_tx[DATA2+1]       = STOP_BYTE;
					}
					else if (sizeof(AdcDataType) == 2){
						frame_tx[LENGTH]        = 2;                    // LENGTH PAYLOAD - 16 BITS ADC
						frame_tx[DATA0]         = adc_value         & 0x00FF;
						frame_tx[DATA1]         = (adc_value >> 8)  & 0x00FF;
						frame_tx[DATA1+1]       = STOP_BYTE;
					}
					
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
	    	waiting_ans = false;
	    	uartSM.currUartTxSTATE = IDLETx;
	    	if (uartSM.currUartCMD==SEND_ACK)
	    		bluetoothSt = 0;
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

/*

FIRMWARE:

- LIBRERIA PARA LA TARJETA SD:
			- RECIBIR UN BUFFER , FORMATEARLO Y ESCUPIRLO EN UN TXT. --------> IVAN

- DEFINIR QUE HACEMOS CON LOS ERRORES DEL COM BT Y LOS DATOS.		

- LECTURA DE LA BATERIA:
			- LEE EL VOLTAJE C/ X TIEMPO.

HARDWARE:

- DEFINIR CIRCUITO CARGA BATERIA:
			- SE HACE EN PLACA SEPARADA?							-----------> EZE
			- SE PUEDE CARGAR MIENTRAS SE USA?

- DEFINIR COMO MEDIMOS LA BATERIA:
			- VER EL CIRCUITO , SWITCH C/ X TIEMPO.					-----------> EZE

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

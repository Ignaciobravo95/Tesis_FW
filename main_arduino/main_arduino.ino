/************************************************
 *  		   INCLUDES SECTION
 ***********************************************/
#include "menu.h"
#include "com_bt.h"
#include "eepromlib.h"
#include "extra.h"
#include <math.h>
#include <TimerOne.h>

/************************************************
 *   		DEFINES SECTION
 ***********************************************/
#define pinSW 	21  // CLK del encoder
#define pinDT  	20  // DT  del encoder
#define pinCLK  19  // DT  del encoder
#define pinLED1 13 // CPU state
#define TIMER0_PERIOD		  	1000	 	/*  = 1ms */	
#define PERIODIC_TASK1_PERIOD 	5000000 	/*  = 10s	Check battery and bt	*/
#define PERIODIC_TASK2_PERIOD 	250000   	/*  = .25s 	Blinking items 			*/
#define PERIODIC_TASK3_PERIOD 	15000000   	/*  = 15s 	Writes items 			*/


/************************************************
 *   		FUNCTIONS DECLARATION
 ***********************************************/
void eeprom_write();
void clearFlagRecordingSD();
void setFlagRecordingSD();

/************************************************
 *   		GLOBAL DATA SECTION
 ***********************************************/
/* INTERRUPT FLAGS */
volatile bool flag_butt_pressed = false;
volatile bool flag_encoder = false;
volatile bool up = false;
volatile bool flag_periodic_task1 = false;
volatile bool flag_periodic_task2 = false;
volatile bool flag_periodic_task3 = false;
volatile bool flag_periodic_task4 = false;
volatile bool flag_timer0		  = false;

/* AUX VARIABLES */
volatile uint8_t lastEncoderState = 0;
volatile uint32_t count = 0; 
uint32_t PERIODIC_TASK4_PERIOD = 0;
uint32_t task4_count_value = 0,task3_count_value = 0, task2_count_value = 0, task1_count_value = 0;
uint16_t upperlimit = 0;
uint8_t recordingSD = 0;

/* GLOBAL */
extern uint32_t global_val[5];
extern AdcDataType adcbuffer[BUFFER_SIZE];

/* MENU ITEMS */
menu_t PPAL, VIS, PAC, CONF, CAL1, CAL2, CAL3, CAL4;
menu_t *currMenu;
menuItem_t *currItem;

void setup(void){
	/* READ CONFIG FROM EEPROM */
	global_val[BUJIA] 	= getEEPROM_bujia();
	global_val[TH] 		= getEEPROM_th();
	global_val[FREC] 	= getEEPROM_frec();
	global_val[MODE] 	= getEEPROM_mode();
	global_val[IDPAC] 	= getEEPROM_id();

	/* TIMER INIT */
	Timer1.initialize(TIMER0_PERIOD); /* Value in microseconds? */

	/* TIMER TASK CALC */
	PERIODIC_TASK4_PERIOD = (uint32_t)(1000000/global_val[FREC]);
	task4_count_value = PERIODIC_TASK4_PERIOD / TIMER0_PERIOD; 	
	task3_count_value = PERIODIC_TASK3_PERIOD / TIMER0_PERIOD; 	
	task2_count_value = PERIODIC_TASK2_PERIOD / TIMER0_PERIOD;
	task1_count_value = PERIODIC_TASK1_PERIOD / TIMER0_PERIOD;
  	
	/* INIT SERIAL COMMUNICATIONS */
	Serial.begin(9600);
	//Serial.println("Starting system...");

  	/* GPIO INIT */
	pinMode(pinSW	,INPUT);
	pinMode(pinCLK 	,INPUT);
	pinMode(pinDT 	,INPUT);
	// pinMode(pinLED1 ,OUTPUT);
	// digitalWrite(pinLED1, HIGH);	

	/* ATTACH INTERRUPTS */
	attachInterrupt(digitalPinToInterrupt(pinSW),ISR_BUTT_PRESSED,RISING);
	attachInterrupt(digitalPinToInterrupt(pinCLK),ISR_A_CHANGE,FALLING);	
	Timer1.attachInterrupt(ISR_TIMER_EXPIRED); 

	/* INITIALIZE MENU */
	PPAL.display_header   	= &menu_principal_header;
	PPAL.display_option   	= &menu_principal_option;
	PPAL.item[0].nextMenu 	= &VIS;		// CREA UN BOTON QUE CUANDO LO APRETE PASE A VISUALIZACION
	PPAL.item[1].nextMenu 	= &PAC;
	PPAL.item[2].nextMenu 	= &CONF;
	PPAL.item[3].nextMenu 	= &CAL1;
	PPAL.items_number		= 4;

	VIS.display_header				= &menu_visualizacion_header;
	VIS.display_option				= &menu_visualizacion_option;
	VIS.display_fields 				= &menu_visualizacion_fields;
	VIS.item[0].nextMenu 			= NULL;
	VIS.item[1].nextMenu 			= NULL;
	VIS.item[2].nextMenu 			= &PPAL;
	VIS.item[3].nextMenu 			= NULL;
	VIS.items_number				= 4;
	VIS.item[3].blinking_function 	= &blinkBUJIA;
	VIS.item[3].value_id 	     	= BUJIA;
	VIS.item[3].value_limit	      	= 13;
	VIS.item[3].doAction		 	= &eeprom_write;
	VIS.item[0].doAction		  	= &setFlagRecordingSD;
	VIS.item[1].doAction		  	= &clearFlagRecordingSD;

	PAC.display_header				= &menu_pacientes_header;
	PAC.display_option				= &menu_pacientes_option;
	PAC.display_fields				= &menu_pacientes_fields;
	PAC.item[0].nextMenu 			= NULL;
	PAC.item[1].nextMenu 			= &PPAL;
	PAC.item[2].nextMenu 			= &PPAL;
	PAC.items_number				= 3;
	PAC.item[0].blinking_function	= &blinkIDPACIENTE;
	PAC.item[0].value_id 			= IDPAC;
	PAC.item[0].value_limit			= 40001;
	PAC.item[1].doAction 			= &eeprom_write;
	
	CONF.display_header				= &menu_configuracion_header;
	CONF.display_option 			= &menu_configuracion_option;
	CONF.display_fields 			= &menu_configuracion_fields;
	CONF.item[0].nextMenu 			= NULL;
	CONF.item[1].nextMenu 			= NULL;
	CONF.item[2].nextMenu 			= NULL;
	CONF.item[3].nextMenu 			= &PPAL;
	CONF.item[4].nextMenu 			= &PPAL;
	CONF.item[0].blinking_function 	= &blinkTH;
	CONF.item[1].blinking_function 	= &blinkFREQ;
	CONF.item[2].blinking_function 	= &blinkMODO;
	CONF.item[0].value_id			= TH;
	CONF.item[1].value_id			= FREC;
	CONF.item[2].value_id			= MODE;
	CONF.item[0].value_limit 		= 100;
	CONF.item[1].value_limit 		= 11;
	CONF.item[2].value_limit 		= 2;
	CONF.item[3].doAction 			= &eeprom_write;
	CONF.items_number				= 5;

	CAL1.display_header				= &menu_calibracion_step1_header;
	CAL1.display_option				= &menu_calibracion_option;
	CAL1.item[0].nextMenu 			= &CAL2;
	CAL1.item[1].nextMenu 			= &PPAL;
	CAL1.items_number				= 2;

	CAL2.display_header				= &menu_calibracion_step2_header;
	CAL2.display_option				= &menu_calibracion_option;
	CAL2.item[0].nextMenu 			= &CAL3;
	CAL2.item[1].nextMenu 			= &PPAL;
	CAL2.items_number				= 2;

	CAL3.display_header				= &menu_calibracion_step3_header;
	CAL3.display_option				= &menu_calibracion_option;
	CAL3.item[0].nextMenu 			= &CAL4;
	CAL3.item[1].nextMenu 			= &PPAL;
	CAL3.items_number				= 2;

	CAL4.display_header				= &menu_calibracion_step4_header;
	CAL4.display_option				= &menu_calibracion_option;
	CAL4.item[0].nextMenu 			= &PPAL;
	CAL4.item[1].nextMenu 			= &PPAL;
	CAL4.items_number				= 2;

	currMenu = &PPAL;
	currItem = &currMenu->item[0];
	/* INITIALIZE TFT DISPLAY */
	init_tft();	
	/* READ INITIAL ENCODER STATE */
    lastEncoderState = (digitalRead(pinCLK) << 1 | digitalRead(pinDT));

	/* DISPLAY START MENU */
	currMenu->display_header();
	currMenu->display_option(0);
	upperlimit = currMenu->items_number;
	
	batteryStatus(0);
	bluetoothStatus(0);	
}

void loop(void){
	uint8_t i = 0;						// var used to iterate trough arrays
	static uint8_t index = 0;			// selected item number 
	static uint8_t digit = 0; 			// digit being edited
	static uint8_t nDigit = 1;			// Number of digits 
	static uint16_t encoder_count = 0;	// Rotative encoder counter to switch between items or edit values
	static uint16_t encoder_mode = 0;  	// Encoder mode - 0 switch items - 1 modifie value
	
	/* Main LOOP */
	/* BUTTON PRESSED EVENT */
	if (flag_butt_pressed){
		// digitalWrite(pinLED1, HIGH);
		/******************************
		IF APPLICABLE TO  SELECTED ITEM 
		- PERFORM ACTION (SAVE, TURN ON, DISP MSG, ETC)
		- CHANGE MENU 
		- CHANGE ENCODER FUNCTION (SELECTOR, COUNTER)
		- ENABLE BLINKING
		******************************/	
		if (currItem -> doAction != NULL ) {
			currItem -> doAction();	
		}
		if (currItem -> nextMenu != NULL ){
			if (currMenu == &VIS)
				menu_visualizacion_signal(0, true);
			clearFlagRecordingSD();
			index = 0;
			currMenu = currItem -> nextMenu;
			currItem = &currMenu -> item[index];
			for ( i = 0; i < currMenu -> items_number ; i++ ) currMenu->item[i].tmp_value = global_val[currMenu->item[i].value_id];
			currMenu -> display_header();
			currMenu -> display_option(index);
			if (currMenu -> display_fields != NULL ) currMenu -> display_fields();	
			batteryStatus(0);
			bluetoothStatus(0);
			/* SET ENCODER UPPER LIMIT */
			upperlimit = currMenu -> items_number;
			encoder_count = index;
		}
		else if (currItem -> blinking_function != NULL ){
			/* IF IM NOT EDITING VALUE START EDITING*/
			if ( currItem -> blinking == 0){
				encoder_mode = 1;
				currItem -> blinking = 1;
				nDigit = checknDigits( currItem -> value_limit - 1);     // numero de digitos		
				encoder_count = getDigitValue(currItem -> tmp_value, 0); // valor del primer digito
				upperlimit = getDigitMax(currItem -> value_limit - 1, currItem ->tmp_value, nDigit, digit);	
			}
			/* IF IM EDITING CHECK WICH DIGIT IM EDITING */
			else if ( currItem -> blinking == 1){
				encoder_count = getDigitValue(currItem -> tmp_value, ++digit); 
				upperlimit = getDigitMax(currItem -> value_limit - 1, currItem ->tmp_value, nDigit, digit);	
				/* IF I WENT OVER ALL DIGITS, STOP EDITING */
				if (digit >= nDigit){
					currItem -> blinking_function(currItem -> tmp_value, true, 0);
					digit = 0;
					currItem -> blinking = 0;
					encoder_mode = 0;
					encoder_count = index;
					upperlimit = currMenu -> items_number;			
				}
			}			
		}		
		/******************************/
		//Serial.println("EVENT: button_pressed");
		flag_butt_pressed = false;
	}

	/* ENCODER ROTATING EVENT */
	if (flag_encoder){
		// digitalWrite(pinLED1, HIGH);
		/******************************
		- CHANGE SELECTED OPTION OR
		CHANGE SELECTED FIELD VALUE
		******************************/
		if (up){
        encoder_count = (encoder_count + 1) % upperlimit; // 0 - (upperlimit - 1)
	    }
	    else{
	        if (encoder_count == 0) encoder_count = upperlimit - 1;
	        else encoder_count = (encoder_count - 1) % upperlimit;
	    }

		if (!encoder_mode){
			index = encoder_count;
			currItem = &currMenu-> item[index];
			currMenu->display_option(index);
		}else{
			currItem -> tmp_value = modValueDigit(currItem -> tmp_value, digit, encoder_count);				
		}
		/******************************/
		//Serial.println("EVENT: encoder_rotated");
		flag_encoder = false;
	}

	/* PERIODIC TASK 1 EVENT */
	if ((flag_periodic_task1) && (!flag_periodic_task2) && (!flag_periodic_task3) && (!flag_periodic_task4)){
		// digitalWrite(pinLED1, HIGH);
		/******************************
		- CHECK PERIODIC TASK W/LESS PRIORITY:
			- BATTERY VOLTAGE  
			- BLUETOOTH STATUS 
		******************************/
		checkBTstatus(); 
		batteryStatus(0);
		bluetoothStatus(0);
		/******************************/
		//Serial.println("EVENT: PERIODIC TASK 1.");
		flag_periodic_task1 = false;
	}

	/* PERIODIC TASK 2 EVENT */
	if ((flag_periodic_task2) && (!flag_periodic_task3) && (!flag_periodic_task4)){
		// digitalWrite(pinLED1, HIGH);
		/******************************
		- BLINK IF ANY BLINKING ITEMS 
		******************************/
		if (currItem -> blinking) currItem -> blinking_function(currItem->tmp_value, false, digit);
		if (recordingSD) blinkREC(false);
		/******************************/
		//Serial.println("EVENT: PERIODIC TASK 2.");
		flag_periodic_task2 = false;
	}

	/* PERIODIC TASK 3 EVENT */
	if((flag_periodic_task3)  && (!flag_periodic_task4)) {
		// digitalWrite(pinLED1, HIGH);
		/******************************
		- WRITES DATA IN SD CARD
		******************************/	
		if (recordingSD){
			//Serial.print("RECORDING DATA TO SD. ");
		}
		/******************************/	
		//Serial.println("EVENT: PERIODIC TASK 3.");
		flag_periodic_task3 = false;
	}
 	
 	/* PERIODIC TASK 4 EVENT */
	if(flag_periodic_task4){
		flag_periodic_task4 = false;
	}

	/* SERIAL DATA FROM BT EVENT */
	if (Serial.available() > 0){
	 	// digitalWrite(pinLED1, HIGH);
		/******************************
		- PROCCESS RECEIVED DATA FROM
		END DEVICE.
		******************************/
		rxUartStateMachine(Serial.read());
		/******************************/
	}

	/* EVALUATE TX UART STATE MACHINE EVERY TIMER0 PERIOD */
	if (flag_timer0){
		/******************************
		- CHECKS IF THERE'S A REQUEST TO SEND DATA
		AND CHECKS IF THE TIMEOUT EXPIRED
		******************************/
		txUartStateMachine();
		/******************************/
		flag_timer0 = false;
	}

	// digitalWrite(pinLED1, LOW);
	/* SEND TO SLEEP */
}					


/************************************************
 *  		ISR
 ***********************************************/


// Funcion para la deteccion del pulsador del encoder (Interrupcion)
void ISR_BUTT_PRESSED(){

	flag_butt_pressed = true;
}

// Funcion para la deteccion de giro del encoder (Interrupcion)
void ISR_A_CHANGE(){
	// uint8_t currEncoderState = (digitalRead(pinCLK) << 1 | digitalRead(pinDT));
   	


 //   	if (currEncoderState > lastEncoderState)
 //   		up = true;
 //   	else
 //   		up = false;

 //   	if ((currEncoderState == 0) && (lastEncoderState == 3))
 //   		up = true;

 //   	if ((currEncoderState == 3) && (lastEncoderState == 0))
 //   		up = false;

 //    lastEncoderState = currEncoderState;
    
    up = (digitalRead(pinCLK) == digitalRead(pinDT)); 
    flag_encoder = true;
}

void ISR_TIMER_EXPIRED(){
	count++;
	if (count % task1_count_value == 0){
		flag_periodic_task1 = true;
	}
	if (count % task2_count_value == 0){
		flag_periodic_task2 = true;
	}
	if (count % task3_count_value == 0){
		flag_periodic_task3 = true;
	}
	if (count % task4_count_value == 0){
		flag_periodic_task4 = true;
	}
	flag_timer0 = true;
}


/************************************************
 *  		EXTRA
 ***********************************************/

// Esta funcion tendria que ir en una LIBRERIA
void eeprom_write(){
	uint8_t i = 0, imax =  currMenu -> items_number;
	for ( i = 0; i < imax; i++ ){
		switch (i){
			case 0:
				if (currMenu == &PAC){
					global_val[IDPAC] = currMenu -> item[i].tmp_value;
					writeEEPROM_id(global_val[IDPAC]);
				}
				else if (currMenu == &CONF){
					global_val[TH] = currMenu -> item[i].tmp_value;
					writeEEPROM_th(global_val[TH]);
				}
			break;
			
			case 1:
				if (currMenu == &CONF){
					global_val[FREC] = currMenu -> item[i].tmp_value;
					writeEEPROM_frec(global_val[FREC]);
					PERIODIC_TASK4_PERIOD = (uint32_t)(1000000/global_val[FREC]);
					task4_count_value = PERIODIC_TASK4_PERIOD / TIMER0_PERIOD; 	
				}
			break;

			case 2:
				if (currMenu == &CONF){
					global_val[MODE] = currMenu -> item[i].tmp_value;
					writeEEPROM_mode(global_val[MODE]);
				} 
			break;

			case 3:
				if (currMenu == &VIS){
					global_val[BUJIA] = currMenu -> item[i].tmp_value;
					writeEEPROM_bujia(global_val[BUJIA]);
				}
		}
		global_val[currMenu -> item[i].value_id] = currMenu -> item[i].tmp_value;
	}
}

// Setea la bandera para grabar en la SD
void setFlagRecordingSD(){

	recordingSD = 1;
}

// Setea la bandera dejar de grabar en la SD
void clearFlagRecordingSD(){
	recordingSD = 0;
	blinkREC(true);
}


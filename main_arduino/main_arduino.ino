/************************************************
 *  		   INCLUDES SECTION
 ***********************************************/
#include "menu.h"  
#include <TimerOne.h>

/************************************************
 *   		DEFINES SECTION
 ***********************************************/
#define pinSW 	2  // CLK del encoder
#define pinDT  	3  // DT  del encoder
#define pinCLK  12  // DT  del encoder
#define pinLED1 13 // CPU state
#define TIMER0_PERIOD		  	1000	 /* us */	
#define PERIODIC_TASK1_PERIOD 	10000000 /*  = 10s*/
#define PERIODIC_TASK2_PERIOD 	250000   /*  = 0.25S */

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

/* AUX VARIABLES */
volatile uint8_t lastEncoderState = 0;
volatile uint32_t count = 0; 
uint32_t task3_count_value = 0, task2_count_value = 0, task1_count_value = 0;
uint8_t index = 0; 

uint16_t encoder_count = 0, upperlimit = 0, encoder_mode = 0;
uint8_t recordingSD = 0;

/* MENU ITEMS */
menu_t PPAL, VIS, PAC, CONF;
menu_t *currMenu;
menuItem_t *currItem;

/* EEPROM VALUES */					/* DYNAMIC RANGE */
uint16_t value_paciente = 0;		/* DR: 0 - 65536 */
uint8_t  value_bujia = 0;			/* DR: 0 - 10*/
uint8_t  value_th = 0;				/* DR: 0 - 999 */
uint8_t  value_frec = 0;			/* DR: 0 - 10 */
uint8_t  value_mode = 0;			/* DR: 0 - 1 */

void setup(void){
	/* READ CONFIG FROM EEPROM */
	value_bujia = 8;
	value_th = 99;
	value_frec = 2;
	value_mode = 0;
	value_paciente = 33223;

	/* TIMER INIT */
	Timer1.initialize(TIMER0_PERIOD); /* Value in microseconds? */

	/* TIMER TASK CALC */
	task3_count_value = (1000000/ value_frec) / TIMER0_PERIOD; 	
	task2_count_value = PERIODIC_TASK2_PERIOD / TIMER0_PERIOD;
	task1_count_value = PERIODIC_TASK1_PERIOD / TIMER0_PERIOD;

  	
	/* INIT SERIAL COMMUNICATIONS */
	Serial.begin(9600);
	Serial.println("Starting system.");
	Serial.print("count1 : ");
	Serial.println(task1_count_value);
	Serial.print("count2 : ");
	Serial.println(task2_count_value);
	Serial.print("count3 : ");
	Serial.println(task3_count_value);

  	/* GPIO INIT */
	pinMode(pinSW	,INPUT);
	pinMode(pinCLK 	,INPUT);
	pinMode(pinDT 	,INPUT);
	pinMode(pinLED1 ,OUTPUT);
	digitalWrite(pinLED1, HIGH);	

	/* ATTACH INTERRUPTS */
	attachInterrupt(0,ISR_BUTT_PRESSED,RISING);
	attachInterrupt(1,ISR_A_CHANGE,CHANGE);	
	Timer1.attachInterrupt(ISR_TIMER_EXPIRED); 

	/* INITIALIZE MENU */
	PPAL.display_header   	= &menu_principal_header;
	PPAL.display_option   	= &menu_principal_option;
	PPAL.item[0].nextMenu 	= &VIS;
	PPAL.item[1].nextMenu 	= &PAC;
	PPAL.item[2].nextMenu 	= &CONF;
	PPAL.items_number		= 3;

	VIS.display_header				= &menu_visualizacion_header;
	VIS.display_option				= &menu_visualizacion_option;
	VIS.display_fields 				= &menu_visualizacion_fields;
	VIS.item[0].nextMenu 			= NULL;
	VIS.item[1].nextMenu 			= NULL;
	VIS.item[2].nextMenu 			= &PPAL;
	VIS.item[3].nextMenu 			= NULL;
	VIS.items_number				= 4;
	VIS.item[3].blinking_function 	= &blinkBUJIA;
	VIS.item[3].curr_value 	     	= value_bujia;
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
	PAC.item[0].curr_value 			= value_paciente;
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
	CONF.item[0].curr_value			= value_th;
	CONF.item[1].curr_value			= value_frec;
	CONF.item[2].curr_value			= value_mode;
	CONF.item[0].value_limit 		= 100;
	CONF.item[1].value_limit 		= 11;
	CONF.item[2].value_limit 		= 2;
	CONF.item[3].doAction 			= &eeprom_write;
	CONF.items_number				= 5;

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
	uint8_t i = 0;
	uint8_t press = 10;
	/* Main LOOP */
	/* BUTTON PRESSED EVENT */
	if (flag_butt_pressed){
		digitalWrite(pinLED1, HIGH);
		/******************************
		IF APPLICABLE TO  SELECTED ITEM 
		- PERFORM ACTION (SAVE, TURN ON, DISP MSG, ETC)
		- CHANGE MENU 
		- CHANGE ENCODER FUNCTION (SELECTOR, COUNTER)
		- ENABLE BLINKING
		******************************/	
		if (currItem -> doAction != NULL ) {
			if ( (currMenu == &VIS) && (currItem == &VIS.item[2] ) && (currItem -> blinking == 0));
			else currItem -> doAction();	
		}
		if (currItem -> nextMenu != NULL ){
			menu_visualizacion_signal(0, true);
			clearFlagRecordingSD();
			index = 0;
			currMenu = currItem -> nextMenu;
			currItem = &currMenu -> item[index];
			for ( i = 0; i < currMenu -> items_number ; i++ ){
				currMenu->item[i].tmp_value = currMenu->item[i].curr_value;			
			}
			currMenu -> display_header();
			currMenu -> display_option(index);
			if (currMenu -> display_fields != NULL ){
				if (currMenu == &VIS) 		currMenu -> display_fields(0, value_bujia, value_th);		
				else if (currMenu == &PAC)	currMenu -> display_fields(value_paciente, 0, 0);		
				else if (currMenu == &CONF)	currMenu -> display_fields(value_th, value_frec, value_mode);
			}			 				
			batteryStatus(0);
			bluetoothStatus(0);
			/* SET ENCODER UPPER LIMIT */
			upperlimit = currMenu -> items_number;
			encoder_count = index;
		}
		else if (currItem -> blinking_function != NULL ){
			if ( currItem -> blinking ){
				currItem -> blinking_function(encoder_count, true);
				currItem -> blinking = 0;
				encoder_mode = 0;
				encoder_count = index;
				upperlimit = currMenu -> items_number;
			}
			else{
				encoder_mode = 1;
				currItem -> blinking = 1;
				upperlimit = currItem -> value_limit;
				encoder_count = currItem -> tmp_value;
			}
		}		
		/******************************/
		Serial.println("button_pressed");
		flag_butt_pressed = false;
	}

	/* ENCODER ROTATING */
	if (flag_encoder){
		digitalWrite(pinLED1, HIGH);
		/******************************
		- CHANGE SELECTED OPTION OR
		CHANGE SELECTED FIELD VALUE
		******************************/
		if (up){
        encoder_count = (encoder_count + 1) % upperlimit;
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
			currItem -> tmp_value = encoder_count;				
		}
		/******************************/
		Serial.println("encoder_rotated");
		flag_encoder = false;
	}

	/* PERIODIC TASK 1 */
	if ((flag_periodic_task1) && (!flag_periodic_task2) && (!flag_periodic_task3) ){
		digitalWrite(pinLED1, HIGH);
		/******************************
		- CHECK PERIODIC TASK:
			- BATTERY VOLTAGE  
			- BLUETOOTH STATUS 
		******************************/
		batteryStatus(0);
		bluetoothStatus(0);
		/******************************/
		Serial.println("UNO 1.");
		flag_periodic_task1 = false;
	}

	/* PERIODIC TASK 2 */
	if ((flag_periodic_task2) && (!flag_periodic_task3)){
		digitalWrite(pinLED1, HIGH);
		/******************************
		- BLINK IF ANY BLINKING ITEMS 
		******************************/
		if (currItem -> blinking) currItem -> blinking_function(encoder_count, false);
		if (recordingSD) blinkREC(false);
		/******************************/
		Serial.println("DOS 2.");
		flag_periodic_task2 = false;
	}

	/* PERIODIC TASK 3 */
	if(flag_periodic_task3){
		digitalWrite(pinLED1, HIGH);
		/******************************
		- WRITES DATA IN SD CARD
		******************************/	
		if (recordingSD){
			Serial.print("RECORDING DATA TO SD. ");
		}
		/******************************/	
		Serial.println("TRES 3.");
		flag_periodic_task3 = false;
	}

	/* SERIAL DATA FROM BT */
	if (Serial.available() > 0) {
		digitalWrite(pinLED1, HIGH);
		/******************************
		- PARSE DATA RECEIVED FROM
		END DEVICE.
			- BATTERY LEVEL 
			- PRESSURE LEVEL
		******************************/	
		if (currMenu == &VIS){
			uint8_t ucData =  Serial.read();
			menu_visualizacion_signal(ucData, false);
		}	
		/******************************/
	}


	digitalWrite(pinLED1, LOW);

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
	uint8_t currEncoderState = (digitalRead(pinCLK) << 1 | digitalRead(pinDT));
   	 	
   	if (currEncoderState > lastEncoderState)
   		up = true;
   	else
   		up = false;

   	if ((currEncoderState == 0) && (lastEncoderState == 3))
   		up = true;

   	if ((currEncoderState == 3) && (lastEncoderState == 0))
   		up = false;

    lastEncoderState = currEncoderState;
    flag_encoder = true;
}

// Funcion del overflow del temporizador, every 1ms (Interrupcion)
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
				if (currMenu == &PAC)
					value_paciente = currMenu -> item[i].tmp_value;
				else if (currMenu == &CONF)
					value_th = currMenu -> item[i].tmp_value;
			break;
			
			case 1:
				if (currMenu == &CONF){
					value_frec = currMenu -> item[i].tmp_value;
					task3_count_value = (1000000/ value_frec) / TIMER0_PERIOD; 	
				}
			break;

			case 2:
				if (currMenu == &CONF){
					value_mode = currMenu -> item[i].tmp_value;
				}
				else if (currMenu == &VIS){
					value_bujia = currMenu -> item[i].tmp_value;
				}
			break;
		}
		currMenu -> item[i].curr_value = currMenu -> item[i].tmp_value;
	}
}

void setFlagRecordingSD(){
	recordingSD = 1;
}

void clearFlagRecordingSD(){
	recordingSD = 0;
	blinkREC(true);
}

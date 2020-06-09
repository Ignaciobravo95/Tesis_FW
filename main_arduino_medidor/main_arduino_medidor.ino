/************************************************
 *  		   INCLUDES SECTION
 ***********************************************/
#include "com_bt.h"
#include "eepromlib.h"
#include <TimerOne.h>

/************************************************
 *   		DEFINES SECTION
 ***********************************************/
#define pinLED1 13 // CPU state
#define TIMER0_PERIOD		  	1000	 	/*  1ms */	
#define PERIODIC_TASK1_PERIOD 	10000000	/*  Check battery and stuff */
#define PERIODIC_TASK2_PERIOD 	500000   	/*  Send ADC MEASURE  */

/************************************************
 *   		FUNCTIONS DECLARATION
 ***********************************************/

/************************************************
 *   		GLOBAL DATA SECTION
 ***********************************************/
volatile bool flag_periodic_task1 = false;
volatile bool flag_periodic_task2 = false;
volatile bool flag_timer0		  = false;

/* AUX VARIABLES */
volatile uint32_t count = 0; 
uint32_t task3_count_value = 0, task2_count_value = 0, task1_count_value = 0;

/* GLOBAL SCOPE? */

/* MENU ITEMS */

/* EEPROM VALUES */					/* DYNAMIC RANGE */
uint8_t  value_frec = 0;			/* DR: 0 - 10 */
uint8_t  value_mode = 0;			/* DR: 0 - 1 */

void setup(void){
	/* READ CONFIG FROM EEPROM */
	value_frec = getEEPROM_frec();
	value_mode = getEEPROM_mode();

	/* TIMER INIT */
	Timer1.initialize(TIMER0_PERIOD); /* Value in microseconds? */

	/* TIMER TASK CALC */
	task2_count_value = PERIODIC_TASK2_PERIOD / TIMER0_PERIOD;
	task1_count_value = PERIODIC_TASK1_PERIOD / TIMER0_PERIOD;
  	
	/* INIT SERIAL COMMUNICATIONS */
	Serial.begin(9600);
	//Serial.println("Starting system...");

  	/* GPIO INIT */
	digitalWrite(pinLED1, HIGH);	

	/* ATTACH INTERRUPTS */
	Timer1.attachInterrupt(ISR_TIMER_EXPIRED); 

}

void loop(void){
	uint8_t i = 0;
	
	/* Main LOOP */
	/* PERIODIC TASK 1 EVENT */
	if ((flag_periodic_task1) && (!flag_periodic_task2)){
		digitalWrite(pinLED1, HIGH);
		/******************************
		- CHECK PERIODIC TASK W/LESS PRIORITY:
			- BATTERY VOLTAGE  
			- BLUETOOTH STATUS 
		******************************/
		/* Check battery */
		/******************************/
		//Serial.println("EVENT: PERIODIC TASK 1.");
		flag_periodic_task1 = false;
	}

	/* PERIODIC TASK 2 EVENT */
	if (flag_periodic_task2){
		digitalWrite(pinLED1, HIGH);
		/******************************
		- BLINK IF ANY BLINKING ITEMS 
		******************************/
		sendADCValue();
		/******************************/
		//Serial.println("EVENT: PERIODIC TASK 2.");
		flag_periodic_task2 = false;
	}

	/* SERIAL DATA FROM BT EVENT */
	if (Serial.available() > 0) {
	 	digitalWrite(pinLED1, HIGH);
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


	digitalWrite(pinLED1, LOW);
	/* SEND TO SLEEP */
}				


/************************************************
 *  		ISR
 ***********************************************/
void ISR_TIMER_EXPIRED(){
	count++;
	if (count % task1_count_value == 0){
		flag_periodic_task1 = true;
	}
	if (count % task2_count_value == 0){
		flag_periodic_task2 = true;
	}
	flag_timer0 = true;
}


/************************************************
 *  		EXTRA
 ***********************************************/
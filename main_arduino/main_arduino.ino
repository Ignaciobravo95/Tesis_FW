#include "menu.h"  

void setup(void){
	Serial.begin(9600);
	Serial.print("Starting system.");
	init_tft();
	menu_visualizacion(0, true,0.0);
	
}

void loop(void){

	
	menu_visualizacion(0, false, 10.3);

	for(uint16_t i = 0; i < 10; i++){
		menu_visualizacion(i,false,0);
		delay(1000);
	}
	

	for (;;)
		;	
}				
#include "menu.h"  

void setup(void){
	Serial.begin(9600);
	Serial.print("Starting system.");
	init_tft();
	menu_principal(0, true);
}

void loop(void){


}

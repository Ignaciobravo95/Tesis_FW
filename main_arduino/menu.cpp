/************************************************
 *   		INCLUDES
 ***********************************************/
#include "menu.h"
#include "bitmaps.h"
#include "Adafruit_GFX.h"
#include "MCUFRIEND_kbv.h"
#include "Fonts/seguibl7pt7b.h"

/************************************************
 *			GLOBAL VARIABLE SECTION
 ***********************************************/
MCUFRIEND_kbv 	tft;
/************************************************
 *   		PRIVATE FUNCTION DEFINITION
 ***********************************************/
void display_menu_options(uint8_t index){
	/* MENU OPTIONS */
	tft.drawBitmap(40,60, bmp_40_chart,  40, 40, index == 0 ? CYAN : WHITE);
	tft.setCursor(90, 84);
	tft.setTextColor( index == 0 ? CYAN : WHITE);
	tft.print("VISUALIZACION");

	tft.drawBitmap(40,110, bmp_40_person,  40, 40, index == 1 ? CYAN : WHITE); 
	tft.setCursor(90, 134);
	tft.setTextColor( index == 1 ? CYAN : WHITE);
	tft.print("PACIENTES");	

	tft.drawBitmap(40,160, bmp_40_settings,  40, 40, index == 2 ? CYAN : WHITE);
	tft.setCursor(90, 184);
	tft.setTextColor( index == 2 ? CYAN : WHITE);
	tft.print("CONFIGURACION");
}
/************************************************
 *   		FUNCTIONS DEFINITION
 ***********************************************/
void init_tft(){
	uint16_t ID = tft.readID();
    if (ID == 0xD3D3) ID = 0x9481; 
    tft.begin(ID);
    tft.setRotation(1);             // 1 = Horizontal
}

 void menu_principal(uint8_t index, bool refresh){
 	static uint8_t i = 0;

	if ( refresh ){
		tft.fillScreen(BLACK);	
		tft.setTextColor(KAKI);
		/* VERSION */
		tft.setCursor(230,225);
		tft.print("Version 1.0.0");
		/* TITLE DRAWING */
		tft.setFont(&seguibl7pt7b);	
		tft.setCursor(100,27);
		tft.print("MENU PRINCIPAL");
		/* MENU OPTIONS */	
		display_menu_options(index);
		i = index;
	} else if ( i != index ){
		/* MENU OPTIONS */	
		display_menu_options(index);
		i = index;
	}

}
	





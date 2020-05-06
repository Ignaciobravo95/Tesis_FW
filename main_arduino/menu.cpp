/************************************************
 *   		INCLUDES
 ***********************************************/
#include "menu.h"
#include "bitmaps.h"
#include "Adafruit_GFX.h"
#include "MCUFRIEND_kbv.h"

#define parr1_x 25
#define parr2_x 180


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
	tft.setCursor(90, 70);
	tft.setTextColor( index == 0 ? CYAN : WHITE);
	tft.print("VISUALIZACION");

	tft.drawBitmap(40,110, bmp_40_person,  40, 40, index == 1 ? CYAN : WHITE); 
	tft.setCursor(90, 120);
	tft.setTextColor( index == 1 ? CYAN : WHITE);
	tft.print("PACIENTES");	

	tft.drawBitmap(40,160, bmp_40_settings,  40, 40, index == 2 ? CYAN : WHITE);
	tft.setCursor(90, 170);
	tft.setTextColor( index == 2 ? CYAN : WHITE);
	tft.print("CONFIGURACION");
}

void display_visualizacion_options(uint8_t index){
	/* MENU OPTIONS */
	tft.setCursor(parr1_x,60);
	tft.setTextColor( index == 0 ? CYAN : WHITE);
	tft.print("- SELECCIONAR CALIBRE");

	tft.setCursor(parr1_x,75);
	tft.setTextColor( index == 1 ? CYAN : WHITE);
	tft.print("- INICIAR TEST");	

	tft.setCursor(parr1_x,90);
	tft.setTextColor( index == 2 ? CYAN : WHITE);
	tft.print("- REINICIAR");

	tft.setCursor(parr1_x,105);
	tft.setTextColor( index == 3 ? CYAN : WHITE);
	tft.print("- VOLVER ATRAS");
}

void reset_screen(){
	tft.fillScreen(BLACK);
	tft.setFont(NULL);
	tft.setTextSize(1);
	tft.setTextColor(WHITE);
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
		reset_screen();
		/* VERSION */
		tft.setCursor(230,225);
		tft.print("Version 1.0.0");
		/* TITLE DRAWING */
		tft.setTextSize(2);
		tft.setCursor(90,25);
		tft.print("MENU PRINCIPAL");
		/* MENU OPTIONS */	
		display_menu_options(0);
	} 
	else if ( i != index ){
		/* MENU OPTIONS */	
		display_menu_options(index);
		i = index;
	}
}
 
void menu_visualizacion(uint8_t index, bool refresh,float value){
	/*Variables*/
	static float max = 0.0;
	static uint8_t i = 0, x_value = 21;
	char press_value[] = {'0','0','.','0', NULL};
	uint8_t y_value = 0;

	/* PARSE PRESSURE VALUE*/
	if (value < 10){
		press_value[0] = '0';
	}
	else{
		press_value[0] = (int)(value / 10) % 10 + 48 ;
	}
	press_value[1] = (int)value % 10 + 48;
	press_value[3] = (int)(value * 10) % 10 + 48;

	if (refresh){
		max = 0.0;
		x_value = 21;
		reset_screen();
		/* VERSION */
		tft.setCursor(230,225);
		tft.print("Version 1.0.0");
		/* OPTION LABELS */
		display_visualizacion_options(0);
		/* CHART DATA LABELS */
		tft.setCursor(259, 150);
		tft.print("MAX:");
		/* PRESSURE UNIT */
		tft.setFont(NULL);
		tft.setCursor(parr2_x + 100,80);
		tft.print("kg/m2");
		/* CHART DATA VALUES */
		tft.setTextColor(GREEN);
		tft.setCursor(250, 160);
		tft.print("00.0");
		tft.setCursor(275, 160);
		tft.print("kg/m2");
		/* TITLE */
		tft.setTextColor(WHITE);
		tft.setTextSize(2);
		tft.setCursor(60,10);
		tft.print("MENU VISUALIZACION");
		/* SUBTITLES */
		tft.setCursor(parr1_x,35);
		tft.print("Opciones");
		tft.setCursor(parr2_x,35);
		tft.print("Presion");
		/* PRESSURE VALUE */
		tft.setTextSize(4);
		tft.setCursor(parr2_x,60);
		tft.print("00.0");
		/* AXIS */
		tft.drawLine(0, 120, 320 , 120, WHITE);
		tft.drawLine(22, 211, 22 , 130, WHITE);
		tft.drawLine(21, 210, 230, 210, WHITE);
	}
	else{
		/* UPDATE SELECTED OPTION*/
		tft.setTextSize(1);
		if (index != i){
			display_visualizacion_options(index);
			i = index;
		}
		/* CHART DATA VALUES */
		if ((float)value > (float)max){
			tft.setTextColor(GREEN, BLACK);
			tft.setCursor(250, 160);
			tft.print(press_value);
			max = value;
		}	
		/* CHART LINE GRAPH UPDATE */
		y_value = (value * -0.8) + 210;
		x_value++;
		if (x_value == 231){
			tft.writeFillRect(23, 125, 208, 85, BLACK);
			x_value = 22;
		}
		tft.drawPixel( x_value, y_value, WHITE);
		/* PRESSURE VALUE */
		tft.setTextColor(WHITE, BLACK);
		tft.setTextSize(4);
		tft.setCursor(parr2_x,60);
		tft.print(press_value);
	}
}
	





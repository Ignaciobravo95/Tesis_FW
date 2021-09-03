/************************************************
 *   		INCLUDES
 ***********************************************/
#include "menu.h"
#include "bitmaps.h"
#include "Adafruit_GFX.h"
#include "MCUFRIEND_kbv.h"

/************************************************
 *			GLOBAL VARIABLE SECTION
 ***********************************************/
MCUFRIEND_kbv 	tft;


/************************************************
 *   		PRIVATE FUNCTION DEFINITION
 ***********************************************/
uint16_t map_value(uint16_t value, uint16_t fromLow, uint16_t fromHigh, uint16_t toLow, uint16_t toHigh){
	return (value - fromLow) * (toHigh - toLow) / (fromHigh - fromLow) + toLow;
}

/************************************************
 *   		FUNCTIONS DEFINITION
 ***********************************************/

/* INIT */
void init_tft(){
	uint16_t ID = tft.readID();
    if (ID == 0xD3D3) ID = 0x9481; 
    tft.begin(ID);
    tft.setRotation(1);             // 1 = Horizontal
	tft.fillScreen(BLACK);
}

/* HEADER */
void menu_principal_header(){
	/* TITLE DRAWING */
	tft.fillScreen(BLACK);
	tft.fillRoundRect(50, 180, 220,  40, 8, RED   );
	tft.drawRoundRect(50, 180, 220,  40, 8, YELLOW); 
	tft.fillRoundRect(50, 130, 220,  40, 8, RED   );
	tft.drawRoundRect(50, 130, 220,  40, 8, YELLOW);  
	tft.fillRoundRect(50,  80, 220,  40, 8, RED   );   
	tft.drawRoundRect(50,  80, 220,  40, 8, YELLOW);
	tft.fillRoundRect(50,  30, 220,  40, 8, RED   );
	tft.drawRoundRect(50,  30, 220,  40, 8, YELLOW); 
	tft.setTextSize(2); 
	// tft.setTextColor(GREEN); 
	// tft.setCursor(190, 205); tft.print("UTN - FRC");
	tft.setTextColor(BLACK);  
	tft.setCursor(90, 195); tft.print("Calibracion");
	tft.setCursor(82, 145); tft.print("Configuracion");
	tft.setCursor(70,  95); tft.print("Num. de paciente");
	tft.setCursor(82,  45); tft.print("Visualizacion");
}
 
void menu_visualizacion_header(){
	/* TITLE */
	tft.fillScreen(BLACK);
	tft.drawRect(5,3,180,10,YELLOW);                    // Rectangulo para mostrar la presion en forma de barra
	tft.drawRect(5,18,180,172,YELLOW);                  // Rectangulo para mostrar la evolucion de la presion en el tiempo

	tft.fillRoundRect(4, 199, 70,  35, 8, RED   );      // Boton del menu de iniciar almacenamiento de datos
	tft.drawRoundRect(3, 198, 72,  37, 8, YELLOW);
	tft.fillTriangle(52, 215, 30, 202, 30, 228, BLACK);

	tft.fillRoundRect(84, 199, 70,  35, 8, RED   );     // Boton para frenar el almacenamiento de datos 
	tft.drawRoundRect(83, 198, 72,  37, 8, YELLOW);
	tft.fillRect(109, 208, 18, 18, BLACK);

	tft.fillRoundRect(164, 199, 70,  35, 8, RED   );    // Boton para volver al menu principal
	tft.drawRoundRect(163, 198, 72,  37, 8, YELLOW);  
	tft.fillRect(194, 213, 19, 7, BLACK);
	tft.fillTriangle(180, 216, 194, 206, 194, 226, BLACK);

	tft.fillRoundRect(244, 199, 70,  35, 8, RED   );    // Boton para cambiar la bujia
	tft.drawRoundRect(243, 198, 72,  37, 8, YELLOW);
	tft.setTextColor(BLACK);
	tft.setTextSize(3);tft.setCursor(252,205); tft.print(" B");


	tft.drawCircle(200,20,6,YELLOW);// Circulo para indicar cuando se estan grabando los datos

	tft.drawRect(285, 4,25,15,YELLOW); // Bateria
	tft.drawRect(310, 7, 5, 9,YELLOW);
	tft.drawRect(285, 4, 9,15,YELLOW);
	tft.drawRect(293, 4, 9,15,YELLOW);

	tft.setTextColor(WHITE);
	tft.setCursor(282,90);tft.setTextSize(2);tft.print(" kg");

	tft.setTextColor(WHITE);
	tft.setTextSize(2);tft.setCursor(200,142); tft.print("Bujia:");

	tft.setTextColor(WHITE);
	tft.setTextSize(2);tft.setCursor(200,162); tft.print("Thr:");
}

void menu_pacientes_header(){
	/* TITLE DRAWING */
	tft.fillScreen(BLACK);
	tft.fillRoundRect(50,  50, 220,  40, 8, RED   );
	tft.drawRoundRect(50,  50, 220,  40, 8, YELLOW); 
	tft.fillRoundRect(40, 180, 110,  40, 8, RED   );
	tft.drawRoundRect(40, 180, 110,  40, 8, YELLOW); 
	tft.fillRoundRect(170, 180, 110,  40, 8, RED   );
	tft.drawRoundRect(170, 180, 110,  40, 8, YELLOW);
	tft.setTextSize(2); 
	tft.setTextColor(BLACK);  
	tft.setCursor(92,  65); tft.print("ID PACIENTE");
	tft.setCursor(55,  195); tft.print("ACEPTAR");
	tft.setCursor(180, 195); tft.print("CANCELAR");
}

void menu_configuracion_header(){
	/* TITLE DRAWING */
	tft.fillScreen(BLACK);
	tft.fillRoundRect(20, 130, 100,  40, 8, RED   );
	tft.drawRoundRect(20, 130, 100,  40, 8, YELLOW);  

	tft.fillRoundRect(20, 80, 100,  40, 8, RED   );   
	tft.drawRoundRect(20, 80, 100,  40, 8, YELLOW);
	
	tft.fillRoundRect(20,  30, 100,  40, 8, RED   );
	tft.drawRoundRect(20,  30, 100,  40, 8, YELLOW); 
	
	tft.fillRoundRect(40, 180, 110,  40, 8, RED   );
	tft.drawRoundRect(40, 180, 110,  40, 8, YELLOW); 

	tft.fillRoundRect(170, 180, 110,  40, 8, RED   );
	tft.drawRoundRect(170, 180, 110,  40, 8, YELLOW);
	
	tft.setTextSize(2); 
	tft.setTextColor(BLACK);  
	tft.setCursor(40, 145); tft.print("MODO");
	tft.setCursor(40, 95); tft.print("FREQ");
	tft.setCursor(40,  45); tft.print("UMBR");
	tft.setCursor(55,  195); tft.print("ACEPTAR");
	tft.setCursor(180, 195); tft.print("CANCELAR");

	tft.setTextColor(WHITE);
	tft.setCursor(242,50);tft.setTextSize(2);tft.print(" kg");
	tft.setTextColor(WHITE);
	tft.setCursor(242,100);tft.setTextSize(2);tft.print(" Hz");
}

void menu_calibracion_step1_header(){
	/* TITLE DRAWING */
	tft.fillScreen(BLACK); 
	tft.fillRoundRect(40,  180, 110,  40, 8, RED   );
	tft.drawRoundRect(40,  180, 110,  40, 8, YELLOW); 
	tft.fillRoundRect(170, 180, 110,  40, 8, RED   );
	tft.drawRoundRect(170, 180, 110,  40, 8, YELLOW);
	tft.setTextSize(2); 
	tft.setTextColor(BLACK);  
	tft.setCursor(55,  195); tft.print("ACEPTAR");
	tft.setCursor(180, 195); tft.print("CANCELAR");
	tft.setTextColor(WHITE);  
	tft.setCursor(60,  55); tft.print("CALIBRACION PASO 1:");
	tft.setCursor(40,  95); tft.print("->Quitar peso de la");
	tft.setCursor(40, 115); tft.print(" balanza y presionar");
	tft.setCursor(40, 135); tft.print(" aceptar");
}

void menu_calibracion_step2_header(){
	/* TITLE DRAWING */
	tft.fillScreen(BLACK); 
	tft.fillRoundRect(40,  180, 110,  40, 8, RED   );
	tft.drawRoundRect(40,  180, 110,  40, 8, YELLOW); 
	tft.fillRoundRect(170, 180, 110,  40, 8, RED   );
	tft.drawRoundRect(170, 180, 110,  40, 8, YELLOW);
	tft.setTextSize(2); 
	tft.setTextColor(BLACK);  
	tft.setCursor(55,  195); tft.print("ACEPTAR");
	tft.setCursor(180, 195); tft.print("CANCELAR");
	tft.setTextColor(WHITE);  
	tft.setCursor(60,  55); tft.print("CALIBRACION PASO 2:");
	tft.setCursor(40,  95); tft.print("->Colocar patron de");
	tft.setCursor(40, 115); tft.print(" 120g y presionar");
	tft.setCursor(40, 135); tft.print(" aceptar");
}

void menu_calibracion_step3_header(){
	/* TITLE DRAWING */
	tft.fillScreen(BLACK); 
	tft.fillRoundRect(40,  180, 110,  40, 8, RED   );
	tft.drawRoundRect(40,  180, 110,  40, 8, YELLOW); 
	tft.fillRoundRect(170, 180, 110,  40, 8, RED   );
	tft.drawRoundRect(170, 180, 110,  40, 8, YELLOW);
	tft.setTextSize(2); 
	tft.setTextColor(BLACK);  
	tft.setCursor(55,  195); tft.print("ACEPTAR");
	tft.setCursor(180, 195); tft.print("CANCELAR");
	tft.setTextColor(WHITE);  
	tft.setCursor(60,  55); tft.print("CALIBRACION PASO 3:");
	tft.setCursor(40,  95); tft.print("->Colocar patron de");
	tft.setCursor(40, 115); tft.print(" 480g y presionar");
	tft.setCursor(40, 135); tft.print(" aceptar");
}

void menu_calibracion_step4_header(){
	/* TITLE DRAWING */
	tft.fillScreen(BLACK); 
	tft.fillRoundRect(40,  180, 110,  40, 8, RED   );
	tft.drawRoundRect(40,  180, 110,  40, 8, YELLOW); 
	tft.fillRoundRect(170, 180, 110,  40, 8, RED   );
	tft.drawRoundRect(170, 180, 110,  40, 8, YELLOW);
	tft.setTextSize(2); 
	tft.setTextColor(BLACK);  
	tft.setCursor(55,  195); tft.print("ACEPTAR");
	tft.setCursor(180, 195); tft.print("CANCELAR");
	tft.setTextColor(WHITE);  
	tft.setCursor(60,  55); tft.print("CALIBRACION PASO 4:");
	tft.setCursor(40,  95); tft.print("->Colocar patron de");
	tft.setCursor(40, 115); tft.print(" 960g y presionar");
	tft.setCursor(40, 135); tft.print(" aceptar");
}

void menu_calibracion_step5_header(){
	/* TITLE DRAWING */
	tft.fillScreen(BLACK); 
	tft.fillRoundRect(40,  180, 110,  40, 8, RED   );
	tft.drawRoundRect(40,  180, 110,  40, 8, YELLOW); 
	tft.fillRoundRect(170, 180, 110,  40, 8, RED   );
	tft.drawRoundRect(170, 180, 110,  40, 8, YELLOW);
	tft.setTextSize(2); 
	tft.setTextColor(BLACK);  
	tft.setCursor(55,  195); tft.print("ACEPTAR");
	tft.setCursor(180, 195); tft.print("CANCELAR");
	tft.setTextColor(WHITE);  
	tft.setCursor(60,  55); tft.print("CALIBRACION PASO 5:");
	tft.setCursor(40,  95); tft.print("->Colocar patron de");
	tft.setCursor(40, 115); tft.print(" 1440g y presionar");
	tft.setCursor(40, 135); tft.print(" aceptar");
}


/* OPTIONS */
void menu_principal_option(uint8_t i){
	/* OPTIONS */
	tft.drawRoundRect(48,  28, 224, 44, 8, i == 0 ? YELLOW : BLACK ); 
    tft.drawRoundRect(48,  78, 224, 44, 8, i == 1 ? YELLOW : BLACK ); 
    tft.drawRoundRect(48, 128, 224, 44, 8, i == 2 ? YELLOW : BLACK ); 
    tft.drawRoundRect(48, 178, 224, 44, 8, i == 3 ? YELLOW : BLACK ); 
}

void menu_visualizacion_option(uint8_t i){
	/* OPTIONS */
	tft.drawRoundRect(  1, 196, 76, 41, 8, i == 0? YELLOW : BLACK);
    tft.drawRoundRect (81, 196, 76, 41, 8, i == 1? YELLOW : BLACK);
    tft.drawRoundRect(161, 196, 76, 41, 8, i == 2? YELLOW : BLACK);
    tft.drawRoundRect(241, 196, 76, 41, 8, i == 3? YELLOW : BLACK);
}

void menu_configuracion_option(uint8_t i){
	/* OPTIONS */
	tft.drawRoundRect(18,  28, 104, 44, 8, i == 0 ? YELLOW : BLACK ); 
    tft.drawRoundRect(18,  78, 104, 44, 8, i == 1 ? YELLOW : BLACK ); 
    tft.drawRoundRect(18, 128, 104, 44, 8, i == 2 ? YELLOW : BLACK ); 
    tft.drawRoundRect(38, 178, 114, 44, 8, i == 3 ? YELLOW : BLACK ); 
    tft.drawRoundRect(168, 178, 114, 44, 8, i == 4 ? YELLOW : BLACK ); 
}

void menu_pacientes_option(uint8_t i){
	/* OPTIONS */
	tft.drawRoundRect(48,  48, 224, 44, 8, i == 0 ? YELLOW : BLACK ); 
    tft.drawRoundRect(38, 178, 114, 44, 8, i == 1 ? YELLOW : BLACK ); 
    tft.drawRoundRect(168, 178, 114, 44, 8, i == 2? YELLOW : BLACK ); 
}

void menu_calibracion_option(uint8_t i){
	/* OPTIONS */
    tft.drawRoundRect(38,  178, 114, 44, 8, i == 0? YELLOW : BLACK ); 
    tft.drawRoundRect(168, 178, 114, 44, 8, i == 1? YELLOW : BLACK ); 
}

/* FIELD */
void menu_visualizacion_fields(){
	String str1 = String(global_val[BUJIA]);
	String str2 = String(global_val[TH]);
	
	while (str1.length() < 2){
		str1 = '0' + str1;
	}

	while (str2.length() < 2){
		str2 = '0' + str2;
	}

    /* BUJIA */
    tft.setTextColor(WHITE);
	tft.setTextSize(2);tft.setCursor(280,142); tft.print(str1);

	/* TH */
	tft.setTextColor(WHITE);
	tft.setTextSize(2);tft.setCursor(270,162); tft.print(str2);	

	/* LECTURA */
	tft.setTextColor(WHITE); 
	tft.setTextSize(4);tft.setCursor(195,76); tft.print(0.0,1);
}

void menu_configuracion_fields(){
	String str1 = String(global_val[TH]);
	String str2 = String(global_val[FREC]);

	while (str1.length() < 2){
 		str1 = '0' + str1;
 	}

 	while (str2.length() < 2){
 		str2 = '0' + str2;
 	}

	/* UMBRAL */
	tft.setTextColor(WHITE); 
    tft.setTextSize(4);tft.setCursor(170,35); tft.print(str1);

    /* FREC */
    tft.setTextColor(WHITE);
	tft.setTextSize(4);tft.setCursor(170,85); tft.print(str2);

	/* MODO */
	tft.setTextColor(WHITE);
	tft.setTextSize(2);
	if(global_val[MODE] == 0){
		tft.setCursor(150,145); tft.print("PRESS & HOLD");
	}
	else {
		tft.setCursor(160,145); tft.print("PRESS ONCE");
	}
}

void menu_pacientes_fields(){
	String str = String(global_val[IDPAC]);
	/* UMBRAL */
 	while (str.length() < 5){
 		str = '0' + str;
 	}
	tft.setTextSize(4); 
	tft.setTextColor(WHITE);  
	tft.setCursor(92,  125); tft.print(str);
}

/* SIGNAL */
void menu_visualizacion_signal(uint32_t x, uint8_t reset){
	static uint8_t sample_number = 5, last_value = 188;
	uint8_t new_value;
	float presion = (x - offset)*slope;// * 5.0 / 16000000.0 ;
	if (presion > 5.0)
		presion = 0;

	/* LECTURA NUMBER */
	tft.fillRect(195,76,96,35,BLACK);

	if (!reset){
		if(presion > 1.8){
		    tft.setTextColor(RED); 
		    tft.setTextSize(4);tft.setCursor(195,76); tft.print(presion,2);
		}else{
		    tft.setTextColor(WHITE); 
		    tft.setTextSize(4);tft.setCursor(195,76); tft.print(presion,2);
		}
		/* LECTURA VS SAMPLE */
		sample_number++;
		if (sample_number > 184 ){
			sample_number = 6; 
			tft.fillRect(6, 19, 178, 170, BLACK);
		}

		new_value = map(x-offset , 0 ,1.5e6, 188, 18);
		if (new_value>=188)
			new_value = 188;
		else if (new_value<18)
			new_value = 18;
		tft.drawPixel(sample_number, new_value, WHITE);
		tft.drawLine(sample_number-1, last_value, sample_number, new_value, WHITE);

		last_value = new_value;
    }
    else{
    	sample_number = 5;
    }

    /* LECTURA */
	// tft.setTextColor(WHITE); 
	// tft.setTextSize(4);tft.setCursor(195,76); tft.print(presion,1);

}

/* STATUS */
void batteryStatus(float x){
	// Bateria
	tft.drawRect(285, 4,25,15,YELLOW); 
    tft.drawRect(310, 7, 5, 9,YELLOW);
    tft.drawRect(285, 4, 9,15,YELLOW);
    tft.drawRect(293, 4, 9,15,YELLOW);

	if (x >= 4.0){
    	tft.fillRect(286, 5, 7,13,GREEN);  
    	tft.fillRect(294, 5, 7,13,GREEN);
    	tft.fillRect(302, 5, 7,13,GREEN);
  	}
  	else if (x >= 3.6){
      	tft.fillRect(286, 5, 7,13,GREEN); 
		tft.fillRect(294, 5, 7,13,GREEN);
		tft.fillRect(302, 5, 7,13,BLACK);
    }else{
		tft.fillRect(286, 5, 7,13,RED); 
		tft.fillRect(294, 5, 7,13,BLACK);
		tft.fillRect(302, 5, 7,13,BLACK);
    }
}

void bluetoothStatus(uint8_t x){
	if ( x == 0){
		tft.drawBitmap(260,4, bmp_16_bt, 16,16, GREEN);
	}
	else if (x == 1){
		tft.drawBitmap(260,4, bmp_16_bt, 16,16, YELLOW);
	}
	else{
		tft.drawBitmap(260,4, bmp_16_bt, 16,16, RED);	
	}
}

/* EXTRA */
void blinknoSD(uint8_t force){
	static uint8_t on = false;
	if (on && !force ){
		tft.setCursor(195,40); tft.setTextColor(RED); tft.setTextSize(2); tft.print("NO SD CARD");
	}
	else{
		tft.setCursor(195,40); tft.setTextColor(BLACK); tft.setTextSize(2); tft.print("NO SD CARD");
	}
	on = ~on;
}

void blinkREC(uint8_t force){
	static uint8_t on = false;
	tft.setCursor(211,16); tft.setTextColor(RED); tft.setTextSize(1); tft.print("REC.");
	if (on && !force ){
		tft.fillCircle(200,20,5,RED);
	}
	else{
		tft.fillCircle(200,20,5,BLACK);
		tft.setCursor(211,16); tft.setTextColor(BLACK); tft.setTextSize(1); tft.print("REC.");
	}
	on = ~on;
}

void blinkBUJIA(uint16_t x,uint8_t force, uint8_t dig){
	static uint8_t on = false;

	String str = String(x);

	while (str.length() < 2){
		str = '0' + str;
	}

	if (on || force){
		tft.setCursor(280,142);tft.setTextColor(WHITE);tft.setTextSize(2); tft.print(str);
	}
	else{
		tft.fillRect(280+((1-dig)*12),142,12,14,BLACK);
	}
	on = ~on;
}

void blinkIDPACIENTE(uint16_t x,uint8_t force, uint8_t dig){
	static uint8_t on = false;

	String str = String(x);
 	while (str.length() < 5){
 		str = '0' + str;
 	}


	if (on || force){
		tft.setTextSize(4);tft.setTextColor(WHITE);tft.setCursor(92,  125); tft.print(str);
	}
	else{
		tft.fillRect(92+((4-dig)*24), 125, 24, 28, BLACK);
		// custom font has textSize*(6*7) size
	}
	on = !on;	
}

void blinkTH(uint16_t x,uint8_t force, uint8_t dig){
	static uint8_t on = false;

	String str = String(x);
 	while (str.length() < 2){
 		str = '0' + str;
 	}

	if (on || force){
		tft.setTextColor(WHITE);tft.setTextSize(4);tft.setCursor(170,35); tft.print(str);
	}
	else{
		tft.fillRect(170+((1-dig)*24),35,24,28,BLACK);
	}
	on = !on;	
}

void blinkFREQ(uint16_t x,uint8_t force, uint8_t dig){
	static uint8_t on = false;

	String str = String(x);
 	while (str.length() < 2){
 		str = '0' + str;
 	}

	if (on || force){
		tft.setTextColor(WHITE);tft.setTextSize(4);tft.setCursor(170,85); tft.print(str);
	}
	else{
		tft.fillRect(170+((1-dig)*24),85,24,28,BLACK);
	}
	on = !on;	
}

void blinkMODO(uint16_t x,uint8_t force, uint8_t dig){
	static uint8_t on = false;
	if (on || force){
		tft.setTextColor(WHITE);tft.setTextSize(2);
		if(x == 0){
			tft.setCursor(150,145); tft.print("PRESS & HOLD");
		}
		else {
			tft.setCursor(160,145); tft.print("PRESS ONCE");
		}
	}
	else{
		tft.fillRect(150,145,150,20,BLACK);		
	}
	on = !on;	
}
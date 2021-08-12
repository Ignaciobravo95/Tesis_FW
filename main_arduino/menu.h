#ifndef __MENU_H__
#define __MENU_H__

/************************************************
 *  		   INCLUDES SECTION
 ***********************************************/
//#include <Stdint.h>	
#include "extra.h"			
#include <stddef.h>
#include <avr/pgmspace.h>


// / LINK ( VISUALIZACION - PAC - CONF )	
// 	/ nextMenu != NULL
// 	/ doAction == NULL

// / LINK + ACCION 
// 	/ nextMenu != NULL
// 	/ doAction != NULL

// / ACCION
// 	/ nextMenu == NULL
// 	/ doAction != NULL

// / VALOR ( FRECUENCIA ) / 
// 	/ blinking = 0 = no lo estoy modificando o 1 = modificando ; 
// 	/ nextMenu == NULL
// 	/ doAction == NULL
// 	/ blinking_function != NULL
// 	/ curr_value 		= value_frecuencia;
// 	/ tmp_value			= valor temporal ;
// 	/ value_limit 		= 100;


/************************************************
 *  		TYPEDEF SECTION
 ***********************************************/
typedef struct menu_item{
	bool blinking = 0;
	uint16_t value_limit = 0;
	uint16_t value_id = 0;
	uint16_t tmp_value = 0;
	void (*blinking_function)(uint16_t x, uint8_t force, uint8_t dig) = NULL;
	void (*doAction)() = NULL;
	void *nextMenu = NULL;
}menuItem_t;

typedef struct menu_page{
	uint8_t items_number = 0;
	menuItem_t item[5];
	void (*display_header)() = NULL;
	void (*display_option)(uint8_t i) = NULL;
	void (*display_fields)() = NULL;
}menu_t;


/************************************************
 *   		DEFINES SECTION
 ***********************************************/
// Color definitions
#define BLACK   	0x0000
#define BLUE    	0x001F
#define RED     	0xF800
#define GREEN   	0x07E0
#define CYAN    	0x04F3
#define MAGENTA 	0xF81F
#define YELLOW  	0xFFE0
#define WHITE		0xFFFF
#define KAKI 		0xCDCE
/************************************************
 *   		GLOBAL DATA SECTION
 ***********************************************/
extern uint32_t global_val[5];

/************************************************
 *   		FUNCTIONS DECLARATION
 ***********************************************/

/**
 * @brief Inicializa el objeto del TFT. 
 *
 * @param 	No
 * @return  No
 */
void init_tft();

/**
 * @brief <Imprime los graficos estaticos del menu principal>
 *
 *
 * @param 	No
 * @return  No
 *
 */
void menu_principal_header();

/**
 * @brief <Imprime los graficos estaticos del menu visualizacion>
 *
 *
 * @param 	No
 * @return  No
 *
 */
void menu_visualizacion_header();

/**
 * @brief <Imprime los graficos estaticos del menu pacientes>
 *
 *
 * @param 	No
 * @return  No
 *
 */
void menu_pacientes_header();

/**
 * @brief <Imprime los graficos estaticos del menu configuracion>
 *
 *
 * @param 	No
 * @return  No
 *
 */
void menu_configuracion_header();


/**
 * @brief <Imprime los graficos estaticos del menu CALIBRACION PASO 1>
 *
 *
 * @param 	No
 * @return  No
 *
 */
void menu_calibracion_step1_header();
void menu_calibracion_step2_header();
void menu_calibracion_step3_header();
void menu_calibracion_step4_header();

/**
 * @brief <Imprime grafico que muestra resalta la opcion seleccionada >
 *
 *
 * @param 	<i> indice de la opcion seleccionada
 * @return  No
 *
 */
void menu_principal_option(uint8_t i);

/**
 * @brief <Imprime grafico que muestra resalta la opcion seleccionada >
 *
 *
 * @param 	<i> indice de la opcion seleccionada
 * @return  No
 *
 */
void menu_visualizacion_option(uint8_t i);

/**
 * @brief <Imprime grafico que muestra resalta la opcion seleccionada >
 *
 *
 * @param 	<i> indice de la opcion seleccionada
 * @return  No
 *
 */
void menu_pacientes_option(uint8_t i);

/**
 * @brief <Imprime grafico que muestra resalta la opcion seleccionada >
 *
 *
 * @param 	<i> indice de la opcion seleccionada
 * @return  No
 *
 */
void menu_configuracion_option(uint8_t i);

/**
 * @brief <Imprime grafico que muestra resalta la opcion seleccionada >
 *
 *
 * @param 	<i> indice de la opcion seleccionada
 * @return  No
 *
 */
void menu_calibracion_option(uint8_t i);

/**
 * @brief <Imprime los campos que van a ser editables luego>
 *
 *
 * @param <i> 	<Resalta la opcion seleccionada.>
 * @param <refresh> <Refresca la pantalla>
 *
 */
void menu_visualizacion_fields();

/**
 * @brief <Imprime el menu principal en el display>
 *
 *
 * @param <i> 	<Resalta la opcion seleccionada.>
 * @param <refresh> <Refresca la pantalla>
 *
 */
void menu_visualizacion_signal(uint32_t x, uint8_t reset);

/**
 * @brief <Imprime el menu principal en el display>
 *
 *
 * @param <i> 	<Resalta la opcion seleccionada.>
 * @param <refresh> <Refresca la pantalla>
 *
 */
void menu_configuracion_fields();

/**
 * @brief <Imprime el menu principal en el display>
 *
 *
 * @param <i> 	<Resalta la opcion seleccionada.>
 * @param <refresh> <Refresca la pantalla>
 *
 */
void menu_pacientes_fields();

/**
 * @brief <Imprime el menu principal en el display>
 *
 *
 * @param <i> 	<Resalta la opcion seleccionada.>
 * @param <refresh> <Refresca la pantalla>
 *
 */
void batteryStatus(uint8_t x);

/**
 * @brief <Imprime el menu principal en el display>
 *
 *
 * @param <i> 	<Resalta la opcion seleccionada.>
 * @param <refresh> <Refresca la pantalla>
 *
 */
void bluetoothStatus(uint8_t x);

/**
 * @brief <Imprime el menu principal en el display>
 *
 *
 * @param <i> 	<Resalta la opcion seleccionada.>
 * @param <refresh> <Refresca la pantalla>
 *
 */
void blinkREC(uint8_t force);

/**
 * @brief <Imprime el menu principal en el display>
 *
 *
 * @param <i> 	<Resalta la opcion seleccionada.>
 * @param <refresh> <Refresca la pantalla>
 *
 */
void blinkBUJIA(uint16_t x, uint8_t force, uint8_t dig);

/**
 * @brief <Imprime el menu principal en el display>
 *
 *
 * @param <i> 	<Resalta la opcion seleccionada.>
 * @param <refresh> <Refresca la pantalla>
 *
 */
void blinkIDPACIENTE(uint16_t x, uint8_t force, uint8_t dig);

/**
 * @brief <Imprime el menu principal en el display>
 *
 *
 * @param <i> 	<Resalta la opcion seleccionada.>
 * @param <refresh> <Refresca la pantalla>
 *
 */
void blinkTH(uint16_t x, uint8_t force, uint8_t dig);

/**
 * @brief <Imprime el menu principal en el display>
 *
 *
 * @param <i> 	<Resalta la opcion seleccionada.>
 * @param <refresh> <Refresca la pantalla>
 *
 */
void blinkFREQ(uint16_t x, uint8_t force, uint8_t dig);

/**
 * @brief <Imprime el menu principal en el display>
 *
 *
 * @param <i> 	<Resalta la opcion seleccionada.>
 * @param <refresh> <Refresca la pantalla>
 *
 */
void blinkMODO(uint16_t x, uint8_t force, uint8_t dig);

#endif

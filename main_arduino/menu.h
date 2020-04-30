#ifndef __MENU_H__
#define __MENU_H__

/************************************************
 *  		   INCLUDES SECTION
 ***********************************************/
#include <Stdint.h>				
#include <avr/pgmspace.h>
/************************************************
 *  		TYPEDEF SECTION
 ***********************************************/

/************************************************
 *   		DEFINES SECTION
 ***********************************************/
// Color definitions
#define BLACK   	0x0000
#define BLUE    	0x001F
#define RED     	0xF800
#define GREEN   	0x07E0
#define CYAN    	0x07FF
#define MAGENTA 	0xF81F
#define YELLOW  	0xFFE0
#define WHITE		0xFFFF
#define KAKI 		0xCDCE
/************************************************
 *   		GLOBAL DATA SECTION
 ***********************************************/

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

/************************************************
 *   		FUNCTIONS DECLARATION
 ***********************************************/
/**
 * @brief <Imprime el menu principal en el display>
 *
 *
 * @param <index> 	<Resalta la opcion seleccionada.>
 * @param <refresh> <Refresca la pantalla>
 *
 */
void menu_principal(uint8_t index, bool refresh);


#endif
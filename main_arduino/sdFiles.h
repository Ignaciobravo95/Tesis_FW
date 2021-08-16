#ifndef __SDFILES_H__
#define __SDFILES_H__

/************************************************
 *  		   INCLUDES SECTION
 ***********************************************/
#include <avr/pgmspace.h>
/************************************************
 *  		TYPEDEF SECTION
 ***********************************************/

/************************************************
 *   		DEFINES SECTION
 ***********************************************/
#define SDWRITEBUFFER 512
#define chipSelectSD 11

/************************************************
 *   		GLOBAL DATA SECTION
 ***********************************************/

/************************************************
 *   		FUNCTIONS DECLARATION
 ***********************************************/
/**
 * @brief <Add Brief Description of Function Here>
 *
 * <Add Extended Description Here>
 *
 * @param <Add InputName> <add description here>
 * @param <Add InputName> <add description here>
 * @param <Add InputName> <add description here>
 * @param <Add InputName> <add description here>
 *
 * @return <Add Return Informaiton here>
 */
int initSDroutine(uint32_t BUJIA, uint32_t ID);
void writeSD();


#endif

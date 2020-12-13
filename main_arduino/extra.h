#ifndef __EXTRA_H__
#define __EXTRA_H__

/************************************************
 *  		   INCLUDES SECTION
 ***********************************************/
#include <Stdint.h>				
/************************************************
 *   		DEFINES SECTION
 ***********************************************/
/* CONST */
uint32_t const pow_10_lut[10] = {	1,
									10,
									100,
									1000,
									10000,
									100000,
									1000000,
									10000000,
									100000000};

/************************************************
 *  		TYPEDEF SECTION
 ***********************************************/
typedef enum{
	FREC, 
	MODE,
	TH,
	IDPAC,
	BUJIA,
}e_ValID;

/************************************************
 *   		GLOBAL DATA SECTION
 ***********************************************/


/************************************************
 *   		FUNCTIONS DECLARATION
 ***********************************************/
/**
 * @brief This function runs the state machine.
 *
 * @param <Add InputName> <add description here>
 * @param <Add InputName> <add description here>
 * @param <Add InputName> <add description here>
 * @param <Add InputName> <add description here>
 *
 * @return <Add Return Informaiton here>
 */
uint8_t checknDigits(uint16_t x);

/**
 * @brief This function runs the state machine.
 *
 * @param <Add InputName> <add description here>
 * @param <Add InputName> <add description here>
 * @param <Add InputName> <add description here>
 * @param <Add InputName> <add description here>
 *
 * @return <Add Return Informaiton here>
 */
uint8_t getDigitValue(uint16_t x, uint8_t dig);

/**
 * @brief This function runs the state machine.
 *
 * @param <Add InputName> <add description here>
 * @param <Add InputName> <add description here>
 * @param <Add InputName> <add description here>
 * @param <Add InputName> <add description here>
 *
 * @return <Add Return Informaiton here>
 */
uint16_t modValueDigit(uint16_t x, uint8_t dig, uint8_t digitValue);

/**
 * @brief This function runs the state machine.
 *
 * @param <Add InputName> <add description here>
 * @param <Add InputName> <add description here>
 * @param <Add InputName> <add description here>
 * @param <Add InputName> <add description here>
 *
 * @return <Add Return Informaiton here>
 */
uint8_t getDigitMax(uint16_t max, uint16_t x, uint8_t numdig,  uint8_t dig);

#endif
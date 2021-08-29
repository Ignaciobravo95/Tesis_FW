#ifndef __EEPROMLIB_H__
#define __EEPROMLIB_H__

/************************************************
 *  		   INCLUDES SECTION
 ***********************************************/
#include <EEPROM.h> 
//#include <Stdint.h>				
#include <avr/pgmspace.h>
/************************************************
 *  		TYPEDEF SECTION
 ***********************************************/
typedef enum add{
	ID_ADDR 	=		0x00,
    BUJIA_ADDR 	=		0x04,
	TH_ADDR		=		0x08,
	FREC_ADDR	=		0x0C,
	MODE_ADDR 	=		0x10,
	OFFSET_ADDR =		0x14,
	SLOPE_ADDR 	=		0x18,
}e_eepromAddr;

/************************************************
 *   		DEFINES SECTION
 ***********************************************/
#define DEFAULT_ID		 	1
#define DEFAULT_TH			5
#define DEFAULT_FREC		10
#define DEFAULT_MODE		0
#define DEFAULT_BUJIA		8
#define DEFAULT_OFFSET		0
#define DEFAULT_SLOPE		1.0

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
void writeEEPROM_th(uint8_t val);

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
void writeEEPROM_frec(uint8_t val);

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
void writeEEPROM_mode(uint8_t val);

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
void writeEEPROM_bujia(uint8_t val);

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
void writeEEPROM_id(uint16_t val);
void writeEEPROM_offset(uint32_t val);
void writeEEPROM_slope(float val);

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
uint8_t getEEPROM_th();

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
uint8_t getEEPROM_frec();

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
uint8_t getEEPROM_mode();

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
uint8_t getEEPROM_bujia();

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
uint16_t getEEPROM_id();
uint32_t getEEPROM_offset();
float getEEPROM_slope();


#endif

#ifndef __COMBT_H__
#define __COMBT_H__

/************************************************
 *  		   INCLUDES SECTION
 ***********************************************/
#include <Stdint.h>				
#include <avr/pgmspace.h>
/************************************************
 *  		TYPEDEF SECTION
 ***********************************************/
typedef struct status{
	TX_ERROR,
	RX_NO_ACK,
	DATA_ERRROR,
}btStatus;
/************************************************
 *   		DEFINES SECTION
 ***********************************************/
#define RX_BUFFERSIZE 100

/************************************************
 *   		GLOBAL DATA SECTION
 ***********************************************/
uint8_t rx_buffer[RX_BUFFERSIZE];
uint8_t *tail,*head;

tail = rx_buffer;
head = rx_buffer;

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
btStatus parseReceivedData(uint8_t ucReceived);

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
btStatus checkBTStatus();


#endif
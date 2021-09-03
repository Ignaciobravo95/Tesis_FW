/************************************************
 *   		INCLUDES
 ***********************************************/
#include "sdFiles.h"
#include "com_bt.h"
#include <SPI.h>
#include <SD.h>

/************************************************
 *			GLOBAL VARIABLE SECTION
 ***********************************************/
float timevector[SDWRITEBUFFER];
float bufferSD[SDWRITEBUFFER];
float CONSTANT_FRAMS_Sec;
String file_name;
File dataFile;




/************************************************
 *   		PRIVATE FUNCTION DEFINITION
 ***********************************************/
int initSDroutine(uint32_t BUJIA, uint32_t ID){
	/* INIT SD */
	if (!SD.begin(chipSelectSD)) {
    	return 1;
  	}

  	/* GENERATE FOLDER */
	String dir = String(ID);
	if (!SD.exists(dir)){
		SD.mkdir(dir); 
	}

	/* GENERATE FILENAME  */
  	int measurment_index = 0;
	String tmp = dir + "/B" + String(BUJIA) + "_M" + String(measurment_index) + ".csv";   
	while (SD.exists(tmp)){
		measurment_index++;
		tmp = dir + "/B" + String(BUJIA) + "_M" + String(measurment_index) + ".csv";   
	}
	file_name = tmp;
  	
	return 0;
}


void writeSD(uint16_t SDbufferindex,uint8_t frame){
	/* GENERATE DATAFILE TO WRITE TO */
	dataFile = SD.open(file_name, FILE_WRITE);
	for (unsigned int i= 0; i < SDbufferindex; i++){
     	dataFile.print(timevector[i]+frame*CONSTANT_FRAMS_Sec);
     	dataFile.print(",");
     	dataFile.print(bufferSD[i]);
     	dataFile.println(",");
    }
    dataFile.close();
}

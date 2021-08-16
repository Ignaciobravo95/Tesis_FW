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
AdcDataType bufferSD[SDWRITEBUFFER];
String file_name;
File dataFile;
uint8_t adc_values[512];

/************************************************
 *   		PRIVATE FUNCTION DEFINITION
 ***********************************************/
int initSDroutine(uint32_t BUJIA, uint32_t ID){
	/* INIT SD */
	dataFile.close();
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

	/* GENERATE DATAFILE TO WRITE TO */
	dataFile = SD.open(file_name, FILE_WRITE);


	for (int i =0; i <512;i++){
    	adc_values[i] = random(0,512);
  	}
  	
	return 0;
}


void writeSD(){
	for (unsigned int i= 0; i < SDWRITEBUFFER; i++){
     	dataFile.print(adc_values[i]);
     	dataFile.println(",");
    }
}

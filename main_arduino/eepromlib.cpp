/************************************************
 *   		INCLUDES
 ***********************************************/
 #include "eepromlib.h"

 /************************************************
 *			GLOBAL VARIABLE SECTION
 ***********************************************/

 /************************************************
 *   		PRIVATE FUNCTION DEFINITION
 ***********************************************/

 /************************************************
 *   		FUNCTIONS DEFINITION
 ***********************************************/
void writeEEPROM_th(uint8_t val){

	EEPROM.write(TH_ADDR, val);
}

void writeEEPROM_frec(uint8_t val){

	EEPROM.write(FREC_ADDR, val);
}

void writeEEPROM_mode(uint8_t val){

	EEPROM.write(MODE_ADDR, val);
}

void writeEEPROM_bujia(uint8_t val){

	EEPROM.write(BUJIA_ADDR, val);
}

void writeEEPROM_id(uint16_t val){
	uint8_t byte;

	byte = val & 0x00FF;
	EEPROM.write(ID_ADDR, byte);
	byte = (val >> 8 ) & 0x00FF;
	EEPROM.write(ID_ADDR+1, byte);
}

void writeEEPROM_offset(uint32_t val){
	uint8_t byte;

	byte = val & 0x00FF;
	EEPROM.write(OFFSET_ADDR, byte);
	byte = (val >> 8 ) & 0x00FF;
	EEPROM.write(OFFSET_ADDR+1, byte);
	byte = (val >> 16 ) & 0x00FF;
	EEPROM.write(OFFSET_ADDR+2, byte);
	byte = (val >> 24 ) & 0x00FF;
	EEPROM.write(OFFSET_ADDR+3, byte);
}

void writeEEPROM_slope(float val){
	uint8_t byte;
	uint32_t *valuint = (uint32_t *)&val;

	byte = *valuint & 0x00FF;
	EEPROM.write(SLOPE_ADDR, byte);
	byte = (*valuint >> 8 ) & 0x00FF;
	EEPROM.write(SLOPE_ADDR+1, byte);
	byte = (*valuint >> 16 ) & 0x00FF;
	EEPROM.write(SLOPE_ADDR+2, byte);
	byte = (*valuint >> 24 ) & 0x00FF;
	EEPROM.write(SLOPE_ADDR+3, byte);
}

uint8_t getEEPROM_th(){
	uint8_t byte = EEPROM.read(TH_ADDR);
	if (byte == 0xFF){
		writeEEPROM_th(DEFAULT_TH);
		return DEFAULT_TH;
	} 
	return byte; 
}

uint8_t getEEPROM_frec(){
	uint8_t byte = EEPROM.read(FREC_ADDR);
	if (byte == 0xFF){
		writeEEPROM_frec(DEFAULT_FREC);
		return DEFAULT_FREC;
	} 
	return byte;
}

uint8_t getEEPROM_mode(){
	uint8_t byte = EEPROM.read(MODE_ADDR);
	if (byte  == 0xFF){
		writeEEPROM_mode(DEFAULT_MODE);
		return DEFAULT_MODE;
	} 
	return byte;
}

uint8_t getEEPROM_bujia(){
	uint8_t byte = EEPROM.read(BUJIA_ADDR);
	if (byte  == 0xFF){
		writeEEPROM_bujia(DEFAULT_BUJIA);
		return DEFAULT_BUJIA;
	} 
	return byte;
}

uint16_t getEEPROM_id(){
	uint32_t ucByte;
	uint16_t val = 0;

	ucByte = EEPROM.read(ID_ADDR);
	val    = ucByte;
	ucByte = EEPROM.read(ID_ADDR + 1);
	val    |= ucByte << 8;

	if (val == 0xFFFF){ 
		writeEEPROM_id(DEFAULT_ID);
		return DEFAULT_ID;
	}
	return val;
}

uint32_t getEEPROM_offset(){
	uint32_t ucByte;
	uint32_t val = 0;

	ucByte = EEPROM.read(OFFSET_ADDR);
	val    = ucByte;
	ucByte = EEPROM.read(OFFSET_ADDR + 1);
	val    |= ucByte << 8;
	ucByte = EEPROM.read(OFFSET_ADDR + 2);
	val    |= ucByte << 16;
	ucByte = EEPROM.read(OFFSET_ADDR + 3);
	val    |= ucByte << 24;

	if (val == 0xFFFFFFFF){ 
		writeEEPROM_offset(DEFAULT_OFFSET);
		return DEFAULT_OFFSET;
	}
	return val;
}

float getEEPROM_slope(){
	uint32_t ucByte;
	uint32_t val = 0;

	ucByte = EEPROM.read(SLOPE_ADDR);
	val    = ucByte;
	ucByte = EEPROM.read(SLOPE_ADDR + 1);
	val    |= ucByte << 8;
	ucByte = EEPROM.read(SLOPE_ADDR + 2);
	val    |= ucByte << 16;
	ucByte = EEPROM.read(SLOPE_ADDR + 3);
	val    |= ucByte << 24;

	if (val == 0xFFFFFFFF){ 
		writeEEPROM_slope(DEFAULT_SLOPE);
		return DEFAULT_SLOPE;
	}


	return *(float*)&val;
}

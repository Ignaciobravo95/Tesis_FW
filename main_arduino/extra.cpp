/************************************************
 *   		INCLUDES
 ***********************************************/
#include "extra.h"

/************************************************
 *			GLOBAL VARIABLE SECTION
 ***********************************************/
uint32_t global_val[5];



 /************************************************
 *   		PRIVATE FUNCTION DEFINITION
 ***********************************************/



 /************************************************
 *   		FUNCTIONS DEFINITION
 ***********************************************/
 // Chequea la cantidad de digitos del valor x (maximo 8 digitos)
uint8_t checknDigits(uint16_t x){
    int8_t i = 8;    
    uint16_t aux;
    while ( i >= 0){
    	aux = x / pow_10_lut[i];
        if ( aux > 0  &&  aux < 10 ){
            return (i + 1);
        }
        i --;
    }
    return 0;
}

// Chequea el valor del digito 'dig' en x
uint8_t getDigitValue(uint16_t x, uint8_t dig){

	return ((x / pow_10_lut[dig])  % 10);
}

// Modifica el valor del digito 'dig' a 'digitValue' de x
uint16_t modValueDigit(uint16_t x, uint8_t dig, uint8_t digitValue){
	x = x - getDigitValue(x, dig)*pow_10_lut[dig];
	x = x + digitValue*pow_10_lut[dig];

	return x;
}

// Determina el valor maximo del digito basado en el valor actual 
// de los demas digitos y el valor maximo permitido
uint8_t getDigitMax(uint16_t max, uint16_t x, uint8_t numdig, uint8_t dig){
	int8_t i;
	uint16_t maxdigVal, xdigVal;

	numdig = numdig - 1;
	for (i = numdig; i >= 0; i--){
		maxdigVal 	= getDigitValue(max, i);
		xdigVal 	= getDigitValue(x, i);

		if ( i > dig){
			if (xdigVal < maxdigVal){
				return 10;
			}
			else if (xdigVal > maxdigVal){
				return 30; // error no deberia suceder nunca
			}
		}
		else if ( i < dig){
			if (xdigVal < maxdigVal){
				return (getDigitValue(max, dig) + 1);
			}
			else if (xdigVal > maxdigVal){
				return getDigitValue(max, dig);
			}	
			else{
				if ( i == 0){
					if (xdigVal <= maxdigVal){
						return (getDigitValue(max, dig) + 1);
					}
					else if (xdigVal > maxdigVal){
						return getDigitValue(max, dig);
					}	
				}
			}
		}
		else{
			if ( dig == 0 ){
				return (getDigitValue(max, dig) + 1);
			}
		}
	}
  return 50;
}

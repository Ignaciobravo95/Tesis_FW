//en este ejemplo de la librería "separador" le pasamos por serial
//tres elementos separados por coma y nos devuelve dichos elementos separados
// te esperamos en el canal
// PABLO - IOTICOS : https://www.youtube.com/channel/UCqxBxJnwt2JTwPM2c0kx0Yw
// Espero te sirva!!! Saludos!
//

#include <Separador.h>

Separador s;

void setup() {

  //inicio serial.
  Serial.begin(115200);
  Serial.println("ingresa un texto de 3 strings separados por coma ejemplo: hola,como,estas \n y si todo va bien la libreria debera separarlos" );

}


void loop() {

  if (Serial.available()) {
    serialEvent();
  }

}

void serialEvent() {

  String datosrecibidos = Serial.readString();
//hola,como,estas
// 0  ,  1 ,  2
  //separo datos recibidos
  // a la funciòn separa debo pasarle el string que quiero separar, el separador y el indice.
  String elemento1 = s.separa(datosrecibidos, ',', 0);
  String elemento2 = s.separa(datosrecibidos, ',', 1);
  String elemento3 = s.separa(datosrecibidos, ',', 2);

  Serial.println("El elemento 1 es: " + elemento1);
  Serial.println("El elemento 2 es: " + elemento2);
  Serial.println("El elemento 3 es: " + elemento3);
  Serial.println();
  Serial.println();
  Serial.println("ingresa un texto de 3 strings separados por coma ejemplo: hola,como,estas y si todo va bien la libreria debera separarlos" );

}


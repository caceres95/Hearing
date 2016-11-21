#include <SoftI2CMaster.h>
#include <SoftWire.h>

#include <LiquidCrystal.h>
#include <SoftwareSerial.h> //Librería que permite establecer comunicación serie en otros pins
#include <SPI.h>
#include <nRF24L01.h> //Librería conexión inalámbrica
#include <RF24.h> //Librería Radio Frecuencia
//Declaremos los pines CE y el CSN
#define CE_PIN 9
#define CSN_PIN 10

//Variable con la dirección del canal que se va a leer
byte direccion[5] = {'c', 'a', 'n', 'a', 'l'};

//creamos el objeto radio (NRF24L01)
RF24 radio(CE_PIN, CSN_PIN);

LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

//Hasta aquí hemos declarado la librería para la LCD y los pines por donde le va a entrar la información.

//Valida cuak posicion esta activa
bool validaPos[3] = {false, false, false};

int byteRead;

//vector para los datos recibidos
float datosIzq[3];
float datosDer[3];

void setup() {
  // Definimos la LCD con dimension 2x16 y definimos los caracteres que deben salir en las filas:
  lcd.begin(16, 2);

  //inicializamos el NRF24L01
  radio.begin();
  //inicializamos el puerto serie
  Serial.begin(9600);

  //Abrimos el canal de Lectura
  radio.openReadingPipe(1, direccion);

  //empezamos a escuchar por el canal
  radio.startListening();
}


void loop() {
/*
  int contador = 0;
  
  //Leer datos mano Derecha, conectada por cable y guardarlos en arreglo datosDer[]
  while (Serial.available() && contador < 3) {  /* read the most recent byte
    byteRead = Serial.read();     //now byteRead will have latest sensor
    // data sent from Arduino1
    datosDer[contador] = byteRead;
    contador = contador + 1;

  }
*/

  //Recibir datos de RC
  uint8_t numero_canal;

  if ( radio.available() ) {
    //Leer datos mano Izquierda, conectada inalambrica y guardarlos en arreglo datosIzq[]
    radio.read(datosIzq, sizeof(datosIzq));

    //Leer datos mano Derecha, conectada por cable y guardarlos en arreglo datosDer[]

    //reportamos por el puerto serial los datos recibidos de la mano Izquierda
    Serial.print(datosIzq[0]);
    Serial.print(", ");

    Serial.print(datosIzq[1]);
    Serial.print(", ");

    Serial.println(datosIzq[2]);

    //reportamos por el puerto serial los datos recibidos de la mano Derecha
    Serial.print(datosDer[0]);
    Serial.print(", ");

    Serial.print(datosDer[1]);
    Serial.print(", ");

    Serial.println(datosDer[2]);

  }
  else {
    Serial.println("No hay datos de radio disponibles");
  }

  interpretaDatos();

  delay(1000);
}

void interpretaDatos() {
  //Posicion Inicial, borra todo
  if (((datosIzq[0] < 1 && datosIzq[0] > -1) && (validaPos[0])) ||
      (((datosIzq[1] < 1 && datosIzq[1] > -1)) && (validaPos[1])) ||
      (((datosIzq[2] < 1 && datosIzq[2] > -1)) && (validaPos[2]))) {
    lcd.clear();
    Serial.println("Neutral");
  }

  //Mano Izquierda en posicion de Abajo, datosIzq[0] es positivo
  if (datosIzq[0] > 1) {
    lcd.setCursor(0, 0);
    lcd.print("Abajo");
    Serial.print("Abajo y ");

    //Esta en posicion Abajo/Arriba
    validaPos[0] = true;
    validaPos[1] = false;
    validaPos[2] = false;

    if (((datosDer[0] < 1 && datosDer[0] > -1)) || (((datosDer[1] > -1 && datosDer[1] < 1))) || (((datosDer[2] > -1 && datosDer[2] < 1)))) {
      lcd.clear();
      Serial.println("Neutral");
    }
    if (datosDer[0] > 1) {
      Serial.println("Abajo");
    }
    else if (datosDer[0] < -1) {
      Serial.println("Arriba");
    }
    else if (datosDer[1] > 1) {
      Serial.println("GiraIzq");
    }
    else if (datosDer[1] < -1) {
      Serial.println("GiraDer");
    }
    else if (datosDer[2] > 1) {
      Serial.println("Izq");
    }
    else if (datosDer[2] < -1) {
      Serial.println("Der");
    }
  }
  //Mano Izquierda en posicion de Arriba, datosIzq[0] es negativo
  else if (datosIzq[0] < -1)
  {
    lcd.setCursor(0, 0);
    lcd.print("Arriba");
    Serial.print("Arriba y ");

    //Esta en posicion Abajo/Arriba
    validaPos[0] = true;
    validaPos[1] = false;
    validaPos[2] = false;

    if (((datosDer[0] < 1 && datosDer[0] > -1)) || (((datosDer[1] > -1 && datosDer[1] < 1))) || (((datosDer[2] > -1 && datosDer[2] < 1)))) {
      lcd.clear();
      Serial.println("Neutral");
    }
    if (datosDer[0] > 1) {
      Serial.println("Abajo");
    }
    else if (datosDer[0] < -1) {
      Serial.println("Arriba");
    }
    else if (datosDer[1] > 1) {
      Serial.println("GiraIzq");
    }
    else if (datosDer[1] < -1) {
      Serial.println("GiraDer");
    }
    else if (datosDer[2] > 1) {
      Serial.println("Izq");
    }
    else if (datosDer[2] < -1) {
      Serial.println("Der");
    }
  }
  //Mano Izquierda en posicion de GiraIzq, datosIzq[1] es positivo
  else if (datosIzq[1] > 1)
  {
    lcd.setCursor(0, 0);
    lcd.print("GiraIzq");
    Serial.print("GiraIzq y ");

    //Esta en posicion GiraIzq/GiraDer
    validaPos[0] = false;
    validaPos[1] = true;
    validaPos[2] = false;

    if (((datosDer[0] < 1 && datosDer[0] > -1)) || (((datosDer[1] > -1 && datosDer[1] < 1))) || (((datosDer[2] > -1 && datosDer[2] < 1)))) {
      lcd.clear();
      Serial.println("Neutral y ");
    }
    if (datosDer[0] > 1) {
      Serial.println("Abajo");
    }
    else if (datosDer[0] < -1) {
      Serial.println("Arriba");
    }
    else if (datosDer[1] > 1) {
      Serial.println("GiraIzq");
    }
    else if (datosDer[1] < -1) {
      Serial.println("GiraDer");
    }
    else if (datosDer[2] > 1) {
      Serial.println("Izq");
    }
    else if (datosDer[2] < -1) {
      Serial.println("Der");
    }

  }
  //Mano Izquierda en posicion de GiraIzq, datosIzq[1] es negativo
  else if (datosIzq[1] < -1)
  {
    lcd.setCursor(0, 0);
    lcd.print("GiraDer");
    Serial.print("GiraDer y ");

    //Esta en posicion GiraIzq/GiraDer
    validaPos[0] = false;
    validaPos[1] = true;
    validaPos[2] = false;

    if (((datosDer[0] < 1 && datosDer[0] > -1)) || (((datosDer[1] > -1 && datosDer[1] < 1))) || (((datosDer[2] > -1 && datosDer[2] < 1)))) {
      lcd.clear();
      Serial.println("Neutral");
    }
    if (datosDer[0] > 1) {
      Serial.println("Abajo");
    }
    else if (datosDer[0] < -1) {
      Serial.println("Arriba");
    }
    else if (datosDer[1] > 1) {
      Serial.println("GiraIzq");
    }
    else if (datosDer[1] < -1) {
      Serial.println("GiraDer");
    }
    else if (datosDer[2] > 1) {
      Serial.println("Izq");
    }
    else if (datosDer[2] < -1) {
      Serial.println("Der");
    }
  }
  //Mano Izquierda en posicion de Izq, datosIzq[2] es positivo
  else if (datosIzq[2] > 1)
  {
    lcd.setCursor(0, 0);
    lcd.print("Izq");
    Serial.print("Izq y ");

    //Esta en posicion Izq/Der
    validaPos[0] = false;
    validaPos[1] = false;
    validaPos[2] = true;

    if (((datosDer[0] < 1 && datosDer[0] > -1)) || (((datosDer[1] > -1 && datosDer[1] < 1))) || (((datosDer[2] > -1 && datosDer[2] < 1)))) {
      lcd.clear();
      Serial.println("Neutral");
    }
    if (datosDer[0] > 1) {
      Serial.println("Abajo");
    }
    else if (datosDer[0] < -1) {
      Serial.println("Arriba");
    }
    else if (datosDer[1] > 1) {
      Serial.println("GiraIzq");
    }
    else if (datosDer[1] < -1) {
      Serial.println("GiraDer");
    }
    else if (datosDer[2] > 1) {
      Serial.println("Izq");
    }
    else if (datosDer[2] < -1) {
      Serial.println("Der");
    }

  }
  //Mano Izquierda en posicion de Der, datosIzq[2] es negativo
  else if (datosIzq[2] < -1)
  {
    lcd.setCursor(0, 0);
    lcd.print("Der");
    Serial.print("Der y ");

    //Esta en posicion Izq/Der
    validaPos[0] = false;
    validaPos[1] = false;
    validaPos[2] = true;

    if (((datosDer[0] < 1 && datosDer[0] > -1)) || (((datosDer[1] > -1 && datosDer[1] < 1))) || (((datosDer[2] > -1 && datosDer[2] < 1)))) {
      lcd.clear();
      Serial.println("Neutral");
    }
    if (datosDer[0] > 1) {
      Serial.println("Abajo");
    }
    else if (datosDer[0] < -1) {
      Serial.println("Arriba");
    }
    else if (datosDer[1] > 1) {
      Serial.println("GiraIzq");
    }
    else if (datosDer[1] < -1) {
      Serial.println("GiraDer");
    }
    else if (datosDer[2] > 1) {
      Serial.println("Izq");
    }
    else if (datosDer[2] < -1) {
      Serial.println("Der");
    }
  }
}


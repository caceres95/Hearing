#include <SoftwareSerial.h> //Librería que permite establecer comunicación serie en otros pins
#include "pitches.h"//Libreria de tonos
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

int melody[] = {
  NOTE_E7, NOTE_E7, 0, NOTE_E7,
  0, NOTE_C7, NOTE_E7, 0,
  NOTE_G7, 0, 0,  0,
  NOTE_G6, 0, 0, 0,

  NOTE_C7, 0, 0, NOTE_G6,
  0, 0, NOTE_E6, 0,
  0, NOTE_A6, 0, NOTE_B6,
  0, NOTE_AS6, NOTE_A6, 0,

  NOTE_G6, NOTE_E7, NOTE_G7,
  NOTE_A7, 0, NOTE_F7, NOTE_G7,
  0, NOTE_E7, 0, NOTE_C7,
  NOTE_D7, NOTE_B6, 0, 0,

  NOTE_C7, 0, 0, NOTE_G6,
  0, 0, NOTE_E6, 0,
  0, NOTE_A6, 0, NOTE_B6,
  0, NOTE_AS6, NOTE_A6, 0,

  NOTE_G6, NOTE_E7, NOTE_G7,
  NOTE_A7, 0, NOTE_F7, NOTE_G7,
  0, NOTE_E7, 0, NOTE_C7,
  NOTE_D7, NOTE_B6, 0, 0
}; //Notas que se van a cargar

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,

  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,

  9, 9, 9,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,

  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,

  9, 9, 9,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
};

int dato;

float accel;
float  peraccel;
float giron;
float girof;
float giro;

//vector para los datos recibidos
float datos[3];

void setup() {

  //inicializamos el NRF24L01
  radio.begin();
  //inicializamos el puerto serie
  Serial.begin(9600);

  //Abrimos el canal de Lectura
  radio.openReadingPipe(1, direccion);

  //empezamos a escuchar por el canal
  radio.startListening();

  pinMode(4, OUTPUT); //Declaramos nuestros pines como salidas

  pinMode(5, OUTPUT);

  pinMode(6, OUTPUT);

  pinMode(7, OUTPUT);

  Serial.begin(9600); //Velocidad de comunicacion serial

  for (int thisNote = 0; thisNote < 16; thisNote++)
  {

    int noteDuration = 1000 / noteDurations[thisNote];
    tone(3, melody[thisNote], noteDuration);

    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(3);
  }

}

void loop() {

  if (Serial.available()); { //si la comunicacion esta habilitada

    dato = Serial.read();  //asignar el valor recibido a la variable dato

    //Recibir datos de RC
    uint8_t numero_canal;
    //if ( radio.available(&numero_canal) )
    if ( radio.available() )
    {
      //Leemos los datos y los guardamos en la variable datos[]
      radio.read(datos, sizeof(datos));

      if(datos[0] <= -1.5)
        datos[0] = -1.5;

      if(1.5 <= datos[0])
        datos[0] = 1.5;

      if(datos[1] <= -1.5)
        datos[0] = -1.5;

      if(1.5 <= datos[1])
        datos[0] = 1.5;

      //reportamos por el puerto serial los datos recibidos
      //Serial.print("Dato0= " );
      Serial.print(datos[0]);
      Serial.print(", ");
      //Serial.print("Dato1= " );
      Serial.print(datos[1]);
      Serial.print(", ");
      //Serial.print("Dato2= " );
      Serial.println(datos[2]);
    }
    else
    {
      Serial.println("No hay datos de radio disponibles");
    }

    //4 y 5 llanta izquierda
    //  4- y 5+  avanza
    //  4+ y 5- retrocede
    //6 y 7 llanta derecha
    //  6- y 7+  avanza
    //  6+ y 7- retrocede

    if(datos[0] < 0)    //Carro Retrocede
    {
      if(datos[1] < 0)  //Derecha
      {
        accel = datos[0]*170*-1;
        peraccel = accel/255;
        giron = datos[1]/1.5*-1;
        girof = datos[2]/1.5*-1;
        giro = (peraccel*girof) + ((1-peraccel)*giron);
        
        analogWrite(6, accel);
        analogWrite(7, 0);
        analogWrite(4, accel*(1-giro));
        analogWrite(5, 0);

      }
      
      else              //Izquierda
      {
        accel = datos[0]*170*-1;
        peraccel = accel/255;
        giron = datos[1]/1.5;
        girof = datos[2]/1.5;
        giro = (peraccel*girof) + ((1-peraccel)*giron);
        
        analogWrite(6, accel*(1-giro));
        analogWrite(7, 0);
        analogWrite(4, accel);
        analogWrite(5, 0);
      }
    }

    else                //Carro avanza
    {
      if(datos[1] < 0)  //Derecha
      {
        accel = datos[0]*170;
        peraccel = accel/255;
        giron = datos[1]/1.5*-1;
        girof = datos[2]/1.5*-1;
        giro = (peraccel*girof) + ((1-peraccel)*giron);
        
        analogWrite(6, 0);
        analogWrite(7, accel);
        analogWrite(4, 0);
        analogWrite(5, accel*(1-giro));
      }
      
      else              //Izquierda
      {
        accel = datos[0]*170;
        peraccel = accel/255;
        giron = datos[1]/1.5;
        girof = datos[2]/1.5;
        giro = (peraccel*girof) + ((1-peraccel)*giron);
        
        analogWrite(6, 0);
        analogWrite(7, accel*(1-giro));
        analogWrite(4, 0);
        analogWrite(5, accel);
      }
    }

    //MUSICA
//    if (dato == 'M') {
//      for (int thisNote = 0; thisNote < 80; thisNote++)
//      {
//
//        int noteDuration = 1000 / noteDurations[thisNote];
//        tone(3, melody[thisNote], noteDuration);
//
//        int pauseBetweenNotes = noteDuration * 1.30;
//        delay(pauseBetweenNotes);
//        noTone(3);
//
//      }
//    }


    // ADELANTE

    if (dato == 'A') { // Este valor es el que estamos enviando desde nuestra aplicacion

      analogWrite(4, 0);

      analogWrite(5, 255);

      analogWrite(6, 0);

      analogWrite(7, 255);

    }

    //ATRAS

    if (dato == '2') {

      analogWrite(4, 255);

      analogWrite(5, 0);

      analogWrite(6, 255);

      analogWrite(7, 0);

    }

    //DERECHA

    if (dato == '3') {

      analogWrite(4, 255);

      analogWrite(5, 0);

      analogWrite(6, 0);

      analogWrite(7, 255);

    }

    //IZQUIERDA

    if (dato == '4') {

      analogWrite(4, 0);

      analogWrite(5, 255);

      analogWrite(6, 255);

      analogWrite(7, 0);

    }

    //PARO

    if (dato == '5') {

      analogWrite(4, 0);

      analogWrite(5, 0);

      analogWrite(6, 0);

      analogWrite(7, 0);

    }


  }

}

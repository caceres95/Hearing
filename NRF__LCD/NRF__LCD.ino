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

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

//Hasta aquí hemos declarado la librería para la LCD y los pines por donde le va a entrar la información.

int dato;

float accel;
float  peraccel;
float giron;
float girof;
float giro;

//vector para los datos recibidos
float datos[3];

void setup() {
  // Definimos la LCD con dimension 2x16 y definimos los caracteres que deben salir en las filas:
  lcd.begin(16,2);
  //lcd.print("C=      K=");
  //lcd.setCursor(0,1);
  //lcd.print("Temperatura");

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

  
  lcd.setCursor(2,0);
  lcd.print("Palabra");
  

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
   
   }
   
 }

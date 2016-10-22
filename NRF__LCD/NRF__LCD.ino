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

bool xy = true;
int dato;

//vector para los datos recibidos
float datos[3];

void setup() {
  // Definimos la LCD con dimension 2x16 y definimos los caracteres que deben salir en las filas:
  lcd.begin(16,2);

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

  if (Serial.available()); { //si la comunicacion esta habilitada

    dato = Serial.read();  //asignar el valor recibido a la variable dato

    //Recibir datos de RC
    uint8_t numero_canal;
    
    if ( radio.available() )
    {
      //Leemos los datos y los guardamos en la variable datos[]
      radio.read(datos, sizeof(datos));

      //reportamos por el puerto serial los datos recibidos
      Serial.print(datos[0]);
      Serial.print(", ");
      
      Serial.print(datos[1]);
      Serial.print(", ");
      
      Serial.println(datos[2]);
    }
    else
    {
      Serial.println("No hay datos de radio disponibles");
    }
    
    if(((datos[0] < 1 && datos[0] > -1) && (xy)) || ((datos[1] > -1 && datos[1] < 1)) && (!xy))
    {
      lcd.clear();
    }

    if(datos[0] > 1)    
    {
      lcd.setCursor(0,0);
      lcd.print("me");
      xy = true;
    }
    else if(datos[0] < -1)
    {
      lcd.setCursor(0,0);
      lcd.print("Hola");
      xy = true;
    }
    else if(datos[1] < -1)
    {
      lcd.setCursor(0,0);
      lcd.print("llamo");
      xy = false;
    }
    else if(datos[1] > 1)
    {
      lcd.setCursor(0,0);
      lcd.print("Castro");
      xy = false;
    }
   } 
   
   delay(100);
   
 }

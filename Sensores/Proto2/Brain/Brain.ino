#include <LiquidCrystal.h>
#include <Wire.h>
#include <SoftwareSerial.h> //Librería que permite establecer comunicación serie en otros pins
#include <SPI.h>
#include <nRF24L01.h> //Librería conexión inalámbrica
#include <RF24.h> //Librería Radio Frecuencia
//Declaremos los pines CE y el CSN
#define CE_PIN 9
#define CSN_PIN 10

char pos;

// Maximum number of substrings expected
const int MAX_SUBSTRINGS = 3;
// Array of pointers to each substring after displayString() has been called
static char* substrings[MAX_SUBSTRINGS];

//Variable con la dirección del canal que se va a leer
byte direccion[5] = {'c', 'a', 'n', 'a', 'l'};

//creamos el objeto radio (NRF24L01)
RF24 radio(CE_PIN, CSN_PIN);

LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

//Hasta aquí hemos declarado la librería para la LCD y los pines por donde le va a entrar la información.

//Valida cuak posicion esta activa
bool validaPos[3] = {false, false, false};
bool validaPosDerecha[3] = {false, false, false};

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

  //Wired Setup!!
  Wire.begin(16);                // join i2c bus with address #8
  Wire.onReceive(receiveEvent); // register event
}

void loop() {
  delay(1000);
}

void radioDatos() {

  uint8_t numero_canal;
    //Leer datos mano Izquierda, conectada inalambrica y guardarlos en arreglo datosIzq[]
    radio.read(datosIzq, sizeof(datosIzq));
    
    //Leer datos mano Derecha, conectada por cable y guardarlos en arreglo datosDer[]
    //Serial.println("++++++++++++++++++++++ Inalambrico");
    //reportamos por el puerto serial los datos recibidos de la mano Izquierda
//    Serial.print(datosIzq[0]);
//    Serial.print(", ");
//
//    Serial.print(datosIzq[1]);
//    Serial.print(", ");
//
//    Serial.println(datosIzq[2]);
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany) {

  delay(1000);
  String arrValores = "";
  int i = 0;
  while (0 < Wire.available()) { // loop through all but the last
    char c = Wire.read(); // receive byte as a character
    arrValores.concat(c);
    i++;
  }
  
  char arrnuevo[i + 1];

  arrValores.toCharArray(arrnuevo, i + 1);

  displayString(arrnuevo);

  float x = atof(substrings[0]);
  float y = atof(substrings[1]);
  float z = atof(substrings[2]);

  datosDer[0] = x;
  datosDer[1] = y;
  datosDer[2] = z;

//  Serial.println("-------------- Alambrico");
//  Serial.print(x);
//  Serial.print(", ");
//  Serial.print(y);
//  Serial.print(", ");
//  Serial.print(z);
//  Serial.println();

  radioDatos();
  interpretaDatos();
  
  delay(1000);
}


void displayString(char* s) {
  // First clear the array of substrings
  for (int i = 0; i < MAX_SUBSTRINGS; i++)
    substrings[i] = 0;
  // Now split the input string
  char* text = strtok(s, ",");
  int i = 0;
  while (text != 0 && i < MAX_SUBSTRINGS) {
    // A toekn was found: append it to the array of substrings
    substrings[i++] = text;
    text = strtok(0, ",");
  }
}

//---------------------------------------------------------------------------------------------------------------------------
void interpretaDatos() {
  //Posicion Inicial Mano Izq (inalambrica) en neutral, borra todo
  if (((datosIzq[0] < 1 && datosIzq[0] > -1) && !(validaPos[0])) &&
      ((datosIzq[1] < 1 && datosIzq[1] > -1) && !(validaPos[1])) &&
      ((datosIzq[2] < 1 && datosIzq[2] > -1) && !(validaPos[2]))) {
    //Serial.print("Neutral y ");
    
    interpretaManoDerecha(0);
  }

  //Mano Izquierda en posicion de Abajo, datosIzq[0] es positivo
  else if (datosIzq[0] > 1) {
    //Serial.print("Abajo y ");

    //Esta en posicion Abajo/Arriba
    validaPos[0] = true;
    validaPos[1] = false;
    validaPos[2] = false;

    interpretaManoDerecha(1);
  }
  //Mano Izquierda en posicion de Arriba, datosIzq[0] es negativo
  else if (datosIzq[0] < -1)
  {
    //Serial.print("Arriba y ");

    //Esta en posicion Abajo/Arriba
    validaPos[0] = true;
    validaPos[1] = false;
    validaPos[2] = false;

    interpretaManoDerecha(2);
  }
  //Mano Izquierda en posicion de GiraIzq, datosIzq[1] es positivo
  else if (datosIzq[1] > 1)
  {
    //Serial.print("GiraIzq y ");

    //Esta en posicion GiraIzq/GiraDer
    validaPos[0] = false;
    validaPos[1] = true;
    validaPos[2] = false;

    interpretaManoDerecha(3);

  }
  //Mano Izquierda en posicion de GiraIzq, datosIzq[1] es negativo
  else if (datosIzq[1] < -1)
  {
    //Serial.print("GiraDer y ");

    //Esta en posicion GiraIzq/GiraDer
    validaPos[0] = false;
    validaPos[1] = true;
    validaPos[2] = false;

    interpretaManoDerecha(4);
  }
  //Mano Izquierda en posicion de Izq, datosIzq[2] es positivo
  else if (datosIzq[2] > 1)
  {
    //Serial.print("Izq y ");

    //Esta en posicion Izq/Der
    validaPos[0] = false;
    validaPos[1] = false;
    validaPos[2] = true;

    interpretaManoDerecha(5);

  }
  //Mano Izquierda en posicion de Der, datosIzq[2] es negativo
  else if (datosIzq[2] < -1)
  {
    //Serial.print("Der y ");

    //Esta en posicion Izq/Der
    validaPos[0] = false;
    validaPos[1] = false;
    validaPos[2] = true;

    interpretaManoDerecha(6);
  }
  else {
    validaPos[0] = false;
    validaPos[1] = false;
    validaPos[2] = false;
  }
}
//---------------------------------------------------------------------------------------------------------------------------

void interpretaManoDerecha(int izq) {
   if (((datosDer[0] < 1 && datosDer[0] > -1) && !(validaPosDerecha[0])) && 
   ((datosDer[1] > -1 && datosDer[1] < 1) && !(validaPosDerecha[1])) && 
   ((datosDer[2] > -1 && datosDer[2] < 1) && !(validaPosDerecha[2]))) 
   {
      //Serial.println("Neutral");
      if(izq == 0 && pos != 'H')
      {
        pos = 'H';
        Serial.print(pos);
      }
      else if(izq == 1 && pos != 'I')
      {
        pos = 'I';
        Serial.print(pos);
      }
      else if(izq == 2 && pos != 'J')
      {
        pos = 'J';
        Serial.print(pos);
      }
      else if(izq == 3 && pos != 'K')
      {
        pos = 'K';
        Serial.print(pos);
      }
      else if(izq == 4 && pos != 'L')
      {
        pos = 'L';
        Serial.print(pos);
      }
      else if(izq == 5 && pos != 'M')
      {
        pos = 'M';
        Serial.print(pos);
      }
      else if(izq == 6 && pos != 'N')
      {
        pos = 'N';
        Serial.print(pos);
      }
    }
   else if (datosDer[0] > 1  ) {
      //Serial.println("Abajo");
      validaPosDerecha[0] = true;
      validaPosDerecha[1] = false;
      validaPosDerecha[2] = false;
      
      if(izq == 0 && pos != '0')
      {
        pos = '0';
        Serial.print(pos);
      }
      else if(izq == 1 && pos != '1')
      {
        pos = '1';
        Serial.print(pos);
      }
      else if(izq == 2 && pos != '2')
      {
        pos = '2';
        Serial.print(pos);
      }
      else if(izq == 3 && pos != '3')
      {
        pos = '3';
        Serial.print(pos);
      }
      else if(izq == 4 && pos != '4')
      {
        pos = '4';
        Serial.print(pos);
      }
      else if(izq == 5 && pos != '5')
      {
        pos = '5';
        Serial.print(pos);
      }
      else if(izq == 6 && pos != '6')
      {
        pos = '6';
        Serial.print(pos);
      }

    }
    else if (datosDer[0] < -1) 
    {
      //Serial.println("Arriba");
      validaPosDerecha[0] = true;
      validaPosDerecha[1] = false;
      validaPosDerecha[2] = false;

      if(izq == 0 && pos != '7')
      {
        pos = '7';
        Serial.print(pos);
      }
      else if(izq == 1 && pos != '8')
      {
        pos = '8';
        Serial.print(pos);
      }
      else if(izq == 2 && pos != '9')
      {
        pos = '9';
        Serial.print(pos);
      }
      else if(izq == 3 && pos != 'a')
      {
        pos = 'a';
        Serial.print(pos);
      }
      else if(izq == 4 && pos != 'b')
      {
        pos = 'b';
        Serial.print(pos);
      }
      else if(izq == 5 && pos != 'c')
      {
        pos = 'c';
        Serial.print(pos);
      }
      else if(izq == 6 && pos != 'd')
      {
        pos = 'd';
        Serial.print(pos);
      }

    }
    else if (datosDer[1] > 1) {
      //Serial.println("GiraIzq");
      validaPosDerecha[0] = false;
      validaPosDerecha[1] = true;
      validaPosDerecha[2] = false;

      if(izq == 0 && pos != 'e')
      {
        pos = 'e';
        Serial.print(pos);
      }
      else if(izq == 1 && pos != 'f')
      {
        pos = 'f';
        Serial.print(pos);
      }
      else if(izq == 2 && pos != 'g')
      {
        pos = 'g';
        Serial.print(pos);
      }
      else if(izq == 3 && pos != 'h')
      {
        pos = 'h';
        Serial.print(pos);
      }
      else if(izq == 4 && pos != 'i')
      {
        pos = 'i';
        Serial.print(pos);
      }
      else if(izq == 5 && pos != 'j')
      {
        pos = 'j';
        Serial.print(pos);
      }
      else if(izq == 6 && pos != 'k')
      {
        pos = 'k';
        Serial.print(pos);
      }

    }
    else if (datosDer[1] < -1) {
      //Serial.println("GiraDer");
      validaPosDerecha[0] = false;
      validaPosDerecha[1] = true;
      validaPosDerecha[2] = false;

      if(izq == 0 && pos != 'm')
      {
        pos = 'm';
        Serial.print(pos);
      }
      else if(izq == 1 && pos != 'n')
      {
        pos = 'n';
        Serial.print(pos);
      }
      else if(izq == 2 && pos != 'o')
      {
        pos = 'o';
        Serial.print(pos);
      }
      else if(izq == 3 && pos != 'p')
      {
        pos = 'p';
        Serial.print(pos);
      }
      else if(izq == 4 && pos != 'q')
      {
        pos = 'q';
        Serial.print(pos);
      }
      else if(izq == 5 && pos != 'r')
      {
        pos = 'r';
        Serial.print(pos);
      }
      else if(izq == 6 && pos != 's')
      {
        pos = 's';
        Serial.print(pos);
      }

    }
    else if (datosDer[2] > 1) {
      //Serial.println("Izqui");
      validaPosDerecha[0] = false;
      validaPosDerecha[1] = false;
      validaPosDerecha[2] = true;

      if(izq == 0 && pos != 't')
      {
        pos = 't';
        Serial.print(pos);
      }
      else if(izq == 1 && pos != 'u')
      {
        pos = 'u';
        Serial.print(pos);
      }
      else if(izq == 2 && pos != 'v')
      {
        pos = 'v';
        Serial.print(pos);
      }
      else if(izq == 3 && pos != 'w')
      {
        pos = 'w';
        Serial.print(pos);
      }
      else if(izq == 4 && pos != 'x')
      {
        pos = 'x';
        Serial.print(pos);
      }
      else if(izq == 5 && pos != 'y')
      {
        pos = 'y';
        Serial.print(pos);
      }
      else if(izq == 6 && pos != 'z')
      {
        pos = 'z';
        Serial.print(pos);
      }

    }
    else if (datosDer[2] < -1) {
      //Serial.println("Der");
      validaPosDerecha[0] = false;
      validaPosDerecha[1] = false;
      validaPosDerecha[2] = true;

      if(izq == 0 && pos != 'A')
      {
        pos = 'A';
        Serial.print(pos);
      }
      else if(izq == 1 && pos != 'B')
      {
        pos = 'B';
        Serial.print(pos);
      }
      else if(izq == 2 && pos != 'C')
      {
        pos = 'C';
        Serial.print(pos);
      }
      else if(izq == 3 && pos != 'D')
      {
        pos = 'D';
        Serial.print(pos);
      }
      else if(izq == 4 && pos != 'E')
      {
        pos = 'E';
        Serial.print(pos);
      }
      else if(izq == 5 && pos != 'F')
      {
        pos = 'F';
        Serial.print(pos);
      }
      else if(izq == 6 && pos != 'G')
      {
        pos = 'G';
        Serial.print(pos);
      }

    }
    else {
      validaPosDerecha[0] = false;
      validaPosDerecha[1] = false;
      validaPosDerecha[2] = false;
      
    }
}


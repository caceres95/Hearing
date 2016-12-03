#include <LiquidCrystal.h>
#include <Wire.h>
#include <SoftwareSerial.h> //Librería que permite establecer comunicación serie en otros pins
#include <SPI.h>
#include <nRF24L01.h> //Librería conexión inalámbrica
#include <RF24.h> //Librería Radio Frecuencia
//Declaremos los pines CE y el CSN
#define CE_PIN 9
#define CSN_PIN 10

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
    Serial.println("++++++++++++++++++++++ Inalambrico");
    //reportamos por el puerto serial los datos recibidos de la mano Izquierda
    Serial.print(datosIzq[0]);
    Serial.print(", ");

    Serial.print(datosIzq[1]);
    Serial.print(", ");

    Serial.println(datosIzq[2]);
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

  Serial.println("-------------- Alambrico");
  Serial.print(x);
  Serial.print(", ");
  Serial.print(y);
  Serial.print(", ");
  Serial.print(z);
  Serial.println();

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


void interpretaDatos() {
  //Posicion Inicial Mano Izq (inalambrica) en neutral, borra todo
  if (((datosIzq[0] < 1 && datosIzq[0] > -1) && !(validaPos[0])) &&
      ((datosIzq[1] < 1 && datosIzq[1] > -1) && !(validaPos[1])) &&
      ((datosIzq[2] < 1 && datosIzq[2] > -1) && !(validaPos[2]))) {
    lcd.clear();
    Serial.print("Neutral y ");
    
    interpretaManoDerecha();
  }

  //Mano Izquierda en posicion de Abajo, datosIzq[0] es positivo
  else if (datosIzq[0] > 1) {
    lcd.setCursor(0, 0);
    lcd.print("Abajo");
    Serial.print("Abajo y ");

    //Esta en posicion Abajo/Arriba
    validaPos[0] = true;
    validaPos[1] = false;
    validaPos[2] = false;

    interpretaManoDerecha();
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

    interpretaManoDerecha();
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

    interpretaManoDerecha();

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

    interpretaManoDerecha();
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

    interpretaManoDerecha();

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

    interpretaManoDerecha();
  }
  else {
    validaPos[0] = false;
    validaPos[1] = false;
    validaPos[2] = false;
  }
}

void interpretaManoDerecha() {
   if (((datosDer[0] < 1 && datosDer[0] > -1) && !(validaPosDerecha[0])) && 
   ((datosDer[1] > -1 && datosDer[1] < 1) && !(validaPosDerecha[1])) && 
   ((datosDer[2] > -1 && datosDer[2] < 1) && !(validaPosDerecha[2]))) 
   {
      lcd.clear();
      Serial.println("Neutral");
    }
   else if (datosDer[0] > 1  ) {
      Serial.println("Abajo");
      validaPosDerecha[0] = true;
      validaPosDerecha[1] = false;
      validaPosDerecha[2] = false;
    }
    else if (datosDer[0] < -1) 
    {
      Serial.println("Arriba");
      validaPosDerecha[0] = true;
      validaPosDerecha[1] = false;
      validaPosDerecha[2] = false;
    }
    else if (datosDer[1] > 1) {
      Serial.println("GiraIzq");
      validaPosDerecha[0] = false;
      validaPosDerecha[1] = true;
      validaPosDerecha[2] = false;
    }
    else if (datosDer[1] < -1) {
      Serial.println("GiraDer");
      validaPosDerecha[0] = false;
      validaPosDerecha[1] = true;
      validaPosDerecha[2] = false;
    }
    else if (datosDer[2] > 1) {
      Serial.println("Izqui");
      validaPosDerecha[0] = false;
      validaPosDerecha[1] = false;
      validaPosDerecha[2] = true;
    }
    else if (datosDer[2] < -1) {
      Serial.println("Der");
      validaPosDerecha[0] = false;
      validaPosDerecha[1] = false;
      validaPosDerecha[2] = true;

    }
    else {
      validaPosDerecha[0] = false;
      validaPosDerecha[1] = false;
      validaPosDerecha[2] = false;
      
    }
}


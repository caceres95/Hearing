// Wire Slave Receiver
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Receives data as an I2C/TWI slave device
// Refer to the "Wire Master Writer" example for use with this

// Created 29 March 2006

// This example code is in the public domain.


#include <Wire.h>
  // Maximum number of substrings expected
const int MAX_SUBSTRINGS = 3;
// Array of pointers to each substring after displayString() has been called
static char* substrings[MAX_SUBSTRINGS];


void setup() {
  Wire.begin(16);                // join i2c bus with address #8
  Wire.onReceive(receiveEvent); // register event
  Serial.begin(9600);           // start serial for output
}

void loop() {
  delay(100);
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany) {
  String arrValores = "";
  int i = 0;
  while (0 < Wire.available()) { // loop through all but the last
    char c = Wire.read(); // receive byte as a character    
    arrValores.concat(c);
    i++;
    //Serial.print(c);         // print the character
  }
  

  char arrnuevo[i+1];
  
  arrValores.toCharArray(arrnuevo, i+1);

  displayString(arrnuevo);
  
  float x = atof(substrings[0]);
  float y = atof(substrings[1]);
  float z = atof(substrings[2]);

  Serial.print(x);
  Serial.print(", ");
  Serial.print(y);
  Serial.print(", ");
  Serial.print(z);
  Serial.println();
}

void displayString(char* s) {
    // First clear the array of substrings
    for (int i = 0; i < MAX_SUBSTRINGS; i++)
        substrings[i] = 0;
    // Now split the input string
    char* text = strtok(s,",");
    int i = 0;
    while (text != 0 && i < MAX_SUBSTRINGS) {
        // A toekn was found: append it to the array of substrings
        substrings[i++] = text;
        text = strtok(0,",");
    }
}

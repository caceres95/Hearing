#include "SD.h"
#define SD_ChipSelectPin 6
#include "TMRpcm.h"
#include "SPI.h"

TMRpcm tmrpcm;
int incomingByte;      // a variable to read incoming serial data into

void setup(){
tmrpcm.speakerPin = 9;
Serial.begin(9600);
if (!SD.begin(SD_ChipSelectPin)) {
Serial.println("SD fail");
return;
}
else
{
  Serial.println("SD success");
}

tmrpcm.setVolume(6);
}

void loop(){
   if (Serial.available() > 0) {
    // read the oldest byte in the serial buffer:
    incomingByte = Serial.read();

    switch(incomingByte)
    {
      case '1':
        //tmrpcm.play(".wav");
        break;
      case '2':
        tmrpcm.play("19.wav");
        break;
      case '3':
        tmrpcm.play("14.wav");
        break;
      case '4':
        //tmrpcm.play(".wav");
        break;
      case '5':
        tmrpcm.play("15.wav");
        break;
      case '6':
        //tmrpcm.play(".wav");
        break;
      case '7':
        //tmrpcm.play(".wav");
        break;
      case '8':
        tmrpcm.play("1.wav");
        break;
      case '9':
        tmrpcm.play("8.wav");
        break;
      case 'a':
        //tmrpcm.play(".wav");
        break;
      case 'b':
        //tmrpcm.play(".wav");
        break;
      case 'c':
        //tmrpcm.play(".wav");
        break;
      case 'd':
        tmrpcm.play("10.wav");
        break;
      case 'e':
        //tmrpcm.play(".wav");
        break;
      case 'f':
        //tmrpcm.play(".wav");
        break;
      case 'g':
        //tmrpcm.play(".wav");
        break;
      case 'h':
        tmrpcm.play("20.wav");
        break;
      case 'i':
        tmrpcm.play("9.wav");
        break;
      case 'j':
        //tmrpcm.play(".wav");
        break;
      case 'k':
        //tmrpcm.play(".wav");
        break;
      case 'l':
        //tmrpcm.play(".wav");
        break;
      case 'm':
        tmrpcm.play("2.wav");
        break;
      case 'n':
        tmrpcm.play("11.wav");
        break;
      case 'o':
        tmrpcm.play("12.wav");
        break;
      case 'p':
        tmrpcm.play("3.wav");
        break;
      case 'q':
        tmrpcm.play("23.wav");
        break;
      case 'r':
        //tmrpcm.play(".wav");
        break;
      case 's':
        //tmrpcm.play(".wav");
        break;
      case 't':
        tmrpcm.play("5.wav");
        break;
      case 'u':
        tmrpcm.play("24.wav");
        break;
      case 'v':
        //tmrpcm.play(".wav");
        break;
      case 'w':
        tmrpcm.play("4.wav");
        break;
      case 'x':
        //tmrpcm.play(".wav");
        break;
      case 'y':
        tmrpcm.play("18.wav");
        break;
      case 'z':
        tmrpcm.play("13.wav");
        break;
      case 'A':
        //tmrpcm.play(".wav");
        break;
      case 'B':
        //tmrpcm.play(".wav");
        break;
      case 'C':
        tmrpcm.play("22.wav");
        break;
      case 'D':
        //tmrpcm.play(".wav");
        break;
      case 'E':
        //tmrpcm.play(".wav");
        break;
      case 'F':
        tmrpcm.play("21.wav");
        break;
      case 'G':
        tmrpcm.play("16.wav");
        break;
      case 'H':
        //tmrpcm.play(".wav");
        break;
      case 'I':
        //tmrpcm.play(".wav");
        break;
      case 'J':
        tmrpcm.play("6.wav");
        break;
      case 'K':
        tmrpcm.play("7.wav");
        break;
      case 'M':
        tmrpcm.play("17.wav");
        break;
      case 'N':
        //tmrpcm.play(".wav");
        break;
    }
  }
  }z

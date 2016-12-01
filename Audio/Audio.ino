#include "SD.h"
#define SD_ChipSelectPin 6
#include "TMRpcm.h"
#include "SPI.h"

TMRpcm tmrpcm;

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
tmrpcm.play("prueba2.wav");
}

void loop(){  }

#include <SDS011.h>
#include <SD.h>
#include <SPI.h>
#include<dht.h>
#include "MQ131.h"
#define DHT11_PIN 2
dht DHT; 
MQ131 sensor(2,A0, LOW_CONCENTRATION, 10000);
const int VOCSensor = 5;
float VOC;
unsigned long starttime;
float p10,p25;
int error;
SDS011 my_sds;


File myFile; 
int pinCS = 4;

void setup() {
  Serial.begin(9600);
  my_sds.begin(0,1);
  pinMode(pinCS, OUTPUT);
  if (SD.begin()) {
    Serial.println("SD card is ready to use.");
  } else {
    Serial.println("SD card initialization failed");
    return;
  }
  sensor.calibrate();
  Serial.print("R0 = ");
  Serial.print(sensor.getR0());
  Serial.println(" Ohms");
}

void loop() {
  sensor.begin();
  DHT.read11(DHT11_PIN);
  Serial.print(DHT.temperature);
  Serial.println(" C");
  Serial.print(", ");
  Serial.print(DHT.humidity);
  Serial.print(" %");
  Serial.println(", ");
  int VOC = analogRead(VOCSensor);
  Serial.print(VOC);
  Serial.print(" ug/m3");
  Serial.println(", ");
  error = my_sds.read(&p25,&p10);
  if(! error) {
    Serial.println("PM2.5: "+String(p25) + " ug/m3");
    Serial.println(",PM10: "+String(p10) + " ug/m3");
Serial.print(", ");
Serial.print(sensor.getO3(PPB)*3.25);
Serial.println(" ppb");


  } 
myFile = SD.open("data.txt", FILE_WRITE);
if (myFile) {
  myFile.print(starttime);
  myFile.print(",");
  myFile.print(DHT.temperature);
  myFile.print("C. ");
  myFile.print(",");
  myFile.print(DHT.humidity);
  myFile.print("%. ");
  myFile.print(",");
  myFile.print(VOC);
  myFile.print("ug/m3. ");
  myFile.print(",");
  myFile.print(p25);
  myFile.print("ug/m3. ");
  myFile.print(",");
  myFile.print(p10);
  myFile.print("ug/m3. ");
  myFile.print(",");
  myFile.print(sensor.getO3(PPB)*3.25);
  myFile.println("ppb. ");
  myFile.close();
} else { 
  Serial.println("error opening data.txt");
  }
delay(60000);
}

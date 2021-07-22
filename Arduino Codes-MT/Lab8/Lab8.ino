
/*
  created by K-Lab 21/11/2016
  Lab no 5
  
*/
#include <dht.h>

int DHT11pin = 3;

dht DHT;

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(9600);
  Serial.println("DHT TEST PROGRAM ");
  Serial.println("Type,\tHumidity (%),\tTemperature (C)");
  delay(1000);   
}

// the loop function runs over and over again forever
void loop() {

  // READ DATA
  Serial.print("DHT11, \t");
  int chk = DHT.read11(DHT11pin);
  Serial.print(DHT.humidity, 1);
  Serial.print(",\t");
  Serial.println(DHT.temperature, 1);

  delay(2000);
}

/*
  created by Kyi Zaw Win 24/12/2020
  Lab no 11  
*/
#include "LIS3DHTR.h"
#include <Wire.h>
LIS3DHTR<TwoWire> LIS; //IIC
#define WIRE Wire
 
void setup()
{
  Serial.begin(9600);
  LIS.begin(WIRE,0x19); //IIC init
  LIS.openTemp();  //If ADC3 is used, the temperature detection needs to be turned off.
  delay(100);
  LIS.setFullScaleRange(LIS3DHTR_RANGE_2G);
  LIS.setOutputDataRate(LIS3DHTR_DATARATE_50HZ);
}
void loop()
{
  if (!LIS)
  {
    Serial.println("LIS3DHTR didn't connect.");
    while (1)
      ;
    return;
  }
  //3 axis
    Serial.print("x:"); Serial.print(LIS.getAccelerationX()); Serial.print("  ");
    Serial.print("y:"); Serial.print(LIS.getAccelerationY()); Serial.print("  ");
    Serial.print("z:"); Serial.println(LIS.getAccelerationZ());
 
  //temperature
  //Serial.print("temp:");
  //Serial.println(LIS.getTemperature());
  delay(200);
}

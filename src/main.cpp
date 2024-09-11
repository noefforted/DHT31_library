#include <Arduino.h>
#include <Wire.h>
#include <SHT31.h>
#include <SPI.h>

SHT31 sht31(0x44);

void setup()
{
  Serial.begin(9600);
  sht31.begin();
}

void loop()
{
  float temp = sht31.readTemp();
  float humid = sht31.readHumid();
  Serial.print("Suhu: "); Serial.print(temp); Serial.print(" °C || ");
  Serial.print(" Kelembapan: ") ; Serial.print(humid); Serial.println(" RH");
  delay(200);
}
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

Adafruit_BME280 bme; // I2C 

#define SEALEVELPRESSURE_HPA (1026.52)

bool BME280_Init(){
  Serial.println("Start inicialize BME280");
  Wire.begin(D3, D4); //SDA - D2, SDL - D1 
  //Wire.setClock(100000);
  delay(1000); 
  if (!bme.begin(0x76)) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    //while (1);
    return false;
  }
  Serial.println("BME280 sensor was initialized.");
  return true;
}

String BME280_ReadStrTemperature(){
  double data = bme.readTemperature();
  char charData[6];
  dtostrf(data, 5, 1, charData);
  delay(100);
  return String(charData);
}

String BME280_ReadStrHumidity(){
  double data = bme.readHumidity();
  char charData[6];
  dtostrf(data, 5, 1, charData);
  delay(100);
  return String(charData);
}

String BME280_ReadStrPressure(){
  double data = bme.readPressure()/100.0F;
  char charData[6];
  dtostrf(data, 5, 1, charData);
  delay(100);
  return String(charData);
}

double BME280_ReadHumidity(){
  return bme.readHumidity();
}

double BME280_ReadPressure(){
  return bme.readPressure()/100.0F;
}

double BME280_ReadTemperature(){
  return bme.readTemperature();
}

void BME280_ReadData() {
    Serial.print("Temperature = ");
    Serial.print(BME280_ReadStrTemperature());
    Serial.println(" *C");
    Serial.print("Pressure = ");
    Serial.print(BME280_ReadStrPressure());
    Serial.println(" hPa");
    Serial.print("Humidity = ");
    Serial.print(BME280_ReadStrHumidity());
    Serial.println(" %");
}

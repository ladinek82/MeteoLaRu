#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is plugged into pin D1 on the ESP8266 12-E - GPIO 5
#define ONE_WIRE_BUS 5
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature DS18B20(&oneWire);
  
bool DS18B20_Init(){
  Serial.println("Start inicialize DS18B20");
  DS18B20.begin();
  delay(1000); 
  Serial.println("DS18B20 sensor was initialized.");
  return true;
}

String DS18B20_ReadStrTemperature(){  
  double data = DS18B20_ReadTemperature();
  if(data == 8888)
    return "NaN";
  char charData[7];
  dtostrf(data, 2, 2, charData);
  return String(charData);
}

double DS18B20_ReadTemperature(){
  for(int i=0;i<20;i++) {
    DS18B20.requestTemperatures(); 
    double data = DS18B20.getTempCByIndex(0);
    if (data != 85.0 && data != (-127.0))   
      return data;
    delay(100);
  }
  return 8888;  
}

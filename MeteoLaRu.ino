#include "Statistic.h"

struct Config {
   char ssid[100];
   char password[100];
   char thinkSpeakAPIKey[50];
   bool apState;
   char fwLogin[50];
   char fwPassword[50];
};

struct Config cfg;

bool wifiClientConnected = false;
bool bmeState = false;
int mainPeriod = 5;

Statistic tempStatistic, humiStatistic, presStatistic;
int sendingCounter = 0;
int sendingPeriod = 60;
int numSample = 24; // 12 ~ 1min
int counterSample = 0;

/* EEPROM */


void setup() {
  // put your setup code here, to run once:
  delay(1000);
  Storage_Init(512);
  Serial.begin(9600);
  Serial.println();
  Serial.println("----------------------------------------------------"); 
   
  Wifi_Init();
  
  if(Storage_PercentUsed()>=0) {
    Serial.println("EEPROM contains data ("+String(Storage_PercentUsed())+ "%)");
  
    cfg = Storage_LoadConfig();
    Serial.println("----------CFG---------");
    Serial.println(cfg.ssid);
    Serial.println(cfg.password);
    Serial.println(cfg.thinkSpeakAPIKey);
    Serial.println(cfg.apState);
    Serial.println(cfg.fwLogin);
    Serial.println(cfg.fwPassword);
    Serial.println("----------CFG---------");
    
    wifiClientConnected = Wifi_ClientConnect(cfg.ssid, cfg.password);
//    if(wifiClientConnected){
//      if(!cfg.apState){
//        Wifi_EndAP();
//      }      
//    }
//    else{
//      cfg.apState = true;
//      Storage_SaveConfig(cfg);
//      Wifi_StartAP();
//    }

    if(cfg.apState || !wifiClientConnected){ // opakovane pripojeni po nejake dalsi dobe a pak teprve nnahodit AP
      cfg.apState = true;
      Storage_SaveConfig(cfg);
      Wifi_StartAP();
    }
    
  } 
  else {
    Wifi_StartAP();
  }
  
  
  WebServer_Init();
  bmeState = BME280_Init();
}

void loop() {
    WebServer_HandleClient();
    //if(bmeState){
      //BME280_ReadData(); 
    //}
    if(bmeState){
      tempStatistic.add(BME280_ReadTemperature());
      humiStatistic.add(BME280_ReadHumidity());
      presStatistic.add(BME280_ReadPressure());
      counterSample++;
    }
    if(wifiClientConnected && bmeState && (sendingCounter * mainPeriod) >= sendingPeriod){  
      sendingCounter = 0;
      
      double temperature = tempStatistic.average();
      double pressure = presStatistic.average();
      double humidity = humiStatistic.average();
      Serial.print("Temperature = ");
      Serial.print(temperature);
      Serial.println(" *C");
      Serial.print("Pressure = ");
      Serial.print(pressure);
      Serial.println(" hPa");
      Serial.print("Humidity = ");
      Serial.print(humidity);
      Serial.println(" %");
      ts_WriteData(String(temperature),String(pressure),String(humidity));
      tempStatistic.clear();
      presStatistic.clear();
      humiStatistic.clear();
    }

    if(numSample < counterSample){ // pojistka kdyby nebylo mozne odesilat
      tempStatistic.clear();
      presStatistic.clear();
      humiStatistic.clear();
      counterSample = 0;
      sendingCounter = 0;
    }
    
    sendingCounter++;
    delay(mainPeriod*1000);
}

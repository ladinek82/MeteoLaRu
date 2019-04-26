#include <WiFiClientSecure.h>
#include "ThingSpeak.h"

const char* server = "api.thingspeak.com";

WiFiClient tsClient;

void ts_WriteData(String data1, String data2, String data3, String data4){  
  if(cfg.thinkSpeakAPIKey == NULL || cfg.thinkSpeakChNumber == 0)
  {
    Serial.print("Thingspeak has not set.");
    return;
  }
  Serial.print("Sending data to Thingspeak...");
  ThingSpeak.begin(tsClient);
  
 
  ThingSpeak.setField(1, data1);
  ThingSpeak.setField(2, data2);
  ThingSpeak.setField(3, data3);
  ThingSpeak.setField(4, data4);

  int x = ThingSpeak.writeFields(cfg.thinkSpeakChNumber, cfg.thinkSpeakAPIKey);
  // Check the return code
  if(x == 200){
    Serial.println("OK");
  }
  else{
    Serial.println("ERROR (Problem updating channel. HTTP error code " + String(x) + ")");
  }
}

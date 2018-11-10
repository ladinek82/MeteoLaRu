#include <WiFiClientSecure.h>

const char* server = "api.thingspeak.com";

WiFiClient tsClient;

void ts_WriteData(String data1, String data2, String data3){
  Serial.println("Sending data to Thingspeak");
  if (tsClient.connect(server,80)) {
    String msg = cfg.thinkSpeakAPIKey;
    msg +="&field1=";
    msg += String(data1);
    msg +="&field2=";
    msg += String(data2);
    msg +="&field3=";
    msg += String(data3);
    msg += "\r\n\r\n";

    tsClient.print("POST /update HTTP/1.1\n");
    tsClient.print("Host: api.thingspeak.com\n");
    tsClient.print("Connection: close\n");
    tsClient.print("X-THINGSPEAKAPIKEY: "+ String(cfg.thinkSpeakAPIKey)+"\n");
    tsClient.print("Content-Type: application/x-www-form-urlencoded\n");
    tsClient.print("Content-Length: ");
    tsClient.print(msg.length());
    tsClient.print("\n\n");
    tsClient.print(msg);
  }
  tsClient.stop();
}

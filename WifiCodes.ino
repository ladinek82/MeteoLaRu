#include <ESP8266WiFi.h>

/* AP settings - default IP 192.168.4.1 */
const char* APssid = "NodeMCU1";
const char* APpassword = "asdf123456";

int numRetried = 20;

void Wifi_Init(){
  Serial.println("Configure Wifi");  
  WiFi.mode(WIFI_AP_STA);  
}

bool Wifi_ClientConnect(char *ssid, char *password){
  Serial.println("Disconnect");  
  WiFi.disconnect(); 
  Serial.print("Connecting to " + String(ssid));  
  WiFi.begin(ssid, password);
  int countRT = 0;
  while (WiFi.status() != WL_CONNECTED) {
    if(countRT > numRetried){
      Serial.println("Error");
      return false;
    }
    delay(500);
    Serial.print(".");
    countRT++;
  }  
  Serial.println("OK");
  Serial.println("Connected to " + String(cfg.ssid));
  Serial.print("My IP address is ");
  Serial.println(WiFi.localIP());
  return true;
}

void Wifi_StartAP(){
  Serial.println("Starting AP ...");
  WiFi.softAP(APssid, APpassword, 5, false, 2);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
}

void Wifi_EndAP(){
  Serial.println("Ending AP");  
  WiFi.softAPdisconnect();
}

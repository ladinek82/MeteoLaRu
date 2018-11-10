#include <ESP8266WebServer.h>
//#include <ESP8266mDNS.h>
#include <ESP8266HTTPUpdateServer.h>

ESP8266WebServer webServer(80);
ESP8266HTTPUpdateServer httpUpdater;

//const char* host = "meteolaru";
const char* update_path = "/firmware";

const String html_ssid = "#=ssid#";
const String html_clientIP = "#=clientIP#";
const String html_apState = "#=apState#";
const String html_password = "#=password#";
const String html_thinkSpeakAPIKey= "#=thinkSpeakAPIKey#";
const String html_errMsg = "#=errMsg#";
const String html_errMsgVisible = "#=errMsgVisible#";
const String html_fwPassword = "#=fwPassword#";
const String html_fwLogin = "#=fwLogin#";

const String html_temperature = "#=temperature#";
const String html_pressure = "#=pressure#";
const String html_humidity = "#=humidity#";


String indexHtml;
String configHtml;

void WebServer_Init(){
  FileStorage_Init();
  indexHtml = FileStorage_LoadFile("index.html");
  configHtml = FileStorage_LoadFile("config.html");
  
  webServer.on("/", handleRoot);
  webServer.on("/config", handleConfig);
  webServer.on("/save", HTTP_POST, handleSave);
  webServer.on("/ap", handleAP);
 

  //MDNS.begin(host);
  if(cfg.fwLogin == NULL || cfg.fwPassword == NULL){
    strcpy(cfg.fwLogin, "admin");
    strcpy(cfg.fwPassword, "admin");
    Storage_SaveConfig(cfg);
  }
  httpUpdater.setup(&webServer, update_path, cfg.fwLogin, cfg.fwPassword);
  webServer.begin();
  Serial.println("HTTP server started");
  
  //MDNS.addService("http", "tcp", 80);
  //String addr =  "192.168.4.1";
  //if(wifiClientConnected){
  //  addr =  WiFi.localIP().toString();
  //}
  Serial.println("HTTPUpdateServer ready");
  //Serial.println("Url: http://"+addr+update_path+", Login: "+cfg.fwLogin+", Password: " + cfg.fwPassword);
}

void headerDisableCache(){
  webServer.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate", true);
  webServer.sendHeader("Pragma", "no-cache", true);
  webServer.sendHeader("Expires", "0", true);
}

void handleRoot() { 
  String ssid = "NaN";
  String ipAddr = "NaN";
  if(wifiClientConnected){
    ssid = String(cfg.ssid);
    ipAddr = WiFi.localIP().toString();
  }
  String html = indexHtml;
  html.replace(html_ssid, ssid);
  html.replace(html_clientIP, ipAddr);
  html.replace(html_apState, cfg.apState ? "true" : "false");
  html.replace(html_temperature, BME280_ReadStrTemperature());
  html.replace(html_pressure, BME280_ReadStrPressure());
  html.replace(html_humidity, BME280_ReadStrHumidity());

  headerDisableCache();
  webServer.send(200, "text/html", html);   
}

void handleAP(){
  for (int i = 0; i < webServer.args(); i++) {
    if(webServer.argName(i) == "switch" && webServer.arg(i) == "off"){ 
      Wifi_EndAP();
      cfg.apState = false;
    }   
    if(webServer.argName(i) == "switch" && webServer.arg(i) == "on"){ 
      Wifi_StartAP();
      cfg.apState = true;
    }                
  }
  Storage_SaveConfig(cfg);
  headerDisableCache();  
  webServer.sendHeader("Location", "/", true);
  webServer.send(302, "text/plain", "");  
}

void handleConfig() { 
  String errMsg = "";
  for (int i = 0; i < webServer.args(); i++) {
    if(webServer.argName(i) == "conn" && webServer.arg(i) == "1"){ 
      if(wifiClientConnected){
        headerDisableCache();
        webServer.sendHeader("Location", String("http://") + WiFi.localIP().toString(), true);
        webServer.send(302, "text/plain", "");  
        return;
      }
      else{
        errMsg = "Connection failed. Please, check data and try it again.";
      }
    }                  
  } 
  
  String html = configHtml;
  html.replace(html_errMsgVisible, errMsg.length()>0 ? "true" : "false");
  html.replace(html_errMsg, errMsg);
  html.replace(html_ssid, String(cfg.ssid));
  html.replace(html_password, String(cfg.password));
  html.replace(html_thinkSpeakAPIKey, String(cfg.thinkSpeakAPIKey));
  html.replace(html_fwPassword, String(cfg.fwLogin));
  html.replace(html_fwLogin, String(cfg.fwPassword));


  headerDisableCache();
  webServer.send(200,"text/html", html);
}

void handleSave() {  // If a POST request is made to URI /login
  if( ! webServer.hasArg("ssid") || ! webServer.hasArg("password") || ! webServer.hasArg("thinkSpeakAPIKey")
      || webServer.arg("ssid") == NULL || webServer.arg("password") == NULL | webServer.arg("thinkSpeakAPIKey") == NULL) { // If the POST request doesn't have username and password data
    headerDisableCache();
    webServer.send(400, "text/plain", "400: Invalid Request");
    return;
  } 

  String tsStr = webServer.arg("thinkSpeakAPIKey");
  tsStr.toCharArray(cfg.thinkSpeakAPIKey,tsStr.length()+1);
  String sStr = webServer.arg("ssid");
  sStr.toCharArray(cfg.ssid,sStr.length()+1);
  String pStr = webServer.arg("password");
  pStr.toCharArray(cfg.password ,pStr.length()+1);
  String fwlStr = webServer.arg("fwLogin");
  fwlStr.toCharArray(cfg.fwLogin,fwlStr.length()+1);
  String fwpStr = webServer.arg("fwPassword");
  fwpStr.toCharArray(cfg.fwPassword ,fwpStr.length()+1);
  
  headerDisableCache();
  webServer.sendHeader("Connection", "close", true);
  webServer.sendHeader("Refresh", "30;url=/config?conn=1", true);
  webServer.send(200, "text/html", "<p>Connecting to wifi " + String(cfg.ssid) +"...</p>");
  
  Storage_SaveConfig(cfg);
  
  Serial.println("Connecting ssid: " + String(cfg.ssid) + ", pass: " + String(cfg.password) + ", thinkSpeakAPIKey: " + String(cfg.thinkSpeakAPIKey));  

  wifiClientConnected = Wifi_ClientConnect(cfg.ssid, cfg.password);

  if(!wifiClientConnected){
    Wifi_StartAP();
  }
}

void WebServer_HandleClient(){
  webServer.handleClient();
}

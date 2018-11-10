#include "FS.h"


void FileStorage_Init(){
  SPIFFS.begin();
}

String FileStorage_LoadFile(String filename){
  String fileContent = "";
  File f = SPIFFS.open("/"+filename, "r");
  if (!f) {
    Serial.println("file open failed");
    return "";
  }

  for(int i=0;i<f.size();i++){
    byte fc = f.read();
    //Serial.print((char)fc);
    fileContent += (char)fc;    
  }
  //Serial.print(fileContent);
  return fileContent;
}

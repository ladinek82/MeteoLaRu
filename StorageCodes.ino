#include <ESP_EEPROM.h>

int cfgAddress = 0;

void Storage_Init(int size){
  EEPROM.begin(size);
}

void Storage_SaveConfig(Config cfg)
{
  EEPROM.put(cfgAddress, cfg);
  EEPROM.commitReset();
}

Config Storage_LoadConfig()
{
  Config cfg;
  EEPROM.get(cfgAddress, cfg);
  return cfg;
}

int Storage_PercentUsed(){
  return EEPROM.percentUsed();
}

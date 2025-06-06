// Settings: Stores persistant settings, loads and saves to EEPROM

#include <Arduino.h>
#include <EEPROM.h>

#define EEPROM_SIZE 1024

class Settings
{
  // change eeprom config version ONLY when new parameter is added and need reset the parameter
  unsigned int configVersion = 11;

public:
  String deviceNameStr;
  struct Data
  {                              // do not re-sort this struct
    unsigned int coVers;         // config version, if changed, previus config will erased
    char deviceName[40];         // device name
    char mqttServer[40];         // mqtt Server adress
    char mqttUser[40];           // mqtt Username
    char mqttPassword[40];       // mqtt Password
    char mqttTopic[40];          // mqtt publish topic
    char mqttTriggerPath[80];    // MQTT Data Trigger Path
    unsigned int mqttPort;       // mqtt port
    unsigned int mqttRefresh;    // mqtt refresh time
    unsigned int deviceQuantity; // Quantity of Devices
    bool mqttJson;               // switch between classic mqtt and json
    bool webUIdarkmode;          // Flag for color mode in webUI
    char httpUser[40];           // http basic auth username
    char httpPass[40];           // http basic auth password
    bool haDiscovery;            // HomeAssistant Discovery switch
    char NTPTimezone[40];        // Time zone code for NTP get it from here: https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv
    char NTPServer[40];          // NTP timepool Server
  } data;

  void load()
  {
    data = {}; // clear bevor load data
    EEPROM.begin(EEPROM_SIZE);
    EEPROM.get(0, data);
    EEPROM.end();
    coVersCheck();
    sanitycheck();
    deviceNameStr = data.deviceName;
  }

  void save()
  {
    sanitycheck();
    EEPROM.begin(EEPROM_SIZE);
    EEPROM.put(0, data);
    EEPROM.commit();
    EEPROM.end();
  }

  void reset()
  {
    data = {};
    save();
  }

private:
  // check the variables from eeprom

  void sanitycheck()
  {
    if (strlen(data.deviceName) == 0 || strlen(data.deviceName) >= 40)
    {
      strcpy(data.deviceName, "EPEver2MQTT");
    }
    if (strlen(data.mqttServer) == 0 || strlen(data.mqttServer) >= 40)
    {
      strcpy(data.mqttServer, "");
    }
    if (strlen(data.mqttUser) == 0 || strlen(data.mqttUser) >= 40)
    {
      strcpy(data.mqttUser, "");
    }
    if (strlen(data.mqttPassword) == 0 || strlen(data.mqttPassword) >= 40)
    {
      strcpy(data.mqttPassword, "");
    }
    if (strlen(data.mqttTopic) == 0 || strlen(data.mqttTopic) >= 40)
    {
      strcpy(data.mqttTopic, "EPEver");
    }
    if (data.mqttPort <= 0 || data.mqttPort >= 65530)
    {
      data.mqttPort = 0;
    }
    if (data.mqttRefresh <= 1 || data.mqttRefresh >= 65530)
    {
      data.mqttRefresh = 0;
    }
    if (data.mqttJson && !data.mqttJson)
    {
      data.mqttJson = false;
    }
    if (data.deviceQuantity < 1 || data.deviceQuantity >= 10)
    {
      data.deviceQuantity = 1;
    }
    if (strlen(data.mqttTriggerPath) == 0 || strlen(data.mqttTriggerPath) >= 80)
    {
      strcpy(data.mqttTriggerPath, "");
    }
    if (data.webUIdarkmode && !data.webUIdarkmode)
    {
      data.webUIdarkmode = false;
    }
    if (strlen(data.httpUser) == 0 || strlen(data.httpUser) >= 40)
    {
      strcpy(data.httpUser, "");
    }
    if (strlen(data.httpPass) == 0 || strlen(data.httpPass) >= 40)
    {
      strcpy(data.httpPass, "");
    }
    if (data.haDiscovery && !data.haDiscovery)
    {
      data.haDiscovery = false;
    }
    if (strlen(data.NTPTimezone) == 0 || strlen(data.NTPTimezone) >= 40)
    {
      strcpy(data.NTPTimezone, "");
    }
    if (strlen(data.NTPServer) == 0 || strlen(data.NTPServer) >= 40)
    {
      strcpy(data.NTPServer, "pool.ntp.org");
    }
  }
  void coVersCheck()
  {
    if (data.coVers != configVersion)
    {
      data.coVers = configVersion;
      strcpy(data.deviceName, "EPEver2MQTT");
      strcpy(data.mqttServer, "");
      strcpy(data.mqttUser, "");
      strcpy(data.mqttPassword, "");
      strcpy(data.mqttTopic, "EPEver");
      strcpy(data.mqttTriggerPath, "");
      data.deviceQuantity = 1;
      data.mqttPort = 0;
      data.mqttRefresh = 300;
      data.mqttJson = false;
      data.webUIdarkmode = false;
      strcpy(data.httpUser, "");
      strcpy(data.httpPass, "");
      data.haDiscovery = false;
      save();
      load();
    }
  }
};

#define SOFTWARE_VERSION "1.6"
#define module 2

#include <AsyncWiFiManager.h>
#include <LittleFS.h>
#include <ArduinoHA.h>
#include <ArduinoJson.h>
#include <EEPROM.h>
#include <Ticker.h>
#include <ESP8266WebServer.h>
#include <WiFiClient.h>
#include <ESP8266HTTPUpdateServer.h>
#include <ESP8266mDNS.h>
#include <EspHtmlTemplateProcessor.h>


byte mac[WL_MAC_ADDR_LENGTH];
#define DEVICE_MANUFACTURER "MDtronix Lab"
#if (module == 1)
#define DEVICE_MODEL "1CH V1.0"
#elif (module == 2)
#define DEVICE_MODEL "2CH V1.0"
#elif (module == 3)
#define DEVICE_MODEL "3CH V1.0"
#elif (module == 4)
#define DEVICE_MODEL "4CH V1.0"
#endif
#define BROKER_USER "hassio"
#define BROKER_PASS "darkmaster"

#if (module == 1)
#define DEVICE_NAME "MDtronix 1CH"
String AP_SSID = "MDtronix-1CH-" + String(ESP.getChipId(), HEX);
#elif (module == 2)
#define DEVICE_NAME "MDtronix 2CH"
String AP_SSID = "MDtronix-2CH-" + String(ESP.getChipId(), HEX);
#elif (module == 3)
#define DEVICE_NAME "MDtronix 3CH"
String AP_SSID = "MDtronix-3CH-" + String(ESP.getChipId(), HEX);
#elif (module == 4)
#define DEVICE_NAME "MDtronix 4CH"
String AP_SSID = "MDtronix-4CH-" + String(ESP.getChipId(), HEX);
#endif

#define relay1_mem 1
#define relay2_mem 2
#define relay3_mem 3
#define relay4_mem 4

#define switch1_laststate_mem 5
#define switch2_laststate_mem 6
#define switch3_laststate_mem 7
#define switch4_laststate_mem 8

#define bootFlag 9

#include <pins.h>
#if (module == 1 || module == 2 || module == 3 || module == 4)
uint8_t switch1;
#endif
#if (module == 2 || module == 3 || module == 4)
uint8_t switch2;
#endif
#if (module == 3 || module == 4)
uint8_t switch3;
#endif
#if (module == 4)
uint8_t switch4;
#endif

bool switch1_state,
    switch2_state,
    switch3_state,
    switch4_state,
    switch1_laststate(false),
    switch2_laststate(false),
    switch3_laststate(false),
    switch4_laststate(false);
bool onConnect = false;
// AsyncWebServer server(80);
AsyncWiFiManager wm;
ESP8266WebServer server(80);
EspHtmlTemplateProcessor TP(&server);
ESP8266HTTPUpdateServer httpUpdater;
// Timer t;
Ticker t;

WiFiClient client;
HADevice device;
HAMqtt mqtt_HA(client, device);

#if (module == 1 || module == 2 || module == 3 || module == 4)
HASwitch _switch1("1", false);
#endif
#if (module == 2 || module == 3 || module == 4)
HASwitch _switch2("2", false);
#endif
#if (module == 3 || module == 4)
HASwitch _switch3("3", false);
#endif
#if (module == 4)
HASwitch _switch4("4", false);
#endif

char mqtt_server[20];
char room_name[20];
#if (module == 1 || module == 2 || module == 3 || module == 4)
char relay1_name[20];
#endif
#if (module == 2 || module == 3 || module == 4)
char relay2_name[20];
#endif
#if (module == 3 || module == 4)
char relay3_name[20];
#endif
#if (module == 4)
char relay4_name[20];
#endif

#define MQTT_PARAM "mqtt"
#define ROOM_PARAM "room"
#define R1_PARAM "r1"
#define R2_PARAM "r2"
#define R3_PARAM "r3"
#define R4_PARAM "r4"

#include <init.h>
#include <otherEvents.h>
#include <wifiSetup.h>
#include <buttonEvent.h>

void setup()
{
  EEPROM.begin(32);
  Serial.begin(115200);
  Serial.println();
  Serial.println();
  if (!LittleFS.begin())
  {
    Serial.println(F("Failed to mount file system"));
    return;
  }
  else
  {
    Serial.println(F("mounted"));
  }
  if (!loadConfig())
  {
    Serial.println(F("failed to load config"));
  }
  else
  {
    Serial.println(F("config loaded"));
  }
#if (module == 1 || module == 2 || module == 3 || module == 4)
  pinMode(relay1, OUTPUT);
  // S1.begin();
  pinMode(switch1, INPUT_PULLUP);
#endif
#if (module == 2 || module == 3 || module == 4)
  pinMode(relay2, OUTPUT);
  // S2.begin();
  pinMode(switch2, INPUT_PULLUP);
#endif
#if (module == 3 || module == 4)
  pinMode(relay3, OUTPUT);
  // S3.begin();
  pinMode(switch3, INPUT_PULLUP);
#endif
#if (module == 4)
  pinMode(relay4, OUTPUT);
  // S4.begin();
  pinMode(switch4, INPUT_PULLUP);
#endif
  pinMode(led, OUTPUT);
  wifi_set();
  setup_HA();
  t.attach_ms(100, ButtonEvent);
  readLastState();
}

void loop()
{
#if (module == 1 || module == 2 || module == 3 || module == 4)
  switch1_state = digitalRead(switch1);
#endif
#if (module == 2 || module == 3 || module == 4)
  switch2_state = digitalRead(switch2);
#endif
#if (module == 3 || module == 4)
  switch3_state = digitalRead(switch3);
#endif
#if (module == 4)
  switch4_state = digitalRead(switch4);
#endif
  wm.process();
  mqtt_HA.loop();
  server.handleClient();
  MDNS.update();
}
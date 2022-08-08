void setup_HA()
{
  WiFi.macAddress(mac);
  device.setUniqueId(mac, sizeof(mac));
  device.setName(DEVICE_NAME);
  device.setSoftwareVersion(SOFTWARE_VERSION);
  device.setManufacturer(DEVICE_MANUFACTURER);
  device.setModel(DEVICE_MODEL);
  device.enableSharedAvailability();
  device.enableLastWill();
#if (module == 1 || module == 2 || module == 3 || module == 4)
  _switch1.onStateChanged(Switch1_action);
  _switch1.setName(relay1_name);
#endif
#if (module == 2 || module == 3 || module == 4)
  _switch2.onStateChanged(Switch2_action);
  _switch2.setName(relay2_name);
#endif
#if (module == 3 || module == 4)
  _switch3.onStateChanged(Switch3_action);
  _switch3.setName(relay3_name);
#endif
#if (module == 4)
  _switch4.onStateChanged(Switch4_action);
  _switch4.setName(relay4_name);
#endif

  IPAddress HA_IP;
  HA_IP.fromString(mqtt_server);
  mqtt_HA.begin(HA_IP, BROKER_USER, BROKER_PASS);
  mqtt_HA.onConnected(onMqttConnected);
  mqtt_HA.onConnectionFailed(onMqttConnectionFailed);
}

void configModeCallback(AsyncWiFiManager *myAsyncWiFiManager)
{
  t.attach(1, led_blink);
}

void onSaveCallback()
{
  Serial.println(F("WiFi Connected"));
  t.detach();
  digitalWrite(led, LOW);
}

String processor(const String &var)
{
  if (var == "set_server")
    return mqtt_server;
  if (var == "device_name")
    return room_name;
  if (var == "switch_set")
  {
    String val = "";
#if (module == 1 || module == 2 || module == 3 || module == 4)
    val += "<br><label>Switch 1 Name</label><br><input type=\"text\" name=\"" + String(R1_PARAM) + "\" maxlength=\"20\" value=\"" + relay1_name + "\"><br>";
#endif
#if (module == 2 || module == 3 || module == 4)
    val += "<br><label>Switch 2 Name</label><br><input type=\"text\" name=\"" + String(R2_PARAM) + "\" maxlength=\"20\" value=\"" + relay2_name + "\"><br>";
#endif
#if (module == 3 || module == 4)
    val += "<br><label>Switch 3 Name</label><br><input type=\"text\" name=\"" + String(R3_PARAM) + "\" maxlength=\"20\" value=\"" + relay3_name + "\"><br>";
#endif
#if (module == 4)
    val += "<br><label>Switch 4 Name</label><br><input type=\"text\" name=\"" + String(R4_PARAM) + "\" maxlength=\"20\" value=\"" + relay4_name + "\"><br>";
#endif
    return val;
  }
  return String();
}

void wifi_set()
{
  t.attach(.2, led_blink);
  WiFi.mode(WIFI_AP_STA);
  wm.setConfigPortalBlocking(false);
  // wm.resetSettings();
  wm.setAPCallback(configModeCallback);
  // wm.setSaveConfigCallback(onSaveCallback);
  wm.setClass("invert");                // set dark theme
  if (!wm.autoConnect(AP_SSID.c_str())) // if Connected successfully
  {
    // Serial.println(F("cannot start WM"));
    t.detach();
    digitalWrite(led, LOW);
    // ESP.restart();
  }
  else
  {
    t.detach();
    digitalWrite(led, LOW);
  }
  server.onNotFound([]()
                    { server.send(404, "text/plain", "hii"); });

  server.on("/setting", HTTP_GET, []()
            { TP.processAndSend("index.html", processor); });

  server.on("/save", HTTP_GET, []()
            {
              String InputMSG;
              StaticJsonDocument<200> doc;
              if(server.hasArg(MQTT_PARAM))
              {
                doc["mqtt_server"] = server.arg(MQTT_PARAM);
              }
              if(server.hasArg(ROOM_PARAM))
              {
                doc["room name"] = server.arg(ROOM_PARAM);
              }
              if (server.hasArg(R1_PARAM))
              {
                doc["switch 1"] = server.arg(R1_PARAM);
              }
              if (server.hasArg(R2_PARAM))
              {
                doc["switch 2"] = server.arg(R2_PARAM);
              }
              if (server.hasArg(R3_PARAM))
              {
                doc["switch 3"] = server.arg(R3_PARAM);
              }
              if (server.hasArg(R4_PARAM))
              {
                doc["switch 4"] = server.arg(R4_PARAM);
              }
              InputMSG=""            ;
              File configFile = LittleFS.open("/config.json","w");
              if (!configFile)
              {
                  Serial.println(F("Failed to open config file for writing"));
              }
              serializeJsonPretty(doc,configFile);
              Serial.println(F("Wrote to FS"));
              configFile.close();
              serializeJsonPretty(doc,InputMSG);
              server.send(200,"text/json",InputMSG); 
              ESP.restart(); });
  server.on("/pins", HTTP_GET, []()
            {
              String InputMSG;
              StaticJsonDocument<200> doc;
              if (server.hasArg("s1"))
                doc["s1"] = server.arg("s1");
              if (server.hasArg("s2"))
                doc["s2"] = server.arg("s2");
              if (server.hasArg("s3"))
                doc["s3"] = server.arg("s3");
              if (server.hasArg("s4"))
                doc["s4"] = server.arg("s4");
              InputMSG = "";
              File pinFile = LittleFS.open("/pins.json", "w");
              if (!pinFile)
              {
                Serial.println(F("Failed to open config file for writing"));
              }
              serializeJsonPretty(doc, pinFile);
              Serial.println(F("pins Wrote"));
              pinFile.close();
              serializeJsonPretty(doc,InputMSG);
              server.send(200,"text/json",InputMSG); 
              ESP.restart(); });
  server.on("/get", HTTP_GET, []()
            {
    StaticJsonDocument<200> doc;
    doc["module"] = module;
    doc["mqtt_server"] = mqtt_server;
    doc["room"] = room_name;
#if (module == 1 || module == 2 || module == 3 || module == 4)
    doc["switch 1"] = relay1_name;
#endif
#if (module == 2 || module == 3 || module == 4)
    doc["switch 2"] = relay2_name;
#endif
#if (module == 3 || module == 4)
    doc["switch 3"] = relay3_name;
#endif
#if (module == 4)
    doc["switch 4"] = relay4_name;
#endif
    String data;
    serializeJsonPretty(doc, data);
    server.send(200, "text/json", data); });
  server.on("/rst", HTTP_GET, []()
            {
    server.send(200, "text/text", "OK");
    ESP.restart(); });
  server.on("/rstwifi", HTTP_GET, []()
            {
    server.send(200, "text/text", "OK");
    wm.resetSettings();
    delay(5000);
    ESP.restart(); });

  if (!MDNS.begin(room_name))
  {
    Serial.println("Error setting up MDNS responder!");
    while (1)
    {
      delay(1000);
    }
  }
  httpUpdater.setup(&server);
  server.begin();
  MDNS.addService("http", "tcp", 80);
}
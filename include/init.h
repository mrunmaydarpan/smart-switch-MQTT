bool loadConfig()
{
    File configFile = LittleFS.open("/config.json", "r");
    if (!configFile)
    {
        Serial.println(F("Failed to open config file"));
        return false;
    }

    size_t size = configFile.size();
    if (size > 1024)
    {
        Serial.println(F("Config file size is too large"));
        return false;
    }

    std::unique_ptr<char[]> buf(new char[size]);
    configFile.readBytes(buf.get(), size);

    StaticJsonDocument<200> doc;
    auto error = deserializeJson(doc, buf.get());
    if (error)
    {
        Serial.println(F("Failed to parse config file"));
        return false;
    }

    strcpy(mqtt_server, doc["mqtt_server"]);
    strcpy(room_name, doc["room name"]);
#if (module == 1 || module == 2 || module == 3 || module == 4)
    strcpy(relay1_name, doc["switch 1"]);
#endif
#if (module == 2 || module == 3 || module == 4)
    strcpy(relay2_name, doc["switch 2"]);
#endif
#if (module == 3 || module == 4)
    strcpy(relay3_name, doc["switch 3"]);
#endif
#if (module == 4)
    strcpy(relay4_name, doc["switch 4"]);
#endif
    configFile.close();
    Serial.println();
    serializeJsonPretty(doc, Serial);
    Serial.println();
    ////////////////////////////////////////////////////////////////////
    StaticJsonDocument<200> pinFile_doc;
    File pinFile = LittleFS.open("/pins.json", "r");
    if (!pinFile)
    {
        Serial.println(F("Failed to open config file"));
#if (module == 1)
        switch1 = 14;
        pinFile_doc["s1"] = switch1;
#elif (module == 2)
        switch1 = 5;
        switch2 = 4;
        pinFile_doc["s1"] = switch1;
        pinFile_doc["s2"] = switch2;
#elif (module == 3)
        switch1 = 5;
        switch2 = 4;
        switch3 = 13;
        pinFile_doc["s1"] = switch1;
        pinFile_doc["s2"] = switch2;
        pinFile_doc["s3"] = switch3;
#elif (module == 4)
        switch1 = 3;
        switch2 = 13;
        switch3 = 12;
        switch4 = 14;
        pinFile_doc["s1"] = switch1;
        pinFile_doc["s2"] = switch2;
        pinFile_doc["s3"] = switch3;
        pinFile_doc["s4"] = switch4;
#endif
        pinFile = LittleFS.open("/pins.json", "w");
        if (!pinFile)
        {
            Serial.println(F("failed to write pinfile"));
        }
        serializeJsonPretty(pinFile_doc, pinFile);
        Serial.println(F("pins Wrote"));
        return false;
    }

    size_t pinFile_size = pinFile.size();
    if (pinFile_size > 1024)
    {
        Serial.println(F("Config file size is too large"));
        return false;
    }

    std::unique_ptr<char[]> pinFile_buf(new char[pinFile_size]);
    pinFile.readBytes(pinFile_buf.get(), pinFile_size);

    auto pinFile_error = deserializeJson(pinFile_doc, pinFile_buf.get());
    if (pinFile_error)
    {
        Serial.println(F("Failed to parse config file"));
        return false;
    }
    if (pinFile_doc["s1"] == "" && pinFile_doc["s2"] == "" && pinFile_doc["s3"] == "" && pinFile_doc["s4"] == "")
    {
#if (module == 1)
        switch1 = 14;
        pinFile_doc["s1"] = switch1;
#elif (module == 2)
        switch1 = 5;
        switch2 = 4;
        pinFile_doc["s1"] = switch1;
        pinFile_doc["s2"] = switch2;
#elif (module == 3)
        switch1 = 5;
        switch2 = 4;
        switch3 = 13;
        pinFile_doc["s1"] = switch1;
        pinFile_doc["s2"] = switch2;
        pinFile_doc["s3"] = switch3;
#elif (module == 4)
        switch1 = 3;
        switch2 = 13;
        switch3 = 12;
        switch4 = 14;
        pinFile_doc["s1"] = switch1;
        pinFile_doc["s2"] = switch2;
        pinFile_doc["s3"] = switch3;
        pinFile_doc["s4"] = switch4;
#endif
        pinFile = LittleFS.open("/pins.json", "w");
        if (!pinFile)
        {
            Serial.println(F("failed to write pinfile"));
        }
        serializeJsonPretty(pinFile_doc, pinFile);
        Serial.println(F("pins Wrote"));
        // pinFile.close();
    }
    else
    {
    }
#if (module == 1 || module == 2 || module == 3 || module == 4)
    switch1 = pinFile_doc["s1"];
#endif
#if (module == 2 || module == 3 || module == 4)
    switch2 = pinFile_doc["s2"];
#endif
#if (module == 3 || module == 4)
    switch3 = pinFile_doc["s3"];
#endif
#if (module == 4)
    switch4 = pinFile_doc["s4"];
#endif
    pinFile.close();
    Serial.println();
    serializeJsonPretty(pinFile_doc, Serial);
    Serial.println();
    return true;
}
void readLastState()
{
    EEPROM.begin(16);
    switch1_laststate = EEPROM.read(switch1_laststate_mem);
    switch2_laststate = EEPROM.read(switch2_laststate_mem);
    switch3_laststate = EEPROM.read(switch3_laststate_mem);
    switch4_laststate = EEPROM.read(switch4_laststate_mem);
    Serial.print(F("-+-+-+-EEPROM: "));
#if (module == 1 || module == 2 || module == 3 || module == 4)
    digitalWrite(relay1, EEPROM.read(relay1_mem) ? HIGH : LOW);
#endif
#if (module == 2 || module == 3 || module == 4)
    digitalWrite(relay2, EEPROM.read(relay2_mem) ? HIGH : LOW);
#endif
#if (module == 3 || module == 4)
    digitalWrite(relay3, EEPROM.read(relay3_mem) ? HIGH : LOW);
#endif
#if (module == 4)
    digitalWrite(relay4, EEPROM.read(relay4_mem) ? HIGH : LOW);
#endif
    Serial.printf("%d %d %d %d", EEPROM.read(relay1_mem), EEPROM.read(relay2_mem), EEPROM.read(relay3_mem), EEPROM.read(relay4_mem));
    Serial.println();
    // Serial.printf("Relays: %d %d %d\n\n", digitalRead(relay1), digitalRead(relay2), digitalRead(relay3));
}

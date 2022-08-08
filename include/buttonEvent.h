void ButtonEvent()
{
#if (module == 1 || module == 2 || module == 3 || module == 4)
    if (switch1_state != switch1_laststate)
    {
        digitalWrite(relay1, !digitalRead(relay1));
        EEPROM.write(relay1_mem, digitalRead(relay1));
        _switch1.setState(digitalRead(relay1));
        // Serial.printf("\nswitch state changed: %d\n", digitalRead(relay1));
    }
    EEPROM.write(switch1_laststate_mem, switch1_state);
    switch1_laststate = switch1_state;
#endif
#if (module == 2 || module == 3 || module == 4)
    if (switch2_state != switch2_laststate)
    {
        digitalWrite(relay2, !digitalRead(relay2));
        EEPROM.write(relay2_mem, digitalRead(relay2));
        EEPROM.commit();
        _switch2.setState(digitalRead(relay2));
    }
    EEPROM.write(switch2_laststate_mem, switch2_state);
    switch2_laststate = switch2_state;
#endif
#if (module == 3 || module == 4)
    if (switch3_state != switch3_laststate)
    {
        digitalWrite(relay3, !digitalRead(relay3));
        EEPROM.write(relay3_mem, digitalRead(relay3));
        EEPROM.commit();
        _switch3.setState(digitalRead(relay3));
    }
    EEPROM.write(switch3_laststate_mem, switch3_state);
    switch3_laststate = switch3_state;
#endif
#if (module == 4)
    if (switch4_state != switch4_laststate)
    {
        digitalWrite(relay4, !digitalRead(relay4));
        EEPROM.write(relay4_mem, digitalRead(relay4));
        EEPROM.commit();
        _switch4.setState(digitalRead(relay4));
    }
    EEPROM.write(switch4_laststate_mem, switch4_state);
    switch4_laststate = switch4_state;
#endif
    EEPROM.commit();
}
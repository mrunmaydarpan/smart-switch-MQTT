#if (module == 1 || module == 2 || module == 3 || module == 4)
void Switch1_action(bool state, HASwitch *s)
{
  digitalWrite(relay1, state ? HIGH : LOW);
  EEPROM.write(relay1_mem, state);
  EEPROM.commit();
  Serial.printf("s1: %d\n", state);
}
#endif
#if (module == 2 || module == 3 || module == 4)
void Switch2_action(bool state, HASwitch *s)
{
  digitalWrite(relay2, state ? HIGH : LOW);
  EEPROM.write(relay2_mem, state);
  EEPROM.commit();
  Serial.printf("s2: %d\n", state);
}
#endif
#if (module == 3 || module == 4)
void Switch3_action(bool state, HASwitch *s)
{
  digitalWrite(relay3, state ? HIGH : LOW);
  EEPROM.write(relay3_mem, state);
  EEPROM.commit();
  Serial.printf("s3: %d\n", state);
}
#endif
#if (module == 4)
void Switch4_action(bool state, HASwitch *s)
{
  digitalWrite(relay4, state ? HIGH : LOW);
  EEPROM.write(relay4_mem, state);
  EEPROM.commit();
  Serial.printf("s4: %d\n", state);
}
#endif

void onMqttConnected()
{
  Serial.println(F("Connected to the broker!"));
#if (module == 1 || module == 2 || module == 3 || module == 4)
  _switch1.setState(EEPROM.read(relay1_mem));
#endif
#if (module == 2 || module == 3 || module == 4)
  _switch2.setState(EEPROM.read(relay2_mem));
#endif
#if (module == 3 || module == 4)
  _switch3.setState(EEPROM.read(relay3_mem));
#endif
#if (module == 4)
  _switch4.setState(EEPROM.read(relay4_mem));
#endif
}

void onMqttConnectionFailed()
{
  Serial.println(F("Failed to connect to the broker!"));
}

void led_blink()
{
  digitalWrite(led, !digitalRead(led));
}
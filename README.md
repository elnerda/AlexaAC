# Alexa Klimaanlage


Alexa Klimaanlage ist ein selbst entwickelter Controller um Klimaanlagen über Amazon Alexa zu steuern.
# Features
  - Ein- und Ausschalten der Klimaanlage per Sprachebefehl und Alexa App
# Geplante Features
 - Einstellen der Temperatur per Sprachebefehl und Alexa App
 - Rückgabe der Temperatur als Sprachausgabe über den Echo
### Software
- Arduino Bibliothek für den ESP8266 
- ArduinoJson 5.13.1, die Version der Bibliothek ist kritisch für erfolgreiches Kompilieren des Codes
- IRsend
- IRremoteESP8266
- OneWire
- DallasTemperature
- WebSocketsClient
# Installation der nötigen Software
Ich habe zum Aufspielen des Sourcecodes Atom mit dem [PlatformIO Plug-In](https://platformio.org/) verwendet. Eine ausführliche Doukmentation dazu findet sich [hier](https://docs.platformio.org/en/latest/).
Die benötigten Bibliotheken lassen sich direkt in Atom installieren. Auch hierzu gibt es eine [Anleitung](https://docs.platformio.org/en/latest/librarymanager/index.html).




#include <Arduino.h>
#include <UniversalTelegramBot.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "secrets.h"
/*-----------------------------------------------------------------------------------------------------------------------*/
char ssid[] = SECRET_SSID;     // Hier Netzwerknamen eingeben
char password[] = SECRET_PASS; // Hier Netzwerkpasswort eingeben
const char botToken[]=SECRET_BOT_TOKEN;
/*-----------------------------------------------------------------------------------------------------------------------*/
WiFiClientSecure client;
UniversalTelegramBot bot(botToken,client);
IRsend irsend(IR_LED);
OneWire onewire(ONE_WIRE_BUS);
DallasTemperature ds(&onewire);
/*-----------------------------------------------------------------------------------------------------------------------*/
int resetTicker = 0;            // Reset Ticker, damit ESP sich selbst resetten kann
int postTicker = 0;
int tickDelay = 500;      //Zeit zwischen der Abfrage neuer Nachrichten
int khz = 38;
int ledPin = D1;
bool anaus= false;
float temp;
int settemp=18;
/*------------------------------------------------------------------------------------------------------------------------*/
unsigned int Bot_mtbs = 1000; //mean time between scan messages
unsigned long Bot_lasttime;   //last time messages' scan has been done
bool Start = false;
/*------------------------------------------------------------------------------------------------------------------------*/
uint16_t aus[] = {8350, 4200, 450, 1650, 450, 550, 500, 550, 500, 550, 500, 1600, 500, 550, 450, 600, 450,
  550, 500, 1650, 450, 1600, 500, 550, 500, 550, 450, 600, 450, 550, 500, 550, 500, 550, 500, 550, 500, 550,
  500, 550, 500, 550, 450, 550, 500, 1650, 450, 550, 500, 1600, 500, 550, 500, 550, 450, 550, 500, 1650, 450};
uint16_t an18[] = {8300,4200, 500,1600, 500,550, 500,550, 450,550, 500,1650, 450,550, 500,550, 500,550, 500,550,
   500,550, 450,550, 500,550, 500,550, 500,550, 500,550, 500,550, 500,550, 500,500, 500,1650, 450,1600, 500,550,
    500,1600, 500,500, 500,550, 500,550, 500,1600, 500,1600, 500,1600, 450};
uint16_t an19[] = {8300,4200, 500,1600, 500,550, 500,550, 500,500, 500,1650, 450,550, 500,550, 500,550, 500,550,
  500,550, 450,550, 500,550, 500,550, 500,550, 500,550, 500,550, 500,550, 500,1600, 450,550, 500,550, 500,550,
  500,1600, 500,550, 450,600, 450,1650, 450,550, 500,550, 500,550, 500};
uint16_t an20[] = {8350,4200, 500,1600, 450,550, 500,550, 500,550, 500,1600, 500,550, 500,500, 500,550, 500,550,
  500,550, 500,550, 500,550, 500,550, 500,550, 500,500, 500,550, 500,550, 500,1600, 500,550, 500,1600, 450,550,
  500,1650, 450,550, 500,550, 500,1600, 500,550, 450,550, 500,1650, 450};
uint16_t an21[] = {8350,4200, 500,1600, 450,600, 450,550, 500,550, 500,1600, 500,550, 500,550, 500,550, 450,550,
   500,600, 450,550, 500,550, 500,550, 500,550, 500,550, 500,550, 450,550, 500,1650, 450,1600, 500,550, 500,550,
    500,1600, 450,600, 450,600, 450,1600, 500,550, 500,1600, 450,550, 500};
uint16_t an22[] = {8300,4200, 500,1650, 450,550, 500,550, 500,550, 500,1600, 450,550, 500,550, 500,550, 550,500,
  500,550, 500,550, 500,550, 500,550, 450,550, 500,600, 450,550, 500,550, 500,1600, 500,1600, 450,1650, 450,550,
   500,1600, 500,550, 500,550, 450,1650, 450,600, 500,1550, 500,1600, 500};
uint16_t set21[] = {8350,4200, 500,1600, 450,550, 500,550, 500,550, 500,1600, 500,550, 450,600, 450,600, 450,550,
   500,550, 500,550, 500,550, 450,1650, 500,550, 450,600, 450,550, 500,550, 500,1600, 500,1600, 500,550, 450,550,
   500,1600, 500,550, 500,550, 450,600, 500,550, 500,1550, 500,550, 500};
uint16_t set20[] = {8300,4200, 500,1600, 500,550, 500,550, 450,600, 500,1550, 500,550, 500,550, 500,550, 500,550,
  500,550, 500,550, 500,550, 450,1600, 500,550, 500,550, 450,600, 500,550, 500,1600, 450,550, 500,1600, 500,550,
   450,1650, 500,550, 450,600, 450,550, 500,550, 500,550, 500,1600, 500};
uint16_t set19[] = {8350,4200, 500,1600, 450,550, 500,550, 500,550, 500,1600, 500,550, 500,550, 450,550, 500,550,
  500,550, 500,550, 500,550, 500,1600, 450,550, 500,550, 500,550, 500,550, 500,1600, 500,550, 500,550, 450,550,
  500,1650, 450,550, 500,550, 500,550, 500,550, 500,550, 450,550, 500};
uint16_t set18[] = {8300,4200, 550,1550, 500,550, 500,550, 500,500, 500,1600, 500,550, 500,550, 500,550, 500,550,
  500,550, 450,550, 500,550, 500,1600, 500,550, 500,550, 500,550, 500,500, 500,550, 500,1600, 500,1600, 500,550,
  500,1550, 500,550, 500,550, 500,1600, 500,1600, 500,1550, 500,1600, 500};
uint16_t set22[] = {8350,4200, 500,1600, 450,550, 500,550, 500,550, 500,1600, 500,550, 500,550, 500,500, 500,550,
  500,550, 500,550, 500,550, 500,1600, 500,550, 450,550, 500,550, 500,550, 500,1600, 500,1600, 500,1600, 450,550,
  500,1650, 450,550, 500,550, 500,550, 500,550, 450,1650, 450,1600, 500};
/*---------------------------------------------------------------------------------------------------------------------------*/
void handleNewMessages(int numNewMessages) {
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));

  for (int i=0; i<numNewMessages; i++) {
    String chat_id = String(bot.messages[i].chat_id);
    String text = bot.messages[i].text;
    String from_name = bot.messages[i].from_name;
    if (from_name == "") from_name = "Gast";
    if (text == "/on") {
      digitalWrite(ledPin, HIGH);   // turn the LED on (HIGH is the voltage level)
      irsend.sendRaw(an18,59,khz);
      anaus=true;
      bot.sendMessage(chat_id, "Klima ist an.", "");
    }
    if (text == "/off") {
      digitalWrite(ledPin, LOW);    // turn the LED off (LOW is the voltage level)
      irsend.sendRaw(aus,59,khz);
      anaus=false;
      bot.sendMessage(chat_id, "Klima ist aus.", "");
    }
    if (text == "/on18") {
      digitalWrite(ledPin, LOW);    // turn the LED off (LOW is the voltage level)
      irsend.sendRaw(an18,59,khz);
      anaus=true;
      bot.sendMessage(chat_id, "Klima ist an mit 18°C.", "");
    }
    if (text == "/on19") {
      digitalWrite(ledPin, LOW);    // turn the LED off (LOW is the voltage level)
      irsend.sendRaw(an19,59,khz);
      anaus=true;
      settemp=19;
      bot.sendMessage(chat_id, "Klima ist an mit 19°C.", "");
    }
    if (text == "/on20") {
      digitalWrite(ledPin, LOW);    // turn the LED off (LOW is the voltage level)
      irsend.sendRaw(an20,59,khz);
      anaus=true;
      settemp=20;
      bot.sendMessage(chat_id, "Klima ist an mit 20°C.", "");
    }
    if (text == "/on21") {
      digitalWrite(ledPin, LOW);    // turn the LED off (LOW is the voltage level)
      irsend.sendRaw(an21,59,khz);
      anaus=true;
      settemp=21;
      bot.sendMessage(chat_id, "Klima ist an mit 21°C.", "");
    }
    if (text == "/on22") {
      digitalWrite(ledPin, LOW);    // turn the LED off (LOW is the voltage level)
      irsend.sendRaw(an22,59,khz);
      anaus=true;
      settemp=22;
      bot.sendMessage(chat_id, "Klima ist an mit 22°C.", "");
    }
    if (text == "/set18") {
      digitalWrite(ledPin, LOW);    // turn the LED off (LOW is the voltage level)
      irsend.sendRaw(set18,59,khz);
      settemp=18;
      bot.sendMessage(chat_id, "Klima ist auf 18°C gesetzt.", "");
    }
    if (text == "/set19") {
      digitalWrite(ledPin, LOW);    // turn the LED off (LOW is the voltage level)
      irsend.sendRaw(set19,59,khz);
      settemp=19;
      bot.sendMessage(chat_id, "Klima ist auf 19°C gesetzt.", "");
    }
    if (text == "/set20") {
      digitalWrite(ledPin, LOW);    // turn the LED off (LOW is the voltage level)
      irsend.sendRaw(set20,59,khz);
      settemp=20;
      bot.sendMessage(chat_id, "Klima ist auf 20°C gesetzt.", "");
    }
    if (text == "/set21") {
      digitalWrite(ledPin, LOW);    // turn the LED off (LOW is the voltage level)
      irsend.sendRaw(set21,59,khz);
      settemp=21;
      bot.sendMessage(chat_id, "Klima ist auf 21°C gesetzt.", "");
    }
    if (text == "/set22") {
      digitalWrite(ledPin, LOW);    // turn the LED off (LOW is the voltage level)
      irsend.sendRaw(set22,59,khz);
      settemp=22;
      bot.sendMessage(chat_id, "Klima ist auf 22°C gesetzt.", "");
    }
    if (text == "/temp") {
      digitalWrite(ledPin, LOW);    // turn the LED off (LOW is the voltage level)
      ds.requestTemperatures();
      temp=ds.getTempCByIndex(0);
      bot.sendMessage(chat_id, String(temp), "");
    }
    if (text == "/gettemp") {
      digitalWrite(ledPin, LOW);    // turn the LED off (LOW is the voltage level)
      bot.sendMessage(chat_id, String(settemp), "");
    }
    if (text == "/status") {
      digitalWrite(ledPin, LOW);    // turn the LED off (LOW is the voltage level)
      if(anaus)
      {
        bot.sendMessage(chat_id, "Klima ist an.", "");
      }
      else
      {
        bot.sendMessage(chat_id, "Klima ist aus.", "");
      }

    }
    if (text == "/help") {
      String help = "Dies ist die Hilfe für die TS Klimaanlage, " + from_name + ".\n";
      help += "Folgende Befehle sind verfügbar.\n\n";
      help += "/on : zum einschalten der Klimaanlage.\n";
      help += "/off : Zum ausschalten der Klimaanlage.\n";
      help += "/on18: Klimaanlage mit 18°C einschalten.\n";
      help += "/on19: Klimaanlage mit 19°C einschalten.\n";
      help += "/on20: Klimaanlage mit 20°C einschalten.\n";
      help += "/on21: Klimaanlage mit 21°C einschalten.\n";
      help += "/on22: Klimaanlage mit 22°C einschalten.\n";
      help += "/set18: Klimaanlage auf 18°C stellen.\n";
      help += "/set19: Klimaanlage auf 19°C stellen.\n";
      help += "/set20: Klimaanlage auf 20°C stellen.\n";
      help += "/set21: Klimaanlage auf 21°C stellen.\n";
      help += "/set22: Klimaanlage auf 22°C stellen.\n";
      help += "/gettemp: Zeigt eingestellte Temperatur\n";
      help += "/status: Zeigt an ob die Klimaanlage an ist oder nicht\n";
      help += "/temp: Zeigt die Temperatur im Raum an\n";
      bot.sendMessage(chat_id, help, "Markdown");
    }
  }
}
/*---------------------------------------------------------------------------------------------------------------------------*/
void setup() {
    // put your setup code here, to run once:
    Serial.begin(9600);
    irsend.begin();
    ds.begin();
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);
    // WLan-Verbindung aufbauen
    Serial.println();
    Serial.println();
    Serial.print("Verbinde mit ");
    Serial.println(ssid);
    WiFi.begin(ssid,password); //Versucht sich ins WLan anzumelden
    while(WiFi.status() != WL_CONNECTED) { //solange keine WLan-Verbindung besteht
      delay(tickDelay);
      Serial.print(".");              //Alle 500ms einen Punkt ausgeben
      resetTicker++;                  //Alle 500ms Ticker um 1 erhöhen
      if(resetTicker >= 25) {         //Bei 25 versuchen neustarten
        Serial.println(".");
        Serial.println("Verbindung konnte nicht aufgebaut werden");
        delay(3000);
        ESP.restart();               //ESP neustarten
      }
    }
    Serial.println(" ");
    Serial.println("WLan verbunden");
    Serial.println("die IP Adresse lautet: ");
    Serial.println(WiFi.localIP()); //Bei Wlan_Verbindung IP adresse ausgeben
    pinMode(ledPin,OUTPUT);
    pinMode(IR_LED,OUTPUT);
}
/*---------------------------------------------------------------------------------------------------------------------------*/
void loop() {
  if (millis() > Bot_lasttime + Bot_mtbs)  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    while(numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    Bot_lasttime = millis();
  }
}

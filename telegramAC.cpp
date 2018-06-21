#include <Arduino.h>
#include <UniversalTelegramBot.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include "secrets.h"


char ssid[] = SECRET_SSID;     // Hier Netzwerknamen eingeben
char password[] = SECRET_PASS; // Hier Netzwerkpasswort eingeben
const char botToken[]=SECRET_BOT_TOKEN;

WiFiClientSecure client;
UniversalTelegramBot bot(botToken,client);

int resetTicker = 0;            // Reset Ticker, damit ESP sich selbst resetten kann
int postTicker = 0;
int tickDelay = 500;      //Zeit zwischen der Abfrage neuer Nachrichten
int ledPin = D1;

unsigned int Bot_mtbs = 1000; //mean time between scan messages
unsigned long Bot_lasttime;   //last time messages' scan has been done
bool Start = false;

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
      bot.sendMessage(chat_id, "Klima ist an", "");
    }

    if (text == "/off") {
      digitalWrite(ledPin, LOW);    // turn the LED off (LOW is the voltage level)
      bot.sendMessage(chat_id, "Klima ist aus", "");
    }



    if (text == "/help") {
      String help = "Dies ist die Hilfe für die TS Klimaanlage, " + from_name + ".\n";
      help += "Folgende Befehle sind verfügbar.\n\n";
      help += "/on : zum einschalten der Klimaanlage\n";
      help += "/off : Zum ausschalten der Klimaanlage\n";
      bot.sendMessage(chat_id, help, "Markdown");
    }
  }
}

void setup() {
    // put your setup code here, to run once:
    Serial.begin(9600);
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

}

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

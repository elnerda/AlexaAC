include <Arduino.h>
#include <IRremote.h>

int RECV_PIN = 11;

IRrecv irrecv(RECV_PIN);

decode_results results;

void setup()
{
  Serial.begin(9600);
  Serial.println("IR Empfangspin einschalten");
  irrecv.enableIRIn(); // Empfänger starten
}

void loop() {
  if (irrecv.decode(&results)) {
    Serial.println(results.value, HEX);
    irrecv.resume(); // 
  }
  delay(100);
}

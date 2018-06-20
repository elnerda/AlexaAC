#include <Arduino.h>
#include <IRremote.h>

IRsend irsend;

void setup()
{
  Serial.begin(9600);
}

void loop() {
  int khz = 38; // 38kHz carrier frequency for the NEC protocol
  unsigned int aus[] = {8350, 4200, 450, 1650, 450, 550, 500, 550, 500, 550, 500, 1600, 500, 550, 450, 600, 450, 550, 500, 1650, 450, 1600, 500, 550, 500, 550, 450, 600, 450, 550, 500, 550, 500, 550, 500, 550, 500, 550, 500, 550, 500, 550, 450, 550, 500, 1650, 450, 550, 500, 1600, 500, 550, 500, 550, 450, 550, 500, 1650, 450}; //AnalysIR Batch Export (IRremote) - RAW
  unsigned int an[]={8300,4200, 500,1600, 500,550, 500,550, 450,550, 500,1650, 450,550, 500,550, 500,550, 500,550, 500,550, 450,550, 500,550, 500,550, 500,550, 500,550, 500,550, 500,550, 500,500, 500,1650, 450,1600, 500,550, 500,1600, 500,500, 500,550, 500,550, 500,1600, 500,1600, 500,1600, 450};
  irsend.sendRaw(aus, 59, khz); //Note the approach used to automatically calculate the size of the array.

  delay(2000); //In this example, the signal will be repeated every 5 seconds, approximately.
  Serial.println("aus");
  irsend.sendRaw(an, 59, khz);
  delay(2000);
  Serial.println("an");

}

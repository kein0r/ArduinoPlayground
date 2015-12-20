#include<Time.h>
#include<FastLED.h>
#define NUM_LEDS 16
#define DATA_PIN 6

CRGB leds[NUM_LEDS];


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  setTime(12412);
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);

}

void loop() {
  for(int dot = 0; dot < NUM_LEDS; dot++)
  { 
    leds[dot] = CRGB::Blue;
    FastLED.show();
    // clear this led for the next time around the loop
    leds[dot] = CRGB::Black;
    delay(5);
  }
}

#include<Time.h>
#include<FastLED.h>
#define NUM_LEDS 24
#define DATA_PIN 6

CRGB leds[NUM_LEDS];

/*
 * There are 24 LEDs to display 60 seconds or minutes. This means 24/60 = 0,4 steps per second/minute. In order
 * to make the movement as smoze as possible two LEDs will be used to interpolate between the steps. The brightness
 * of those two LEDs should always sum up to a defined value.
 * With 0,4 steps (that is 0,0 0,4 0,8 1,2 1,6 2,0) only every second LED will match completely. Therefore we need
 * two separate patterns for even and odd LEDs.
 */
typedef struct {
  uint8_t primary;
  uint8_t seconday;
} clockLED_t;

const clockLED_t evenDisplayPattern[3] = {
  {10,0},
  {60,40},
  {20,80}
};

const clockLED_t oddDisplayPattern[2] = {
  {80,20},
  {40,60}
};

void setup() {
  Serial.begin(115200);
  setTime(12412);
  // put your setup code here, to run once:
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);

}

void loop() {
  uint8_t actHour = hourFormat12();
  uint8_t actMinute = 10 * 24 * minute() / 60;
  uint8_t actSecond = 10 * 24 * second() / 60;

  leds[actHour] = CRGB::Red;
  leds[actMinute] = CRGB::Green;
  leds[actSecond] = CRGB::Blue;
  FastLED.show();
  Serial.print("H: "); Serial.print(actHour);Serial.print(" -> "); Serial.print(actHour);
  Serial.print(" M: "); Serial.print(actMinute);Serial.print(" -> "); Serial.print(actMinute);
  Serial.print(" S: "); Serial.print(second());Serial.print(" -> "); Serial.print(actSecond);
  Serial.println();
  delay(15);
  leds[actHour] = CRGB::Black;
  leds[actMinute] = CRGB::Black;
  leds[actSecond] = CRGB::Black;
}

#include<Time.h>
#include<FastLED.h>
#define NUM_LEDS        24 /* How many LEDs are in the ring */
#define SECONDS_MAX     60 /* How many seconds are in one minute */
#define MINUTES_MAX     60 /* How many minutes are in one hour */
#define DATA_PIN 6

CRGB leds[NUM_LEDS];

/*
   There are 24 LEDs to display 60 seconds or minutes. This means 24/60 = 0,4 steps per second/minute. In order
   to make the movement as smoze as possible two LEDs will be used to interpolate between the steps. The brightness
   of those two LEDs should always sum up to a defined value.
   With 0,4 steps (that is 0,0 0,4 0,8 1,2 1,6 2,0) only every second LED will match completely. Therefore we need
   two separate patterns for even and odd LEDs.
*/
#define LED_BRIGHTNESS_STEP   (24*255)/60
typedef struct {
  uint8_t primary;
  uint8_t seconday;
} clockLED_t;

const clockLED_t evenDisplayPattern[3] = {
  {10, 0},
  {60, 40},
  {20, 80}
};

const clockLED_t oddDisplayPattern[2] = {
  {80, 20},
  {40, 60}
};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  setTime(12412);
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
}

uint8_t secondIndex;
uint8_t actSecondBrightness;
uint8_t minuteIndex;
uint8_t actMinuteBrightness;

void loop() {
  uint8_t hourIndex = hourFormat12();

  /* Get current time */
  /* TODO */
  /* Calculate for minutes and seconds the main LED and brightness. See Excel file
   * how to brightness is calculated
   * =(1-MOD(Second;5)*0,4)*255+MOD(Index;2)*255 = (255-255*MOD(A2;5)*0,4)+MOD(D2;2)*255
   */
  secondIndex = NUM_LEDS * second() / SECONDS_MAX;
  actSecondBrightness = 255 - 255 * (second() % 5) * NUM_LEDS/SECONDS_MAX + ((secondIndex % 2) * 255);
  minuteIndex = NUM_LEDS * minute() / SECONDS_MAX;
  actMinuteBrightness = 255 - 255 * (minute() % 5) * NUM_LEDS/MINUTES_MAX + ((secondIndex % 2) * 255);

  leds[hourIndex] = CRGB::Red;
  leds[minuteIndex].green = actMinuteBrightness;
  leds[minuteIndex + 1].green = 255 - actMinuteBrightness;
  leds[secondIndex].blue = actSecondBrightness;
  leds[secondIndex + 1].blue = 255 - actSecondBrightness;
  FastLED.show();
  Serial.print("H: "); Serial.print(hour()); Serial.print(" -> "); Serial.println(hourIndex);
  Serial.print(" M: "); Serial.print(minute()); Serial.print(" -> "); Serial.print(minuteIndex);Serial.print(" -> "); Serial.print(actMinuteBrightness);Serial.print(" "); Serial.println(255-actMinuteBrightness);
  Serial.print(" S: "); Serial.print(second()); Serial.print(" -> "); Serial.print(secondIndex); Serial.print(" -> "); Serial.print(actSecondBrightness);Serial.print(" "); Serial.println(255-actSecondBrightness);
  Serial.println();
  delay(500);
  leds[hourIndex] = CRGB::Black;
  leds[minuteIndex] = CRGB::Black;
  leds[minuteIndex + 1] = CRGB::Black;
  leds[secondIndex] = CRGB::Black;
  leds[secondIndex + 1] = CRGB::Black;
}

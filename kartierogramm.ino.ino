#include <FastLED.h>

#define NUM_LEDS_PER_STRIP 450
#define DATA_PIN 6
CRGB leds[NUM_LEDS_PER_STRIP];

int currentLED=0;
int buttonApin=4;

void setup() {
  // put your setup code here, to run once:
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS_PER_STRIP);
  pinMode(buttonApin, INPUT_PULLUP);
}

void loop() {
  if (digitalRead(buttonApin) == LOW) {
    moveByOne();
  }
}

void moveByOne() {
  leds[currentLED] = CRGB::Black;  // set our current dot to black before we continue
  currentLED++;
  leds[currentLED] = CRGB::Red;
  FastLED.show();
   
}

void autoMove() {
   for(int i = 0; i < NUM_LEDS_PER_STRIP; i++) {
    leds[i] = CRGB::Red;    // set our current dot to red
    FastLED.show();
    delay(50);
    leds[i] = CRGB::Black;  // set our current dot to black before we continue
    delay(2000);
  }

}


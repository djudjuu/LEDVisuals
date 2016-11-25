#include <FastLED.h>

#define NUM_LEDS 450
#define DATA_PIN 6
#define NUM_BEAMS 1
CRGB leds[NUM_LEDS];
int currentLED=0;
int tau=0;
int breite=3;
int BO[NUM_BEAMS];


void setup() {
  // put your setup code here, to run once:
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  for (int i=0; i<NUM_LEDS; i++){
    leds[i] = CRGB::Black;
  }
  FastLED.show();
}

void loop() {
  Beam(tau,0,breite,0);
  delay(1000);
  tau++;
  }

int* mapSeg(int x){
  int* = x{0}; 
}


void Beam (int t, int b_id, int breite, int spawn){
  if (tau>spawn){
    int ort = BO[b_id];
    int* segleds;
    int inc = 256/(breite/2 +1);
    for (int i=ort-1; i>=ort-breite/2; i--){ //Ausgehender Ball
       segleds=mapSeg(i);
      for (int led : segleds)
        leds[led].fadeToBlackBy(+inc);
      
  }
    for (int i=ort; i<=ort+breite/2; i++){ //aufgehender Ball
      segleds=mapSeg(i);
      for (int led : segleds){
        leds[led].fadeToBlackBy(-inc);
      }
    }
    BO[b_id]++;
    FastLED.show();
  }
}


void moveByOne() {
  leds[currentLED] = CRGB::Black;  // set our current dot to black before we continue
  currentLED++;
  currentLED = currentLED % 450;
  Serial.begin(9600);
  Serial.println(currentLED);
  leds[currentLED] = CRGB::Red;
  FastLED.show();
   
}

void autoMove() {
   for(int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Red;    // set our current dot to red
    FastLED.show();
    delay(50);
    leds[i] = CRGB::Black;  // set our current dot to black before we continue
    delay(2000);
  }

}


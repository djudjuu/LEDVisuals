#include <utility.h>
#include <StandardCplusplus.h>
#include <unwind-cxx.h>
#include <system_configuration.h>


#include <FastLED.h>
#include <string>
#include <vector>


using namespace std;

#define NUM_LEDS 450
#define DATA_PIN 6
#define NUM_BEAMS 1
//initializations
CRGB leds[NUM_LEDS];
int currentLED=0;
int tau=0;
int breite=3;
int BO [NUM_BEAMS] = { }; //another way to allocate storage and initialize with zeros
vector<int> LEDkarte; //= readLEDmap(pathLEDmap);
string pathLEDmap = "./LEDkarte.txt";
//create a lookupTable for LEDpositions and fill it
vector<vector<int>> seg2ledsLookup;
fillSegLookUp()
int NUM_SEG = seg2ledsLookup.size()

void setup() {
  //initializing Leds
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

void SegLookUp(){ 
	for (int i=0; i <= LEDkarte.size(); i++)
	{
		seg2ledsLookup[LEDkarte[i]].push_back(i);
	}
}

int* mapSeg(int seg){
//return a vector of ints that represent all indeces of the LEDs in the given segement
	return seg2ledsLookup[seg];
} 

void Beam (int t, int b_id, int breite, int spawn){
/*creates a beam that starts at a given time spawn,
runs through by manipulating the brightness of the LEDS and 
 restart from beginning when last segment is reached
*/ 
  if (tau>spawn){
    int ort = BO[b_id];
    int* segleds;
    int inc = 256/(breite/2 +1);
    for (int i=ort-1; i>=ort-breite/2; i--){ //Ausgehender Ball
	i = i % NUM_SEG //restart
	segleds=mapSeg(i);
	for (int led : segleds)
		leds[led].fadeToBlackBy(+inc);
      }
    for (int i=ort; i<=ort+breite/2; i++){ //aufgehender Ball
	i = i % NUM_SEG //restart
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


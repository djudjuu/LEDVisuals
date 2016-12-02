#include <utility.h>
#include <StandardCplusplus.h>
#include <unwind-cxx.h>
#include <system_configuration.h>
#include <vector>
#include <serstream>
#include <avr/pgmspace.h>

#include <FastLED.h>
#define NUM_LEDS 31
#define DATA_PIN 6
using namespace std;

CRGB leds[NUM_LEDS];

//int LEDmapBastiFlur [450] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,2,2,2,2,3,3,3,3,3,3,3,3,3,3,3,3,3,4,4,4,4,4,4,4,4,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,6,6,6,6,7,7,7,7,7,7,7,8,8,8,8,8,8,8,8,8,8,8,8,9,9,9,9,9,9,10,10,10,10,10,10,10,10,11,11,11,11,11,11,11,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,11,11,11,11,11,11,11,10,10,10,10,10,10,9,9,9,9,9,9,9,9,9,9,9,9,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,9,9,9,9,9,9,9,10,10,10,10,10,10,10,10,11,11,11,11,11,11,12,12,12,12,12,12,12,12,12,13,13,13,13,13,13,13,14,14,14,14,14,15,15,15,15,15,15,15,16,16,16,16,16,16,17,17,17,17,17,17,17,17,18,18,18,18,18,19,19,19,19,19,19,20,20,20,20,20,20,20,20,21,21,21,21,21,21,22,22,22,22,22,22,22,23,23,23,23,23,23,23,23,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,23,23,23,23,23,23,23,22,22,22,22,22,22,22,21,21,21,21,21,20,20,20,20,20,19,19,19,19,19,19,19,18,18,18,18,18,18,17,17,17,17,17,17,17,17,18,18,18,18,18,18,18,19,19,19,19,19,19,19,19,19,19,19};
int LEDkarte [31] = {0,0,1,1,2,2,3,3,4,4,5,5,6,6,7,7,8,8,9,9,10,10,11,11,12,12,13,13,14,14,15};
const int nSegs = 15;

//---------- initialization ------------
//create a lookupTable for LEDpositions and fill it
// list of pointers to vector<int>s
vector<int>* seg2ledsLookup [nSegs];

void fillSegLookUp(){
  // create one vector for each segment
  for (int i=0; i <= nSegs; i++) 
  {
    vector<int> segVec(0); //create new empty vector for the segment
    //now go over the LEDS and push their locations on the segment-vector
    for (int j=0; j <= sizeof(LEDkarte); j++) // n
    {
      segVec.push_back(j);
    }
  //now save the adress of the segVec in the lookupTable
  seg2ledsLookup[i] = &segVec;
  }
}


vector<int>* mapSeg(int seg){
  //returns a pointer to a vector of ints that represent all indeces of the LEDs in the given segement
  return seg2ledsLookup[seg];
} 

void setup() {
  // put your setup code here, to run once:
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  for (int i=0; i<NUM_LEDS; i++){
    leds[i] = CRGB::Black;
  }
  fillSegLookUp();
  FastLED.show();
}

void loop() {
  // put your main code here, to run repeatedly:
  //autoMove();
  vector<int>* segleds;
  for ( int i; i<=nSegs;i++){
      segleds = mapSeg(i);
        for (int j=0; j<segleds->size();j++){
          leds[ (*segleds)[j] ] = CRGB::Red;
        }
        FastLED.show();
        delay(1000);
  }
  
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


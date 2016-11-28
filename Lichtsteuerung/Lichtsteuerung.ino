#include <utility.h>
#include <StandardCplusplus.h>
#include <unwind-cxx.h>
#include <system_configuration.h>

#include <FastLED.h>
#include <string>
#include<vector>


using namespace std;

//---------- constants------------
#define NUM_LEDS 31
#define DATA_PIN 6
#define NUM_BEAMS 1
#define NUM_SEG 16
CRGB leds[NUM_LEDS];
int tau=100;
int breite=3;
int BO [NUM_BEAMS] = { }; //another way to allocate storage and initialize with zeros
int currentLED=0; //old leftover

//---------- variables we need ------------
//vector<int> LEDLocationsList; //holds the untreated list [0 0 1 1 0 ...] 
int LEDkarte [31] = {0,0,1,1,2,2,3,3,4,4,5,5,6,6,7,7,8,8,9,9,10,10,11,11,12,12,13,13,14,14,15};
const int nSegs = 16;

//---------- initialization ------------
//create a lookupTable for LEDpositions and fill it
// list of pointers to vector<int>s
vector<int>* seg2ledsLookup [nSegs];
//string pathLEDmap = "../LEDkarte.txt"; 


//---------- functions ------------

vector<int>* mapSeg(int seg)
{
	//returns a pointer to a vector of ints that represent all indeces of the LEDs in the given segement
	return seg2ledsLookup[seg];
} 

void fillSegLookUp(){
	//create one vector for each segment
	for (int s=0; s <= nSegs; s++) 
	{
		//cout << "segment"<< s<< "holds LEDS: ";
		vector<int>* segVec = new vector<int>(); //create new empty vector for the segment//TODO use new operation
		//now go over the LEDS and push their locations on the segment-vector
		for (int j=0; j < NUM_LEDS; j++)
		{
		if (LEDkarte[j]==s){
				segVec->push_back(j);
				//cout << segVec->back()<<',';
			}
		}
		//now save the adress of the segVec in the lookupTable
		seg2ledsLookup[s] = segVec;

		//cout << segVec->size()<<endl;
	}
	//cout << "firstnumber is"<<(*seg2ledsLookup[0])[0]<<endl;
	//return 1;
} 

void setup() {
	//initializing Leds
	FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
	for (int i=0; i<NUM_LEDS; i++)
	{
		leds[i] = CRGB::Black;
	}
	FastLED.show(); 
	fillSegLookUp();
}

void loop() {
  //Beam(tau,0,breite,0);
  //delay(1000);
  //tau++;
  // sample code to test the segment control:
	vector<int>* segleds;
	for ( int i=0; i <= nSegs; i++)
	{
		segleds = mapSeg(i);
		//cout << "seg"<< i<< "holds"<<segleds->size();
		//cout << "elements and its LEDS-indeces are:";
		for (int i : *segleds)
			//cout << i<<',';
			leds[ i ] = CRGB::Red;
		//cout <<"ende segment"<< endl;
	FastLED.show(); 
	delay(1000);
	}
  }

void Beam (int t, int b_id, int breite, int spawn){
/*creates a beam that starts at a given time spawn,
runs through by manipulating the brightness of the LEDS and 
 restart from beginning when last segment is reached
*/ 
  if (tau>spawn){
    int ort = BO[b_id];
    vector<int>* segleds;
    int inc = 256/(breite/2 +1);
    for (int i=ort-1; i>=ort-breite/2; i--){ //Ausgehender Ball
	i = i % NUM_SEG; //restart
	segleds=mapSeg(i);
	for (int led : *segleds)
		leds[led].fadeToBlackBy(+inc);
      }
    for (int i=ort; i<=ort+breite/2; i++){ //aufgehender Ball
	i = i % NUM_SEG; //restart
	segleds=mapSeg(i);
	for (int led : *segleds){
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


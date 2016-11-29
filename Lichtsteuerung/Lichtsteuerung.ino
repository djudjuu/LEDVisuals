#include <utility.h>
#include <StandardCplusplus.h>
#include <unwind-cxx.h>
#include <system_configuration.h>

#include <FastLED.h>
#include <string>
#include<vector>
#include <math.h>


using namespace std;

//---------- constants------------
#define NUM_LEDS 31
#define DATA_PIN 6
#define NUM_BEAMS 1
#define NUM_SEG 16
CRGB leds[NUM_LEDS];
CHSV colors[NUM_LEDS];
int tau=0; //starttime
int simTime=100;  //endtime
int breite=3;
int BO [NUM_BEAMS]; //another way to allocate storage and initialize with zeros
int currentLED=3; //old leftover

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
    leds[i].setRGB(0,0,0);
  	//leds[i]= CHSV( 230, 200, 200);
    colors[i]= CHSV( 160, 255, 0);

	}
 //initializing LEDlookup
  fillSegLookUp();
  
 //setting the initial beam Positions"
  BO[0]=0;
	
	FastLED.show(); 

}

void loop() {
  simpleBeam(100);
  /*if (tau<simTime && 0)
  {
    Beam(tau,0,3,0);
    delay(1000);
    tau++;
  }
  */
}
   
void simpleBeam(int traveltime){
	  /* sample code to test the segment control by sending a simple beam 
     * with breite 1 
     * of a color (default is red)//TODO make color customizable
     * that will travel with a given traveltime ( will be used as delay)-> small values make the beam faster
	   */
  vector<int>* segleds;
  vector<int>* trailingSegleds;
  //    if (color){      color = CRGB::Red; }
	for ( int i=0; i <= nSegs; i++)
	{
		segleds = mapSeg(i);
    trailingSegleds = mapSeg(i-1);
		for (int led : *segleds){//leading LED
			leds[ led ] = min(255,colors[led].v+50);//increase but not to more than 255
		}
    for (int led2 : *trailingSegleds){//trailing LED
      //leds[ led2 ] = CRGB::Black;
		  //leds[ led2 ].fadeToBlackBy(125);does not fade out completely
      //leds[ led2 ].nscale8(128); //somehow this brightens again...maybe the color does some kind of modulo if not set to exactly 0
      //colors[led2].setVal(200);
      leds[led2] = max(0,colors[led2].v-50);//decrease but not to less than 0
    }
	FastLED.show(); 
  delay(traveltime);
	
	}
 }

void Beam (int tick, int b_id, int breite, int spawn){
/*creates a beam that:
 * whose current location is in BO[b_id] (<- needs to be set before first call)
 * is of total width=breite
 * starts at a given time spawn,
 * tick is the current time of the simulation
runs through by manipulating the brightness of the LEDS and 
 restart from beginning when last segment is reached
*/ 
  //if (tick>=spawn){
    int ort = BO[b_id];
    vector<int>* segleds;
    int inc = floor(256/(floor(breite/2) +1));
    int beamReach = floor(breite/2);  // Beam: -beamreach<-centerLED->+beamReach
    for (int i=ort-1; i>=ort-beamReach-1; i--)
    { //Ausgehender Ball
	    //i = i % NUM_SEG; //restart
	    segleds = mapSeg(i);
	    for (int led : *segleds)
		    //leds[led].fadeToBlackBy(+inc);
        leds[led] = CRGB::Black;
    }
    for (int i=ort; i<=ort+beamReach; i++)
    { //aufgehender Ball
	  //i = i % NUM_SEG; //restart
	  segleds = mapSeg(i);
	  for (int led : *segleds)
	    {
      leds[led] = CRGB::Red;
		  //leds[led].fadeToBlackBy(-inc);
      }
    }
    BO[b_id]++;
    FastLED.show();
    
  //}
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


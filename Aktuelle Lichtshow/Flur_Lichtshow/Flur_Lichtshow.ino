#include "FastLED.h"

FASTLED_USING_NAMESPACE

#define NUM_LEDS 450
#define DATA_PIN 6

CRGB leds[NUM_LEDS];

const int nSegs = 24;
CHSV vleds[nSegs+1];
const int nLimits = 44; 

#define LED_TYPE    WS2811
#define COLOR_ORDER GRB

#define BRIGHTNESS          96
#define FRAMES_PER_SECOND  120

int limits  [nLimits] = {0,1,2,1,2,3,4,5,6,7,8,9,10,11,12,11,10,9,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,23,22,21,20,19,18,17,18,19};
int limit2seg [nLimits] = {15,22,45,69,74,87,95,149,153,160,172,178,186,193,222,229,235,247,262,269,277,283,292,299,304,311,317,325,330,336,344,350,357,365,387,394,401,406,411,418,424,432,439};

void setup() {
  delay(3000); // 3 second delay for recovery
  
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  //FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
}


// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = {bouncingBeam, confetti,fadingBeam, sinelon, juggle, beam, theaterChase, theaterChaseRainbow};

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns
  
void loop()
{
  // Call the current pattern function once, updating the 'leds' array
  gPatterns[gCurrentPatternNumber]();

  // send the 'leds' array out to the actual LED strip
  FastLED.show();  
  // insert a delay to keep the framerate modest
  FastLED.delay(1000/FRAMES_PER_SECOND); 

  // do some periodic updates
  EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow
  EVERY_N_SECONDS( 10 ) { nextPattern(); } // change patterns periodically
}

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
}

int getVirtual(int rled)
{
  for (int l=0;l<nLimits;l++)
  {
    if (rled < limit2seg[l]) 
    {
      return limits[l];
    }
  } 
}

void fire(){
  fire(40);
}

void fire(int d)
{
  for (int i=0; i<NUM_LEDS; i++)
  {
    leds[i] = vleds[getVirtual(i)];
  }
  FastLED.show();
  delay(d);
}
void confetti() 
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( gHue + random8(64), 200, 255);
}

void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 20);
  int pos = beatsin16(7,0,NUM_LEDS);
  leds[pos] += CHSV( gHue, 255, 192);
}

void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  byte dothue = 0;
  for( int i = 0; i < 8; i++) {
    leds[beatsin16(i+7,0,NUM_LEDS)] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}
void beam(){
  for (int i=0; i<nSegs+1; i++)
  {
    if (i==0)
    {
      vleds[i]= CHSV(gHue, 255, 255);
      vleds[nSegs]=CHSV(gHue, 255, 0);
      fire();
    }
    else
    {
      vleds[i]= CHSV(gHue, 255, 255);
      vleds[i-1]=CHSV(gHue, 255, 0);
      fire();
    }
  }
}


void theaterChase() {
  uint8_t wait = 50;
  for (int j=0; j<20; j++) {
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < NUM_LEDS; i=i+3) {
        leds[i+q] = CHSV(gHue, 255, 255);    //turn every third pixel on
      }
      FastLED.show();
      delay(wait);
      for (uint16_t i=0; i < NUM_LEDS; i=i+3) {
        leds[i+q] = CRGB::Black;        //turn every third pixel off
      }
    }
  }
}
void theaterChaseRainbow() {
  uint8_t wait = 50;
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < NUM_LEDS; i=i+3) {
        leds[i+q]=CHSV(gHue+j, 255, 255);    //turn every third pixel on
      }
      FastLED.show();

      delay(wait);

      for (uint16_t i=0; i < NUM_LEDS; i=i+3) {
        leds[i+q]=CRGB::Black;     //turn every third pixel off
      }
    }
  }
}
int bounceIndex(int tau, int d){
  int m = nSegs;
  if (d>0){
    return tau % m;
  }else{
    return(nSegs- (tau % m)); 
  }
}

int getDelay(int tau){
  int tempi=5;
  return 2 + (int(floor(tau/50))%tempi)*5;
}
void bouncingBeam()
{
     /* makes a beam bounce back and forth
      * basically it goes
      * 1) compute the index to turn off
      * 2) increase counters
      * 3) compute where to turn on next
      * 4) call fire
      * */
     int bounces = 0; //counts how often the beam went back and forth
     int d = 1; // direction: either -1 or +1
     int tau = 1; //der takt
     int myHue = gHue;
     //while (bounces<3){//use this for 3 iterations
     while (tau<500){ //use to make it shorter or longer
     int offIndex = bounceIndex(tau,d);
     tau++;
     if (tau%(nSegs)==0){ //if beam reached some end of the segment
        bounces++;  //remember and
        d = d*(-1); //switch direciton
        myHue=myHue+10;
     }
         int onIndex = bounceIndex(tau,d);
     //fire
     vleds[onIndex]= CHSV(myHue, 255, 255);
     //vleds[offIndex]=CRGB::Black;
     vleds[offIndex].val=0;
     fire(getDelay(tau));
     //cout << onIndex<< "  "<<offIndex << endl;
     }
}

void fadingBeam(){
  int breite=5;
  int speeed=20;
  //int d = 1; // direction: either -1 or +1
  //int inc = floor(256/(floor(breite/2) +1));
  int tau = 0; //der takt
  int inc = floor(256/speeed);
  //int beamReach = floor(breite/2);  // Beam: -beamreach<-centerLED->+beamReach
  int onLED =0;
  int offLED;
  int myHue=gHue;
  CHSV front = CHSV(myHue,255,2);
  CHSV back = CHSV(myHue,255,0);
  int prevSeg=0;
  while (tau<3000){//change this to make it shorter or longer
    prevSeg=onLED;
    onLED = floor(tau/speeed);
    if (onLED != prevSeg){
      front.val=2;
      back.val=255;
      myHue=myHue+3;
    }
    //float intensity = (tau%speeed)/speeed;
    if (onLED<nSegs){
      //front.val = floor(intensity*255);
      //front.hue = front.hue+10;
      front.val = front.val+inc;
      front.hue =myHue;
      vleds[onLED] = front;
    }
    if ((onLED-breite)>=0){
      int offLED = max(0,onLED-breite);
      //back.val = floor((1-intensity)*255);
      back.hue = myHue;
      int tmp = back.val-inc;
      if (tmp<0){
        tmp=0;
      }
      back.val = tmp;
      vleds[offLED] = back;//.val = vleds[onLED].val-inc;
      for (int b=offLED-1; b>=0; b--){
        //vleds[b]=CRGB::Black;
      }
    }
    tau++; 
    if (tau> speeed*(nSegs+breite+1)){//restart
      tau=0;
      //myHue=myHue+33;
    }
    fire(1);
  }
} 


  


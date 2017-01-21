
/*
  Beam.h - Library for creating LED beams
*/

// ensure this library description is only included once
#ifndef Beam_h
#define Beam_h

// include types & constants of Wiring core API
#include "Arduino.h"
#include <FastLED.h>

#include <utility.h>
#include <StandardCplusplus.h>
#include <unwind-cxx.h>
#include <system_configuration.h> 
#include <string>
#include <vector>
#include <math.h>

using namespace std;

class Beam
{
  // user-accessible "public" interface
  public:
    Beam(CHSV* vleds, int nLeds,
	   int start_location, int start_direction,
	   int beamSize, int hue); 
    void move(int tau);
    void toggle();
    void fire();
    int color;
    int color2;
    int flag;
    CHSV* leds;
    void bounce(int tau);
    void cross(int t);
    void setVal(int idx, int val);
    void setHue(int idx, int color);
    void addHue(int idx, int color);
    void off();

  // library-accessible "private" interface
  private:
    int _centerLED;
    int _speed;
    int _width;
    int _d; //direction of the beam
    bool _active;
    int _nLeds;
    int bounceIndex(int tau);
    
};

#endif

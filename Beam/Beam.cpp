/*
  Beam.cpp -  Library for creating LED beams
*/

// include core Wiring API
#include "Beam.h"
using namespace std;

// Constructor /////////////////////////////////////////////////////////////////
// Function that handles the creation and setup of instances

Beam::Beam(CHSV* vleds, int nLeds,
	   int start_location, int start_direction,
	   int beamSize, int hue) 
{
  // initialize this instance's variables
  color = hue;
  flag=0;
  color2 = hue+60;
  leds = vleds;
  _centerLED = start_location;
  _d = start_direction;
  _width = beamSize;
  _active = 1;
  _nLeds = nLeds;
  _speed = 10; //increments are needed from dark to bright 
  for (int i=0;i<_nLeds;i++)
  {
	leds[i] = CHSV(50,255,0);
  }

}


// Public Methods //////////////////////////////////////////////////////////////
// Functions available in Wiring sketches, this library, and other libraries

void Beam::setVal(int idx, int val){
	if (0 <= idx <=_nLeds){
		leds[idx].val = leds[idx].val+ val;
	}
}
void Beam::setHue(int idx, int color){
	if (0 <= idx <=_nLeds){
		leds[idx].hue = color;
	}
}
void Beam::addHue(int idx, int color){
	if (0 <= idx <=_nLeds){
		leds[idx].hue = leds[idx].hue+color;
	}
}

void Beam::fire()
{
	for (int i=0;i<_nLeds-2;i++)
	{
		leds[i].val=200;
		leds[i].hue=200;
	}
}

void Beam::off()
{
	for (int i=0;i<_nLeds;i++)
	{
		leds[i].val=0;
		leds[i].hue=0;
	}
}



void Beam::move(int tau)
{

	//move centerLED forward
	//_centerLED = _centerLED +(1*_direction);
	int i = _centerLED;
        int inc = floor(256/_speed); 
	//if (_centerLED = 0
	//fade out the trailing half of the beam
	//vleds[i].val = vleds[i].val - inc;
	//vleds[i].hue + color;
	// brighten the leading bit
	// add hue to all leds belonging to the beam
	for (int j=i;j>=i-(_width*_d);j-_d){
		setHue(j,color);
	}
	//smooth increasing of the front brightness:
	setVal(i+1*_d,(tau%_speed)*inc);
	//vleds[i].val = 255;
	//for (int j=i-_d; j>i-(_width*_d); j-_d ) 
	//decrease brightness from full (index i)
	//to 1/width-th of the brightness in the schweif
	//of the beam: 
	for (int j=0; j< _width; j++){
		int index = i -_d*j;
		setVal(index, int(255*((_width-j)/_width)));
	}
	//make the last one fade out smoothly too:
	int last = i-_d*_width;
	setVal(last, 255 - (_speed-(tau%_speed))*inc);
	//make sure the last one is really out:
	setVal(last-_d,0); 
	//move the centerLED
	if (tau%_speed==0){
		_centerLED = _centerLED + _d;
		//check if it reached one of the ends
		if (_centerLED > _nLeds+_width or 
				_centerLED <0-_width){
			_d = _d*(-1); 
			_centerLED = _centerLED + _d*_width;
		}
	} 
}

int Beam::bounceIndex(int tau){
	if (_d>0){
		return tau % _nLeds;
	}else{
		return(_nLeds- (tau % _nLeds)); 
	}
}

void Beam::bounce(int tau)
{
	//leds[_centerLED].val =200; //works
	//one beam	
	_centerLED = tau%_nLeds;
	//setVal(_centerLED,200);
	
	//reverse beam	
	int c2 = _nLeds - (tau%_nLeds-1);
	setVal(c2,200);

	//if (tau % _nLeds == 0 ){
	//	_d = _d*-1;
	//}
	//setVal(_centerLED,220);
	//turns everyting on and off
	//leds[bounceIndex(tau-1)].val =30; 
	//leds[bounceIndex(tau)].val =200;
	//setVal(_centerLED,200);
}


void Beam::cross(int t)
{
       	int speeed=20;
	int tau = t % (speeed*(_nLeds+_width+1));
        int inc = floor(250/speeed);
        int onLED2 =_nLeds;
	int onLED1 = floor(tau/speeed); 
	if(onLED1 < _nLeds+_width){
		onLED2 = _nLeds - onLED1;
		setVal(onLED1,inc);
		setVal(onLED2,inc);
		if (onLED1 ==onLED2){
			flag=_width;
		}
		if (flag>0){
			addHue(onLED2,color2);
			addHue(onLED1,color);
			flag--;
		} else {
			setHue(onLED2,color2);
			setHue(onLED1,color);
		}
	}
	if ((onLED1 - _width)>=0)
	{
		//forward tail
		int offLED1 = max(0,onLED1-_width);
		setVal(offLED1, -inc);
		//backward  tail
		int offLED2 = min(_nLeds,onLED2+_width);
		setVal(offLED2, -inc);
	}
	if (tau == speeed*(_nLeds+_width))
	{
		color = color + 53;
		color2 = color2 - 113;
	}
}


              


void Beam::toggle()
{
	_active = !_active;
} 
// Private Methods /////////////////////////////////////////////////////////////
// Functions only available to other functions in this library




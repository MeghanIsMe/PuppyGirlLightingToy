/* **********************************************************
	FILE: main.cpp
Contains driving code for running aRGB effects on hardware configured on my desktop PC case.

	AUTHOR: Meghan Allen

	ASCII fonts for large comments: 
https://www.asciiart.eu/text-to-ascii-art - pagga

	TO-DOs
Add default speeds (and other default values like fade percentage in SpinColorWaveFadeFans)
************************************************************/

// dependancy inclusions
#include "FastLED.h"				// for backend hardware interfacing
#include "globalsfordoggos.h"		// global settings like number of leds per type of fan and similar
#include "ledcontrolfordoggos.h"	// classes and functions that support and control the lighting effects

// constants to make it easier to see what pin is being used by FastLED to connect to arrays
// this is based on hardware setup - how many and what kind of LEDs are connected to the controller
// is fixed by hardware configuration
#define DATA_PIN2 2
#define DATA_PIN4 4
#define DATA_PIN5 5
#define DATA_PIN7 7
#define DATA_PIN8 8
#define DATA_PIN9 9

// These are the arrays that are written to hardware by FastLED.show at the end of each main loop
// They have a 1-1 correspondence to the aRGB LED hardware
CRGB stripLeds[LINEARSTRIPLEDS];	// front LED strip is on pin 9
CRGB aspect0Leds[ASPECTFANLEDS];	// front bottom fan is on pin 2
CRGB aspect1Leds[ASPECTFANLEDS];	// front top fan is on pin 4
CRGB aspect2Leds[ASPECTFANLEDS];	// back case fan is on pin 8
CRGB cpuFanLeds[CPUFANLEDS];		// CPU fan is on pin 7

// Instantiating the object that does most of the work of setting LED colors
full_SystemLeds systemLeds;
// Instantiating the system timer
system_Timer systemTimer;

// Declaring test functions: each one will run the named effect on all relevant devices
void TestFadeThroughColorsFans(int, const CRGB*);
void TestSpinColorWave(int, const CRGB*);
void TestSpinColorWaveFadeFans(int, const CRGB*);  //REMOVE AND REPLACE SINGLELEDCHASE
void TestSpinLeds(int, CRGB, CRGB = CRGB::Black, CRGB = CRGB::Black);
void TestSpinOneLed(int speed, const CRGB*);
void TestMovingLine(int, const CRGB*);

void RandomTest();


void setup() 
{
	delay(5000);			// power on safety delay
	Serial.begin(9600);		// for serial debugging

	// Associating arrays with FastLED. These arrays will be written out to hardware at the end of each main loop
	FastLED.addLeds<WS2812B, DATA_PIN2, GRB>(aspect0Leds,6);	// aspect0Leds array (bottom front fan) is on pin 2
	FastLED.addLeds<WS2812B, DATA_PIN4, GRB>(aspect1Leds,6);	// aspect1Leds array (top front fan) is on pin 4
	FastLED.addLeds<WS2812B, DATA_PIN8, GRB>(aspect2Leds,6);	// aspect2Leds array (back case fan) is on pin 8
	FastLED.addLeds<WS2812B, DATA_PIN7, GRB>(cpuFanLeds,4);		// cpuFanLeds array (CPU fan) is on pin 7
	FastLED.addLeds<WS2812B, DATA_PIN9, GRB>(stripLeds,20);		// stripLeds array (front LED strip) is on pin 9
 }

void loop() 
{
	// system timer calls
	systemTimer.UpdateSystemTimer();	// have the system timer object find time passed since last main loop execution
	deltaMillis = systemTimer.GetDeltaMillis();	// deltaMillis is defined in globals for doggos. It is global so that all classes have access to the value for use in timing

	// test functions
	// TestFadeThroughColorsFans(600, prideTransgender);
	// TestSpinColorWave(100, prideTransgender);
	// TestSpinColorWaveFadeFans(100, prideTransgender);

	//RandomTest();

	
	//systemLeds.virtualAspectFan[0].SpinLeds(100,CRGB::HotPink, CRGB::Aquamarine);
	//systemLeds.virtualAspectFan[1].SpinColorWave(-150, prideLesbian);

	//systemLeds.virtualAspectFan[2].FadeThroughColors(2000, pinkBreathing);
	systemLeds.virtualAspectFan[2].SingleLedChase(systemTimer.halfSecTriangle, prideRainbow,0.5);

	systemLeds.virtualDualAspectFans[0].MovingLineDualAspect(100, prideLesbian);
	//systemLeds.virtualDualAspectFans[0].StackFill(100, prideLesbian);

	systemLeds.virtualCPUFan[0].SingleLedChase((-1 * systemTimer.halfSecTriangle),prideRainbow,0.5);

	//systemLeds.virtualLedStrip[0].ChaseWithFade(60,prideTransgender, .6,2);
		
	systemLeds.virtualLedStrip[0].ScrollColorsOnFrontStrips(300, prideTransgender, 1,0,1,0);
	systemLeds.virtualLedStrip[0].ScrollColorsOnFrontStrips(-300, prideLesbian, 0,1,0,1);
	
	//systemLeds.virtualLedStrip[0].SingleLedChase(30, prideRainbow,0.8);
	// I feel like it makes the most intuitive sense to make methods of the systemleds object copy arrays out to hardware-associated arrays instead of
	// using individual fan object methods. It feels more like the system leds device is managing system operations and is separated froom the indvidual
	// hardware objects
	// I think I want to rewrite it that way

	// Below copies two fans to front fans
	//systemLeds.virtualAspectFan[0].CopyToExternalArray(aspect0Leds);
	//systemLeds.virtualAspectFan[1].CopyToExternalArray(aspect1Leds);
	
	// Below copies dual aspect fan to front fans
	systemLeds.virtualDualAspectFans[0].CopyToTwoExternalArrays(aspect1Leds, aspect0Leds);
	
	systemLeds.virtualAspectFan[2].CopyToExternalArray(aspect2Leds);

	systemLeds.virtualCPUFan[0].CopyToExternalArray(cpuFanLeds);

	systemLeds.virtualLedStrip[0].CopyToExternalArray(stripLeds);

	FastLED.show();     
}

void TestFadeThroughColorsFans(int speed, const CRGB* palette)
{
	systemLeds.virtualAspectFan[0].FadeThroughColors(speed, palette);
	systemLeds.virtualAspectFan[1].FadeThroughColors(speed * -1, palette);
	systemLeds.virtualAspectFan[2].FadeThroughColors(speed, palette);
	systemLeds.virtualCPUFan[0].FadeThroughColors(speed, palette);
};

void TestSingleLedChase(int speed, const CRGB* palette)
{
	systemLeds.virtualAspectFan[0].SingleLedChase(speed, palette);
	systemLeds.virtualAspectFan[1].SingleLedChase(speed * -1, palette);
	systemLeds.virtualAspectFan[2].SingleLedChase(speed, palette);
	systemLeds.virtualCPUFan[0].SingleLedChase(speed, palette);
};

void TestSpinColorWaveFadeFans(int speed, const CRGB* palette)
{
	systemLeds.virtualAspectFan[0].SingleLedChase(speed, palette,0.8);
	systemLeds.virtualAspectFan[1].SingleLedChase(speed * -1, palette,0.8);
	systemLeds.virtualAspectFan[2].SingleLedChase(speed, palette,0.8);
	systemLeds.virtualCPUFan[0].SingleLedChase(speed, palette,0.8);
};

void TestSpinLeds(int speed, CRGB color1, CRGB color2, CRGB color3)
{
	systemLeds.virtualAspectFan[0].SpinLeds(speed, color1);
	systemLeds.virtualAspectFan[1].SpinLeds(speed * -1, color1 );
	systemLeds.virtualAspectFan[2].SpinLeds(speed, color1);
	systemLeds.virtualCPUFan[0].SpinLeds(speed, color1);
}

void TestSpinOneLed(int speed, const CRGB* palette)
{
	systemLeds.virtualAspectFan[0].SpinOneLed(speed, palette);
	systemLeds.virtualAspectFan[1].SpinOneLed(speed * -1, palette);
	systemLeds.virtualAspectFan[2].SpinOneLed(speed, palette);
	systemLeds.virtualCPUFan[0].SpinOneLed(speed, palette);
}

void (TestMovingLine(int speed, const CRGB* palette))
{
	systemLeds.virtualAspectFan[0].MovingLine(speed, palette);
	systemLeds.virtualAspectFan[1].MovingLine (speed * -1, palette);
	systemLeds.virtualAspectFan[2].MovingLine (speed, palette);
	systemLeds.virtualCPUFan[0].MovingLine (speed, palette);
}

/*
void RandomTest()
{
	int speed = 100;
	CRGB color;
	int random;
	
	for (int i = 0; i < 4; i ++)  // once per fan
	{
		CRGB color = MakeRandomColor();
		int random = rand()% 2;
		if (random == 0)
			CRGB[] paletteUsed = prideTransgender;
		else if (random == 1)
			CRGB[] paletteUsed = prideLesbian;
		random = rand() % 5;
		if (i == 0)
		{
			if (rand == 0)
				systemLeds.virtualAspectFan[0].FadeThroughColorsFans(speed,paletteUsed);
			else if (rand == 1)
				systemLeds.virtualAspectFan[0].SpinColorWave(speed,paletteUsed);
			else if (rand == 2)
				systemLeds.virtualAspectFan[0].SpinColorWaveFadeFans(speed,paletteUsed);
			else if (rand == 3)
				systemLeds.virtualAspectFan[0].SpinLeds(speed,MakeRandomColor(),MakeRandomColor() );
			else if (rand == 4)
				systemLeds.virtualAspectFan[0].SpinOneLed(speed,paletteUsed);
			else if (rand == 5
				systemLeds.virtualAspectFan[0].MovingLine(speed,paletteUsed);
		}
		else if (i == 1)
		{
			if (rand == 0)
				systemLeds.virtualAspectFan[1].FadeThroughColorsFans(speed,paletteUsed);
			else if (rand == 1)
				systemLeds.virtualAspectFan[1].SpinColorWave(speed,paletteUsed);
			else if (rand == 2)
				systemLeds.virtualAspectFan[1].SpinColorWaveFadeFans(speed,paletteUsed);
			else if (rand == 3)
				systemLeds.virtualAspectFan[1].SpinLeds(speed,MakeRandomColor(),MakeRandomColor() );
			else if (rand == 4)
				systemLeds.virtualAspectFan[1].SpinOneLed(speed,paletteUsed);
			else if (rand == 5
				systemLeds.virtualAspectFan[1].MovingLine(speed,paletteUsed);
		}
		else if (i == 2)
		{
			if (rand == 0)
				systemLeds.virtualAspectFan[2].FadeThroughColorsFans(speed,paletteUsed);
			else if (rand == 1)
				systemLeds.virtualAspectFan[2].SpinColorWave(speed,paletteUsed);
			else if (rand == 2)
				systemLeds.virtualAspectFan[2].SpinColorWaveFadeFans(speed,paletteUsed);
			else if (rand == 3)
				systemLeds.virtualAspectFan[2].SpinLeds(speed,MakeRandomColor(),MakeRandomColor() );
			else if (rand == 4)
				systemLeds.virtualAspectFan[2].SpinOneLed(speed,paletteUsed);
			else if (rand == 5
				systemLeds.virtualAspectFan[2].MovingLine(speed,paletteUsed);
		}
		else if (i == )
		{
			if (rand == 0)
				systemLeds.virtualCPUFan[1].FadeThroughColorsFans(speed,paletteUsed);
			else if (rand == 1)
				systemLeds.virtualCPUFan[1].SpinColorWave(speed,paletteUsed);
			else if (rand == 2)
				systemLeds.virtualCPUFan[1].SpinColorWaveFadeFans(speed,paletteUsed);
			else if (rand == 3)
				systemLeds.virtualCPUFan[1].SpinLeds(speed,MakeRandomColor(),MakeRandomColor() );
			else if (rand == 4)
				systemLeds.virtualCPUFan[1].SpinOneLed(speed,paletteUsed);
			else if (rand == 5
				systemLeds.virtualCPUFan[1].MovingLine(speed,paletteUsed);
		}
	}
}*/

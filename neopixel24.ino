/**************************************************************************
 *                                                                       *
     NeoPixel Ring Clock   
     by Andy Doro (mail@andydoro.com)
     http://andydoro.com/ringclock/

     Modified by Todd Resudek @sprsmpl
     to work with 24 pixel neopixel ring.
     
 *                                                                      *
 **************************************************************************


   Revision History

   Date       By            What
   20160327   supersimple   Modifications for 24 pixel ring
   20140320   AFD           First draft
*/

// include the library code:
#include <Wire.h>
#include <RTClib.h>
#include <Adafruit_NeoPixel.h>

// define pins
#define NEOPIN 12
#define BRIGHTNESS 64 // set max brightness
#define PIXELS 24
#define DSTPIN 13 //Daylight Saving Time Adjust

RTC_DS1307 RTC; // Establish clock object
DateTime Clock; // Holds current clock time

Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXELS, NEOPIN, NEO_GRB + NEO_KHZ800); // strip object

byte hourval, minuteval, secondval; // holds the time

byte pixelColorRed, pixelColorGreen, pixelColorBlue; // holds color values

void setup () {
  Wire.begin();         // Begin I2C
  RTC.begin();   // begin clock
  // set pinmodes
  pinMode(NEOPIN, OUTPUT);
  pinMode(DSTPIN, INPUT);

  if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }

  //RTC.adjust(DateTime(__DATE__, __TIME__));//MANUAL RTC SET

  strip.begin();
  //strip.show(); // Initialize all pixels to 'off'

  strip.setBrightness(BRIGHTNESS); // set brightness
  
  // startup sequence
  delay(500);
  colorWipe(strip.Color(202,217,228), 10); // Red
  colorWipe(strip.Color(10,207,79), 10); // Green
  colorWipe(strip.Color(10,145,207), 10); // Blue
  delay(500);

}

void loop () {

  char* colon = ":"; // static characters save a bit
  char* slash = "/"; // of memory

  // get time
  Clock = RTC.now(); // get the RTC time

  int dstOn = digitalRead(DSTPIN);
  
  // since there are only 24 pixels to represent 60 seconds/minutes
  // each second/minute will only advance every 2.5 units
  secondval = (int) (Clock.second() * 0.4);  // get seconds
  minuteval = (int) (Clock.minute() * 0.4);  // get minutes
  hourval = Clock.hour();   // get hours

  //Adjustment for daylight savings time
  if(dstOn == HIGH){
    dstAdjust();
  }
  
  if (hourval > 11) hourval -= 12; // This clock is 12 hour, if 13-23, convert to 0-11
  
  // light mode
  // Blue: strip.Color(10,145,207)
  // Green: strip.Color(10,207,79)
  // White: strip.Color(202,217,228)

  // dark mode
  // Blue: strip.Color(5,72,103)
  // Green: strip.Color(5,103,39)
  // White: strip.Color(101,108,114)
  
  // arc mode
  for (uint8_t i = 0; i < PIXELS; i++) {
    // reset the pixel
    pixelColorRed = 0;
    pixelColorGreen = 0;
    pixelColorBlue = 0;
      
    if (i <= secondval) {
      pixelColorRed = 2;
      pixelColorGreen = 36;
      pixelColorBlue = 51;
    }
    

    if (i == minuteval) {
      pixelColorRed = 10;
      pixelColorGreen = 207;
      pixelColorBlue = 79;
    }
    
    // the hour marker will alternate pixels 0,2,4,6...
    if (i == (hourval * 2)) {
      pixelColorRed = 202;
      pixelColorGreen = 217;
      pixelColorBlue = 228;
    }
    

    strip.setPixelColor(i, strip.Color(pixelColorRed, pixelColorGreen, pixelColorBlue));
  }

  //display
  strip.show();

  // wait
  delay(100);

}


// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

//turns clock back 1 hour for DST
void dstAdjust(){
  if(hourval == 0){
    //this is midnight, so we need to turn back the hours
    hourval == 23
  }else{
    hourval -= 1
  }
}

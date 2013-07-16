// Copyright (c) 2013 Mark Dyer. All rights reserved.
#include "MultiColorLED.h"

namespace mdlib {
  MultiColorLED::MultiColorLED() 
  {
  }

  void MultiColorLED::setup() const {
    red_.setup();
    green_.setup();
    blue_.setup();
  }

  void MultiColorLED::set_pins(int red_pin, int green_pin, int blue_pin) {
    red_.set_pin(red_pin);
    green_.set_pin(green_pin);
    blue_.set_pin(blue_pin);
  }

  void MultiColorLED::set_color(uint32_t color) const {
    red_.setLevel(  (byte)((color & RED) >> 16));
    green_.setLevel((byte)((color & GREEN) >> 8));
    blue_.setLevel( (byte) (color & BLUE));
  }

  void MultiColorLED::blend_colors(uint32_t c1, uint32_t c2, float pct) const {
    if (pct <= 0.0)
      set_color(c1);
    else if (pct >= 1.0)
      set_color(c2);
    else {
      int r1 = (c1 & RED) >> 16;
      int g1 = (c1 & GREEN) >> 8;
      int b1 = c1 & BLUE;
      int r2 = (c2 & RED) >> 16;
      int g2 = (c2 & GREEN) >> 8;
      int b2 = c2 & BLUE;

      int r = r1 + (int)(pct * (float)(r2 - r1));
      int g = g1 + (int)(pct * (float)(g2 - g1));
      int b = b1 + (int)(pct * (float)(b2 - b1));

      set_color(
		((r & 0xff) << 16) |
		((g & 0xff) << 8) |
		(b & 0xff));
    }
  }

  // FROM
  // http://eduardofv.com/read_post/179-Arduino-RGB-LED-HSV-Color-Wheel-
//Convert a given HSV (Hue Saturation Value) to RGB(Red Green Blue) and set the led to the color
//  h is hue value, integer between 0 and 360
//  s is saturation value, double between 0 and 1
//  v is value, double between 0 and 1
//http://splinter.com.au/blog/?p=29
  void MultiColorLED::set_hsv(int h, float s, float v) {
#if 0
    Serial.print("set_hsv(");
    Serial.print(h);
    Serial.println(")");
#endif
    //this is the algorithm to convert from RGB to HSV
    float r=0; 
    float g=0; 
    float b=0;

    float hf=h/60.0;

    int i=(int)floor(h/60.0);
    float f = h/60.0 - i;
    float pv = v * (1 - s);
    float qv = v * (1 - s*f);
    float tv = v * (1 - s * (1 - f));

    switch (i)
      {
      case 0: //rojo dominante
	r = v;	g = tv;	b = pv;
	break;
      case 1: //verde
	r = qv;	g = v;	b = pv;
	break;
      case 2: 
	r = pv;	g = v;	b = tv;
	break;
      case 3: //azul
	r = pv;	g = qv;	b = v;
	break;
      case 4:
	r = tv;	g = pv;	b = v;
	break;
      case 5: //rojo
	r = v;	g = pv;	b = qv;
	break;
      }

    //set each component to a integer value between 0 and 255
    int red=constrain((int)(255.0*r),0,255);
    int green=constrain((int)(255.0*g),0,255);
    int blue=constrain((int)(255.0*b),0,255);

    set_rgb(red,green,blue);
  }

  void MultiColorLED::set_rgb(int red, int green, int blue) {
    red_.setLevel((byte)red);
    green_.setLevel((byte)green);
    blue_.setLevel((byte)blue);
  }
} // namespace mdlib

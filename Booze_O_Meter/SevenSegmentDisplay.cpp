// Copyright (c) 2013 Mark Dyer. All rights reserved.
#include "Arduino.h"

#include "SevenSegmentDisplay.h"

namespace mdlib {
  void SevenSegmentDisplay::setup() {
    begin(9600);
    //    delay(10);
    clear();
  }

  void SevenSegmentDisplay::update() {
    if (colon_on_ && !show_colon_) {
	write(0x77);
	write((byte)0x00);
	colon_on_ = false;
	return;
    }

    if (show_colon_) {
      bool show = millis()/500 % 2;
      if (show) {
	if(!colon_on_) {
	  write(0x77);
	  write(0x10);
	  colon_on_ = true;
	}
      }
      else {
	if (colon_on_) {
	  write(0x77);
	  write((byte)0x00);
	  colon_on_ = false;
	}
      }
    }
  }
  
  void SevenSegmentDisplay::clear() {
    write(0x76);
    write(0x77); // decimal place command
    write((unsigned char)0x00); // turn them all off
    colon_on_ = show_colon_ = false;
  }

  void SevenSegmentDisplay::Reset() {
    clear();
  }

  void SevenSegmentDisplay::set(int value) {
    show_colon_ = false;
#if 0
    Serial.print("set(");
    Serial.print(value);
    Serial.println(")");
#endif
    if (value > 9999 || value < -999) {
      error();
      return;
    }
    else 
      sprintf(buffer_, "%4d", value);
#if 0
    Serial.print(" - '");
    Serial.print(buffer_);
    Serial.println("'");
#endif
    write(buffer_);

    delay(10);
  }

  void SevenSegmentDisplay::error() {
    clear();
    write("Err");
    delay(10);
  }

  void SevenSegmentDisplay::SetDigitSegments(int digit, uint8_t mask) {
    if (digit < 0 || digit > 3) {
      write("Err0");
      return;
    }
    byte d = 0x7B + digit;
    write(d);
    write(mask);
    //write("dEAd");
  }

  void SevenSegmentDisplay::SetSegment(uint8_t cmd, uint8_t mask) {
    Serial.print(" -- SetSegment(0x");
    Serial.print(cmd, HEX);
    Serial.print(", 0x");
    Serial.print(mask, HEX);
    Serial.println(")");
    write(cmd);
    write(mask);
  }

  void SevenSegmentDisplay::set(float value, int accuracy, bool leading_zeros) {
    show_colon_ = false;
    if (accuracy < 0 && accuracy > 3) {
      error();
      return;
    }
      
    // first set the decimal place
    this->write(0x77); // decimal place command
    delay(10);
    this->write(0x01 << (3 - accuracy)); 
    delay(10);

    float x = 1;
    for(int i = 0; i < accuracy; i++) {
      x = x * 10.0f;
    }

    int iVal = (int)(value * x);

    if (iVal > 9999) {
      error();
      return;
    }

    char format[8];
    sprintf(format, "%%%s4d", leading_zeros ? "0" : "");
    char buffer[8];
    sprintf(buffer, format, iVal);

    if (!leading_zeros && value < 1.00f) {
      for (int i = 3 - accuracy; i < 5; i++) {
	if (buffer[i] != ' ')
	  break;
	buffer[i] = '0';
      }
    }
    
    write(buffer);
    //    delay(10);
  }

  void SevenSegmentDisplay::set(const char* string) {
    show_colon_ = false;
    bool ok = false;
    int i = 0;
    for (i = 0; i < 6; i++) 
      if (string[i] == '\0') {
	ok = true;
	break;
      }

    i--;
    
    if (!ok) {
      error();
      return;
    }
    /*
    while(i < 5) {
      write(' ');
      i++;
    }
    */
    write(string);
  }

  void SevenSegmentDisplay::SetSeconds(unsigned long seconds) {
    show_colon_ = true;
    int hrs = seconds / (60 * 60);
    int min = (seconds - (hrs * 60 * 60)) / 60;
    int sec = seconds - (hrs * 60 * 60) - (min * 60);

#if 0
    Serial.print("SetSeconds(");
    Serial.print(seconds);
    Serial.print(") ");
    Serial.print(hrs);
    Serial.print(":");
    Serial.print(min);
    Serial.print(":");
    Serial.print(sec);
    Serial.println(")");
#endif
    if (hrs > 99 || min > 99 || sec > 99) {
      Serial.print("SetSeconds(");
      Serial.print(seconds);
      Serial.print(") ERROR: hrs: ");
      Serial.print(hrs);
      Serial.print(", min: ");
      Serial.print(min);
      Serial.print(", sec: ");
      Serial.println(sec);
      error();
      return;
    }

    char buffer[64];
    if (hrs)
      sprintf(buffer, "%02d%02d", hrs, min);
    else
      sprintf(buffer, "%02d%02d", min, sec);
#if 0
    Serial.print("Time: ");
    Serial.println(buffer);
#endif
    buffer[4] = '\0';
    write(buffer);
  }
  
  void SevenSegmentDisplay::SetBrightness(float b) {
    int brightness = (int)(254.0 * (1.0 - constrain(b, 0.0, 1.0)));
    
    write(0x7A); // brightness command
    write((byte)brightness);    // data byte
  }
} // namespace mdlib

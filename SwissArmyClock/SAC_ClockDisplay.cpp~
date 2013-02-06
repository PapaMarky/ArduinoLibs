/**
 * The 4 digit seven segment display.
 */
#include <arduino.h>
#include <SwissArmyClock.h>

#include "SAC_ClockDisplay.h"
#include "../SAC_tlc5940/SAC_tlc5940.h"

char* SAC_ClockDisplay::Days[] = {"","Sun","Mon","Tue","Wed","Thu","Fri","Sat"};
char* SAC_ClockDisplay::Months[] = {"","Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};

//extern SAC_Tlc5940 Tlc;

SAC_ClockDisplay::SAC_ClockDisplay(int colon1, int colon2)
  : m24hr(false),
    mLeadingZeros(false),
    mHour(0),
    mMinute(0),
    mBrightness(4000),
    mColon1(colon1),
    mColon2(colon2),
    mNeedUpdate(false)
{
}

SAC_ClockDisplay::~SAC_ClockDisplay()
{
}
const int A = 0;
const int B = 1;
const int C = 2;
const int D = 3;
const int E = 4;
const int F = 5;
const int G = 6;
const int DP = 7;

void SAC_ClockDisplay::setup()
{
  //pinMode(mColonPin, OUTPUT);
  Tlc.init();
}

unsigned long lastMillis = 0;

const char* SAC_ClockDisplay::DateString() {
  char buffer[16] = "123456789012345";
  // 0123456789012345
  // Sun May 20, 2012
  DateTime dt = clock.now();

  if(mClockIsSet) {
    //buffer[0] = Days[dt.dayOfWeek()][0];
    //buffer[1] = Days[dt.dayOfWeek()][1];
    //buffer[2] = Days[dt.dayOfWeek()][2];
    //buffer[3] = '\0';

  }
  else {
    return "DATE NOT SET";
  }

  return &buffer[0];
}

const char* SAC_ClockDisplay::TimeString() {
  char buffer[17];
  if(mClockIsSet) {
    DateTime dt = clock.now();
    // '    HH:MM      '
    int hour = dt.hour();
    char* ampm = "AM";
    if(hour == 0) hour = 12;
    if(hour > 12) {
      hour -= 12;
      ampm = "PM";
    }
  
    sprintf(buffer, "    %2d:%02d %s    ",
	    hour, dt.minute(), ampm);
  }
  else {
    return "TIME NOT SET";
  }
  return buffer;

}

inline bool SAC_ClockDisplay::FirstHalfOfSecond() {
  DateTime dt = clock.now();
  
  return dt.second() % 2; // (millis() % 1000) > 500;
}

void SAC_ClockDisplay::ClearSegments() {
  for(int digit = 0; digit < 4; digit++) {
    for(int segment = 0; segment < 8; segment++) {
      Tlc.set(digit * 8 + segment, 0);
    }
  }
}

void SAC_ClockDisplay::update()
{
  static bool colonOn = false;
  if (mClockIsSet) {

    // manage the colon
    if (FirstHalfOfSecond()) {
      if (!colonOn) {
	//analogWrite(mColonPin, 255);
	Tlc.set(mColon1, mBrightness);
	Tlc.set(mColon2, mBrightness);
	mNeedUpdate = true;
	colonOn = true;
      }
    }
    else {
      if (colonOn) {
	//analogWrite(mColonPin, 0);
	Tlc.set(mColon1, 0);
	Tlc.set(mColon2, 0);
	mNeedUpdate = true;
	colonOn = false;
      }
    }
    DateTime dt = clock.now();
    if(mHour != dt.hour() || mMinute != dt.minute()) {
      mNeedUpdate = true;

      mHour = dt.hour();
      mMinute = dt.minute();

      if(mHour == 0) mHour = 12;
      if(mHour > 12) mHour -= 12;

      int h0 = mHour/10;
      int h1 = mHour - (h0 * 10);
      int m0 = mMinute/10;
      int m1 = mMinute - (m0 * 10);

      //Tlc.clear();
      ClearSegments();
      
      if (mLeadingZeros || h0 > 0) {
	SetDigit(0,h0);
      }
      SetDigit(1,h1);
      SetDigit(2,m0);
      SetDigit(3,m1);
    }
  }
  else {
    if (FirstHalfOfSecond()) {
      if (!colonOn) {
	//Tlc.clear();
	ClearSegments();
	mNeedUpdate = true;
	SetDigit(0,'-');
	SetDigit(1,'-');
	SetDigit(2,'-');
	SetDigit(3,'-');
	//analogWrite(mColonPin, 255);
	Tlc.set(mColon1, mBrightness);
	Tlc.set(mColon2, mBrightness);
	colonOn = true;
      }
    }
    else {
      if (colonOn) {
	//Tlc.clear();
	ClearSegments();
	mNeedUpdate = true;
	SetDigit(0,' ');
	SetDigit(1,' ');
	SetDigit(2,' ');
	SetDigit(3,' ');
	//analogWrite(mColonPin, 0);
	Tlc.set(mColon1, 0);
	Tlc.set(mColon2, 0);
	colonOn = false;
      }
    }
  }
    
  if (mNeedUpdate) {
    Tlc.update();
    mNeedUpdate = false;
  }
}

void SAC_ClockDisplay::SetTimeDate(uint32_t now) {
  if(now != 0) {
    clock.adjust(DateTime(now));
    mClockIsSet = true;
  }
  else {
    mClockIsSet = false;
  }
  mNeedUpdate = true;
}

void SAC_ClockDisplay::SetTime(int h, int m)
{
  Debug("SetTime(");
  Debug(h); Debug(", "); Debug(m);
  DebugLn(")");
  if (h >= 0 && h < 24 && m >= 0 && m < 60) {
      mHour = h;
      mMinute = m;
      mClockIsSet = true;
    }
}

void SAC_ClockDisplay::SetBrightness(int b)
{
  if(b < 0) b = 0;
  if(b >= 4096) b -= 4095;
  mBrightness = b;
  mNeedUpdate = true;
}

void SAC_ClockDisplay::SetDigit(int digit, int value)
{
  //Debug("SetDigit(");
  //Debug(digit);
  //Debug(", "); Debug(value); DebugLn(")");
  char c = ' ';
  if(value >= 0 && value <= 9) {
    c = '0' + value;
  }
  SetDigit(digit, c);
}

void SAC_ClockDisplay::SetSegment(int digit, int segment, int brightness)
{
  
  if(brightness == -1) brightness = mBrightness;
  if(segment >= 0 && segment <= 7) {
    //Debug("OUTPUT ");
    //Debug(8 * digit + segment);
    //Debug(" brightness: ");
    //DebugLn(brightness);
    Tlc.set(8 * digit + segment, brightness);
  }
}
void SAC_ClockDisplay::SetDigit(int digit, char value)
{
  //Debug("SetDigit(");
  //Debug(digit);
  //Debug(", '"); Debug(value); DebugLn("')");
  switch(value) {
  case '#':
    SetSegment(digit, A);
    SetSegment(digit, B);
    SetSegment(digit, C);
    SetSegment(digit, D);
    SetSegment(digit, E);
    SetSegment(digit, F);
    SetSegment(digit, G);
    SetSegment(digit, DP);
    break;
  case '0':
    SetSegment(digit, A);
    SetSegment(digit, B);
    SetSegment(digit, C);
    SetSegment(digit, D);
    SetSegment(digit, E);
    SetSegment(digit, F);
    break;
  case '1':
    SetSegment(digit, B);
    SetSegment(digit, C);
    break;
  case '2':
    SetSegment(digit, A);
    SetSegment(digit, B);
    SetSegment(digit, D);
    SetSegment(digit, E);
    SetSegment(digit, G);
    break;
  case '3':
    SetSegment(digit, A);
    SetSegment(digit, B);
    SetSegment(digit, C);
    SetSegment(digit, D);
    SetSegment(digit, G);
    break;
  case '4':
    SetSegment(digit, B);
    SetSegment(digit, C);
    SetSegment(digit, F);
    SetSegment(digit, G);
    break;
  case '5':
    SetSegment(digit, A);
    SetSegment(digit, C);
    SetSegment(digit, D);
    SetSegment(digit, F);
    SetSegment(digit, G);
    break;
  case '6':
    SetSegment(digit, A);
    SetSegment(digit, C);
    SetSegment(digit, D);
    SetSegment(digit, E);
    SetSegment(digit, F);
    SetSegment(digit, G);
    break;
  case '7':
    SetSegment(digit, A); 
    SetSegment(digit, B);
    SetSegment(digit, C);
   break;
  case '8':
    SetSegment(digit, A);
    SetSegment(digit, B);
    SetSegment(digit, C);
    SetSegment(digit, D);
    SetSegment(digit, E);
    SetSegment(digit, F);
    SetSegment(digit, G);
    break;
  case '9':
    SetSegment(digit, A);
    SetSegment(digit, B);
    SetSegment(digit, C);
    SetSegment(digit, F);
    SetSegment(digit, G);
    break;
  case '.':
    SetSegment(digit, DP);
    break;
  case '-':
    SetSegment(digit, G);
    break;
  case ' ':
    break;
  case 'F':
  default:
    SetSegment(digit, A);
    SetSegment(digit, D);
    SetSegment(digit, E);
    SetSegment(digit, F);
    SetSegment(digit, G);
    break;
  }
}

void SAC_ClockDisplay::SetFloat(float v, bool trailingZeros)
{
  //Tlc.clear();
  ClearSegments();
  int iVal = (int)v; // integer part
  int fVal = 0;
  float frac = v - (float)iVal;

  if(iVal < 10000) {
    // which digit's decimal point do I use?
    int dp_pos = 0;
    if(iVal < 10) {
      dp_pos = 0;
      fVal = (int)(frac * 100.0);
    }
    else if(iVal < 100) {
      dp_pos = 1;
      fVal = (int)(frac * 10.0);
    }
    else if(iVal < 1000) {
      dp_pos = 2;
      fVal = (int)(frac);
    }
    else {
      dp_pos = 3;
    }
    //Debug("I: "); Debug(iVal);
    //Debug(", F: "); Debug(fVal);
    //DebugLn("");
    SetSegment(dp_pos, DP);
    int p = 0;
    char buf[12];
    ltoa(iVal,buf,10);
    while(p < 4 && buf[p] != '\0') {
      SetDigit(p,buf[p]);
      p++;
    }
    int offset = p;
    p = 0;
    ltoa(fVal,buf,10);
    while(p+offset < 4 && buf[p] != '\0') {
      SetDigit(p+offset,buf[p]);
      p++;
    }

    p = p+offset;
    while(p<4) {
      SetDigit(p,trailingZeros?'0':' ');
      p++;
    }

    Tlc.update();
  }
  else { 
    // value too big, what should I do?
  }
}

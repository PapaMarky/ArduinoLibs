#ifndef SAC_CLOCK_DISPLAY_H
#define SAC_CLOCK_DISPLAY_H

#include <RTClib.h>

// SAC_ClockDisplay uses the Tlc5940 library
//
// WIRING FOR TLC5940
//
//  3 - GSCLK (Purple)
//  9 - XLAT  (Yellow)
// 10 - BLANK (Blue)
// 11 - SIN   (Orange)
// 13 - SCLK  (Green)
//
class SAC_ClockDisplay {
 public:
  SAC_ClockDisplay(int colon1, int colon2);
  ~SAC_ClockDisplay();

  void SetTime(int h, int m);
  void SetTimeDate(uint32_t now);
  
  void setup();
  void update();

  void SetBrightness(int b);
  void SetDigit(int digit, char value);
  void SetDigit(int digit, int value);
  void SetSegment(int digit, int segment, int brightness = -1);

  void SetFloat(float v, bool trailingZeros = false);

  uint32_t now() { return (mClockIsSet ? clock.now().unixtime() : 0); }
  bool clockIsSet() { return mClockIsSet; }

  const char* DateString();
  const char* TimeString();

  static char* Days[];
  static char* Months[];

 private:

  void ClearSegments();
  bool FirstHalfOfSecond();
  
  bool m24hr;
  bool mLeadingZeros;
  bool mClockIsSet;
  int mHour;
  int mMinute;
  int mTemperature;
  int mBrightness;
  int mColon1;
  int mColon2;
  bool mNeedUpdate;
  RTC_Millis clock;
};


#endif // SAC_CLOCK_DISPLAY_H

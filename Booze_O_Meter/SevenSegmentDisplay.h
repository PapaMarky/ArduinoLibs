// Copyright (c) 2013 Mark Dyer. All rights reserved.
#ifndef SEVEN_SEGMENT_DISPLAY
#define SEVEN_SEGMENT_DISPLAY

#include "../SoftwareSerial/SoftwareSerial.h"

namespace mdlib {

  class SevenSegmentDisplay : public SoftwareSerial {
  public:
    SevenSegmentDisplay(int rx_pin, int tx_pin)
      : SoftwareSerial(rx_pin, tx_pin),
      show_colon_(false),
      colon_on_(false) {}

    ~SevenSegmentDisplay() {};

    void setup();
    void update();
    void clear();

    void set(int value);
    void set(float value, int decimal_places, bool leading_zeros = false);
    void set(const char* string);

    void SetSeconds(unsigned long seconds);
    
    // brightness: 0.0 (off) to 1.0 (full on)
    void SetBrightness(float brightness);
    void Reset();
    void error();

    void SetDigitSegments(int digit, uint8_t mask);

    void SetSegment(uint8_t cmd, uint8_t mask);
    
  private:
    bool show_colon_;
    bool colon_on_;
    char buffer_[128];
  };
  
} // namespace mdlib

#endif // SEVEN_SEGMENT_DISPLAY

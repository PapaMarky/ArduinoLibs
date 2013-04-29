#ifndef MULTI_COLOR_LED_H
#define MULTI_COLOR_LED_H

#include "../base/base.h"

namespace mdlib {

  const uint32_t RED       = 0x00ff0000;
  const uint32_t YELLOW    = 0x00ffff00;
  const uint32_t GREEN     = 0x0000ff00;
  const uint32_t TURQUOISE = 0x0000ffff;
  const uint32_t BLUE      = 0x000000ff;
  const uint32_t PURPLE    = 0x00ff00ff;
  const uint32_t BLACK     = 0x00000000;
  const uint32_t WHITE     = 0x00ffffff;
  
  class MultiColorLED {
  public:
    
    MultiColorLED();
    void setup() const;

    void set_pins(int red_pin, int green_pin, int blue_pin);
    void set_color(uint32_t color) const;

    void blend_colors(uint32_t c1, uint32_t c2, float pct) const;
    
  private:
    AnalogOutput red_;
    AnalogOutput green_;
    AnalogOutput blue_;
  };
}

#endif // MULTI_COLOR_LED_H

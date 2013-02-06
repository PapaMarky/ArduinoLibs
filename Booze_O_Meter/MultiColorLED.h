#ifndef MULTI_COLOR_LED_H
#define MULTI_COLOR_LED_H

namespace mdlib {
  class MultiColorLED {
  public:
    MultiColorLED(int red_pin, int green_pin, int blue_pin);

    void set_color(uint32_t color);

  private:
    AnalogOutput red_;
    AnalogOutput green_;
    AnalogOutput blue_;
  };
}

#endif // MULTI_COLOR_LED_H

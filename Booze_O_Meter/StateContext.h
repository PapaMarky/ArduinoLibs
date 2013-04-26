// Copyright (c) 2013 Mark Dyer. All rights reserved.
#ifndef STATE_CONTEXT_
#define STATE_CONTEXT_

class SoftwareSerial;
class mdlib::DigitalOutput;
class mdlib::MultiColorLED;

namespace BOM {

class BoozeSensor;

class StateContext {
 public:
  StateContext() {}

  void set_display(SoftwareSerial* display) { display_ = display; }
  void set_fan(mdlib::DigitalOutput* fan) { fan_ = fan; }
  void set_rgb_led(mdlib::MultiColorLED* rgb_led) { rgb_led_ = rgb_led; }
  void set_sensor(BoozeSensor* sensor) { sensor_ = sensor; }

  mdlib::DigitalOutput* fan() const {return fan_;}
  SoftwareSerial* display() const {return display_;}
  mdlib::MultiColorLED* led() const {return rgb_led_;}
  BoozeSensor* sensor() const {return sensor_;}

 private:
  // context items (things the states need to manipulate)
  // buttons do not belong here, because they generate events
  // that are handled by the States' handle_event(...) overloads
  
  // TODO all of these should be BOM specific wrapper classes.
  // Maybe not fan_, but certainly the others
  mdlib::DigitalOutput* fan_;
  SoftwareSerial* display_;
  mdlib::MultiColorLED* rgb_led_;
  BoozeSensor* sensor_;
};

} // namespace BOM

#endif // STATE_CONTEXT_

#include "Booze_O_Meter.h"
#include "EventQueue.h"

////////////////////////////////////////////////////////////
// BoozeSensor
////////////////////////////////////////////////////////////
BoozeSensor::BoozeSensor()
{
}

void BoozeSensor::set_pins(int control, int data, int temperature) {
  control_.set_pin(control);
  data_.set_pin(data);
  thermistor_.set_pin(temperature);
}

////////////////////////////////////////////////////////////
// Booze_O_Meter
////////////////////////////////////////////////////////////
Booze_O_Meter::Booze_O_Meter(int display_rx, int display_tx)
  : standalone_(true),
    display_(display_tx, display_rx),
    state_(POWER_ON)
{
}

Booze_O_Meter::~Booze_O_Meter() {}

void Booze_O_Meter::setup() {
  Serial.println('Booze_O_Meter setup');
  fan_.setup();
  sensor_.setup();

  button_states_[0] = button_states_[1] = button_states_[2] = false;
  display_.begin(9600);
  delay(10);
  display_.write("v"); // 0x76); // clear

  rgb_led_.setup();
  rgb_led_.set_color(mdlib::BLACK);
}

void Booze_O_Meter::HandleEvents() {
  while (mdlib::CountEvents() > 0) {
    mdlib::Event e = mdlib::HandleEvent();

    switch (e.event_type) {
    case mdlib::Event::BUTTON_UP:
      break;
    case mdlib::Event::BUTTON_DOWN:
      break;
    }
  }
}

void Booze_O_Meter::loop() {
  // Manage Events
  HandleEvents();
  
  switch(state_) {
  case POWER_ON:
    power_on_loop();
    break;
  case CALIBRATION:
    calibration_loop();
    break;
  }
}

void Booze_O_Meter::power_on_loop() {
  standalone_ = i2c_jumper_.getState();
  state_ = CALIBRATION;
  display_.write(" OFF");
}

void Booze_O_Meter::calibration_loop() {
  bool changed = false;

  static bool showData = true; // otherwise show temperature

  main_button_.update();
  up_button_.update();
  down_button_.update();

  fan_.set_state(up_button_.isPressed());

  if (main_button_.isPressed() != button_states_[0]) {
    button_states_[0] = main_button_.isPressed();
    if (!main_button_.isPressed()) {
      changed = true;

      if (sensor_.isOn())
	sensor_.turnOff();
      else
	sensor_.turnOn();
    }
  }

  if (up_button_.isPressed() != button_states_[1]) {
    button_states_[1] = up_button_.isPressed();
    if (!up_button_.isPressed()) {
      changed = true;
    }
  }

  if (down_button_.isPressed() != button_states_[2]) {
    button_states_[2] = down_button_.isPressed();
    if (!down_button_.isPressed()) {
      changed = true;
      showData = !showData;
    }
  }

  if (!changed && ! sensor_.isOn())
    return;
  
  display_.write(0x76); // clear

  int value = -1;
  if (sensor_.isOn()) {
    if (showData) 
      value = sensor_.read() * -1;
    else
      value = sensor_.getTemperature();
  }
  else {
    display_.write(" OFF");
    return;
  }

  if (value > 9999) {
    display_.write("Err");
    return;
  }
  else {
    char data[5];
    sprintf(data, "%4d", value);
  
    int len = strlen(data);

    if (len < 4) display_.write(' ');
    if (len < 3) display_.write(' ');
    if (len < 2) display_.write(' ');

    display_.write(data);
  }
}

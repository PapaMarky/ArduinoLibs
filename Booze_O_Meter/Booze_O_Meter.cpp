#include "Booze_O_Meter.h"

////////////////////////////////////////////////////////////
// BoozeSensor
////////////////////////////////////////////////////////////
BoozeSensor::BoozeSensor(int control_pin, int data_pin, int temperature_pin)
  : control_(control_pin),
    data_(data_pin),
    thermistor_(temperature_pin)
{
  control_.turnOff();
}

////////////////////////////////////////////////////////////
// Booze_O_Meter
////////////////////////////////////////////////////////////
Booze_O_Meter::Booze_O_Meter(int fan_pin,
			     int i2c_jumper_pin,
			     int disp_rx_pin, int disp_tx_pin,
			     int main_button_pin,
			     int up_button_pin,
			     int down_button_pin,
			     int sensor_control_pin,
			     int sensor_data_pin,
			     int sensor_temperature_pin)
  : standalone_(true),
    fan_(fan_pin),
    i2c_jumper_(i2c_jumper_pin),
    display_(disp_rx_pin, disp_tx_pin),
    main_button_(main_button_pin),
    up_button_(up_button_pin),
    down_button_(down_button_pin),
    sensor_(sensor_control_pin, sensor_data_pin, sensor_temperature_pin),
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
  display_.write('v'); // clear the display
  
}

void Booze_O_Meter::loop() {
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
  static int count = 0;
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
      count = 0;

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
      count++;
    }
  }

  if (down_button_.isPressed() != button_states_[2]) {
    button_states_[2] = down_button_.isPressed();
    if (!down_button_.isPressed()) {
      changed = true;
      showData = !showData;
      count--;
    }
  }

  if (!changed && ! sensor_.isOn())
    return;
  
  display_.write(0x76); // clear

  int value = count;
  if (sensor_.isOn()) {
    if (showData) 
      value = sensor_.read();
    else
      value = sensor_.getTemperature();
  }
  else {
    display_.write(" OFF");
    return;
  }
  if (value > 9999)
    display_.write("Err");
  else {
    char data[5];
    itoa(value, data, 10);
  
    int len = strlen(data);

    if (len < 4) display_.write(' ');
    if (len < 3) display_.write(' ');
    if (len < 2) display_.write(' ');

    display_.write(data);
  }
}

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

int BoozeSensor::getTemperature() {
  int raw = thermistor_.read();

  float pad = 9850;
  
  float Temp;  // Dual-Purpose variable to save space.

  long Resistance=((1024 * pad / raw) - pad); 
  Temp = log(Resistance); // Saving the Log(resistance) so not to calculate  it 4 times later
  Temp = 1 / (0.001129148 + (0.000234125 * Temp) + (0.0000000876741 * Temp * Temp * Temp));
  Temp = Temp - 273.15;  // Convert Kelvin to Celsius                      
  //temp = (Temp * 9.0)/ 5.0 + 32.0;                  // Convert to Fahrenheit
  return (int)(Temp * 10.0);                                      // Return the Temperature
}

// observed values
const float alcohol_sensor_zero = 300;
const float alcohol_sensor_100 = 1023;

float BoozeSensor::CalculateAlcoholPercent() {
  float sensor_value = (float)data_.read();

  if (sensor_value <= alcohol_sensor_zero)
    return 0.0;

  if (sensor_value >= alcohol_sensor_100)
    return 100.0;

  return ((sensor_value - alcohol_sensor_zero) / (alcohol_sensor_100 - alcohol_sensor_zero)) * 100.0;
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
  display_.write('v'); // 0x76); // clear
  display_.write('w'); display_.write((uint8_t)0x00);

  rgb_led_.setup();
  rgb_led_.set_color(mdlib::BLACK);

  set_state(POWER_ON);
  
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

  unsigned long elapsed = millis() - state_start_millis_;

  if (elapsed >= 1000) {
    display_.write(" OFF");
    set_state(CALIBRATION);
    return;
  }

  if (elapsed < 1000/6) {
    rgb_led_.set_color(mdlib::PURPLE);
  }
  else if (elapsed < 2000/6) {
    rgb_led_.set_color(mdlib::RED);
  }
  else if (elapsed < 3000/6) {
    rgb_led_.set_color(mdlib::YELLOW);
  }
  else if (elapsed < 4000/6) {
    rgb_led_.set_color(mdlib::GREEN);
  }
  else if (elapsed < 5000/6) {
    rgb_led_.set_color(mdlib::BLUE);
  }
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
  
  display_.write('v'); // clear
  display_.write('w'); display_.write((uint8_t)0x00);

  int value = -1;
  char data[50];
  unsigned long elapsed = millis() - state_start_millis_;

  static int status = 0;

  float status0_limit = (status > 0) ? 0.0 : 5.0;
  float status1_limit = (status > 1) ? 20.0 : 25.0;
  float status2_limit = (status > 2) ? 85.0 : 95.0;
  if (sensor_.isOn()) {
    float pct = sensor_.CalculateAlcoholPercent();
    if (pct <= status0_limit) {
      rgb_led_.set_color(0x000a0a0a);
      status = 0;
    }
    else if (pct <= status1_limit) {
      rgb_led_.set_color(mdlib::GREEN);
      status = 1;
    }
    else if (pct <= status2_limit) {
      rgb_led_.set_color(mdlib::YELLOW);
      status = 2;
    }
    else {
      rgb_led_.set_color(mdlib::RED);
      status = 3;
    }
      
    if (showData) {
      bool showPct = true; // (elapsed/1000) % 2;

      if (showPct) {
	value = (int)(pct * 10.0);
	if (value == 0) {
	  sprintf(data, "  00");
	}
	else {
	  sprintf(data, "%4d", value);
	}
	display_.write(data);
	display_.write('w');
	display_.write(0x4);
      }
      else {
	value = sensor_.RawAlcoholValue();
	sprintf(data, "%4d", value);
	display_.write(data);
      }
      return;
    }
    else {
      value = sensor_.getTemperature();
      sprintf(data, "%3dC", value);
      display_.write(data);
      display_.write('w');
      display_.write(0x2);
    }
  }
  else {
    display_.write(" OFF");
    rgb_led_.set_color(mdlib::BLUE);
    return;
  }

  if (value > 9999) {
    display_.write("Err");
    return;
  }
  else {
  
    int len = strlen(data);

    if (len < 4) display_.write(' ');
    if (len < 3) display_.write(' ');
    if (len < 2) display_.write(' ');

    display_.write(data);
  }
}

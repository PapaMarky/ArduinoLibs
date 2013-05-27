// Copyright (c) 2013 Mark Dyer. All rights reserved.
#include "Booze_O_Meter.h"
#include "StateMachine.h"
#include "EventQueue.h"
////////////////////////////////////////////////////////////
// BoozeSensor
////////////////////////////////////////////////////////////

namespace BOM {

StartUpState Booze_O_Meter::START_UP;
WarmUpState Booze_O_Meter::WARM_UP;
ReadyState Booze_O_Meter::READY;
SamplingState Booze_O_Meter::SAMPLING;
PostSampleState Booze_O_Meter::POST_SAMPLE;
PostSample2State Booze_O_Meter::POST_SAMPLE2;
SleepState Booze_O_Meter::SLEEP;
PowerSaverState Booze_O_Meter::POWER_SAVER;
////////////////////////////////////////////////////////////
// Booze_O_Meter
////////////////////////////////////////////////////////////
Booze_O_Meter::Booze_O_Meter()
{
  // set up the states
  // - set each one's next_state and timeout_next_state
  START_UP.set_next_state(&WARM_UP);
  START_UP.set_timeout_next_state(&WARM_UP);

  WARM_UP.set_next_state(&READY);
  WARM_UP.set_timeout_next_state(&READY);
  
  READY.set_next_state(&SAMPLING);
  READY.set_timeout_next_state(&POWER_SAVER);

  SAMPLING.set_next_state(&POST_SAMPLE);
  SAMPLING.set_timeout_next_state(&POST_SAMPLE);

  POST_SAMPLE.set_next_state(&WARM_UP);
  POST_SAMPLE.set_timeout_next_state(&POST_SAMPLE2);

  POST_SAMPLE2.set_next_state(&WARM_UP);
  POST_SAMPLE2.set_timeout_next_state(&WARM_UP);

  SLEEP.set_next_state(&WARM_UP);
  SLEEP.set_timeout_next_state(&WARM_UP);

  POWER_SAVER.set_next_state(&SAMPLING);
  POWER_SAVER.set_timeout_next_state(&SLEEP);
}

Booze_O_Meter::~Booze_O_Meter() {}

void Booze_O_Meter::setup() {
  button_states_[0] = button_states_[1] = button_states_[2] = false;

  set_state(&START_UP);
}

  
void Booze_O_Meter::set_context(StateContext* context) {
  context_ = context;
  State::set_context(context);
}

void Booze_O_Meter::loop() {
  // Manage Events
  while (mdlib::CountEvents() > 0) {
    mdlib::Event e = mdlib::HandleEvent();

    State* next_state = state_->handle_event(e);
    if (next_state)
      set_state(next_state);
  }

  State* next_state = state_->loop();
  if (next_state)
    set_state(next_state);
}
  /*
void Booze_O_Meter::power_on_loop() {
  standalone_ = i2c_jumper_.getState();

  unsigned long elapsed = millis() - state_start_millis_;

  if (elapsed >= 1000) {
    context_.display()->write(" OFF");
    set_state(SETUP);
    return;
  }

  if (elapsed < 1000/6) {
    context_.led()->set_color(mdlib::PURPLE);
  }
  else if (elapsed < 2000/6) {
    context_.led()->set_color(mdlib::RED);
  }
  else if (elapsed < 3000/6) {
    context_.led()->set_color(mdlib::YELLOW);
  }
  else if (elapsed < 4000/6) {
    context_.led()->set_color(mdlib::GREEN);
  }
  else if (elapsed < 5000/6) {
    context_.led()->set_color(mdlib::BLUE);
  }
}


void Booze_O_Meter::calibration_loop() {
  bool changed = false;

  static bool showData = true; // otherwise show temperature

  main_button_.update();
  up_button_.update();
  down_button_.update();

  context_.fan()->set_state(up_button_.isPressed());

  if (main_button_.isPressed() != button_states_[0]) {
    button_states_[0] = main_button_.isPressed();
    if (!main_button_.isPressed()) {
      changed = true;

      if (context_.sensor()->isOn())
	context_.sensor()->turnOff();
      else
	context_.sensor()->turnOn();
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

  if (!changed && ! context_.sensor()->isOn())
    return;
  
  context_.display()->write('v'); // clear
  context_.display()->write('w'); context_.display()->write((uint8_t)0x00);

  int value = -1;
  char data[50];
  unsigned long elapsed = millis() - state_start_millis_;

  static int status = 0;

  float status0_limit = (status > 0) ? 0.0 : 5.0;
  float status1_limit = (status > 1) ? 20.0 : 25.0;
  float status2_limit = (status > 2) ? 85.0 : 95.0;
  if (context_.sensor()->isOn()) {
    float pct = context_.sensor()->CalculateAlcoholPercent();
    if (pct <= status0_limit) {
      context_.led()->set_color(0x000a0a0a);
      status = 0;
    }
    else if (pct <= status1_limit) {
      context_.led()->set_color(mdlib::GREEN);
      status = 1;
    }
    else if (pct <= status2_limit) {
      context_.led()->set_color(mdlib::YELLOW);
      status = 2;
    }
    else {
      context_.led()->set_color(mdlib::RED);
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
	context_.display()->write(data);
	context_.display()->write('w');
	context_.display()->write(0x4);
      }
      else {
	value = context_.sensor()->RawAlcoholValue();
	sprintf(data, "%4d", value);
	context_.display()->write(data);
      }
      return;
    }
    else {
      value = context_.sensor()->getTemperature();
      sprintf(data, "%3dC", value);
      context_.display()->write(data);
      context_.display()->write('w');
      context_.display()->write(0x2);
    }
  }
  else {
    context_.display()->write(" OFF");
    context_.led()->set_color(mdlib::BLUE);
    return;
  }

  if (value > 9999) {
    context_.display()->write("Err");
    return;
  }
  else {
  
    int len = strlen(data);

    if (len < 4) context_.display()->write(' ');
    if (len < 3) context_.display()->write(' ');
    if (len < 2) context_.display()->write(' ');

    context_.display()->write(data);
  }
}
  */
}

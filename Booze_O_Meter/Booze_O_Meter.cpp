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
PostSample2State Booze_O_Meter::POST_SAMPLE_2;
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

  POST_SAMPLE.set_next_state(&POST_SAMPLE_2);
  POST_SAMPLE.set_timeout_next_state(&POST_SAMPLE_2);

  POST_SAMPLE_2.set_next_state(&WARM_UP);
  POST_SAMPLE_2.set_timeout_next_state(&WARM_UP);

  SLEEP.set_next_state(&START_UP);
  SLEEP.set_timeout_next_state(&START_UP);

  POWER_SAVER.set_next_state(&WARM_UP);
  POWER_SAVER.set_timeout_next_state(&SLEEP);
}

Booze_O_Meter::~Booze_O_Meter() {}

void Booze_O_Meter::setup() {
  context_->setup();
  set_state(&START_UP);
}

  
void Booze_O_Meter::set_context(StateContext* context) {
  context_ = context;
  State::set_context(context);
}

void Booze_O_Meter::update() {
  context_->update();

  // Manage Events
  while (mdlib::CountEvents() > 0) {
    //Serial.println("HANDLING EVENT");
    mdlib::Event e = mdlib::HandleEvent();

    State* next_state = state_->handle_event(e);
    if (next_state)
      set_state(next_state);
  }

  State* next_state = state_->loop();
  if (next_state)
    set_state(next_state);
}
}

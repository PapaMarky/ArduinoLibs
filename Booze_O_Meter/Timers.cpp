// Copyright (c) 2013 Mark Dyer. All rights reserved.

#include <Arduino.h>
#include "Timers.h"
#include "EventQueue.h"

namespace mdlib {

  CountdownTimer::CountdownTimer() {
  }

  CountdownTimer::~CountdownTimer() {
  }

  void CountdownTimer::Start(unsigned long duration) {
    is_running_ = true;
    start_millis_ = millis();
    duration_ = duration;
  }

  void CountdownTimer::Stop() {
    is_running_ = false;
  }

  void CountdownTimer::update() {
    // TODO handle overflow
    if (is_running_) {
      if (millis() - start_millis_ >= duration_) {
	Stop();
	PostEvent(Event(Event::TIMER_DONE, (int)this));
      }
    }
  }
} // namespace mdlib

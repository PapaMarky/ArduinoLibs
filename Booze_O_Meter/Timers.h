// Copyright (c) 2013 Mark Dyer. All rights reserved.
#ifndef TIMERS_H
#define TIMERS_H

namespace mdlib {
  // fires Event::TIMER_DONE after a specified duration of time
  // expires. When the event is fired, 
  class CountdownTimer {
  public:
    CountdownTimer();
    ~CountdownTimer();

    // Start the timer.
    // duration_millis - amount of time until to wait before event is fired.
    void Start(unsigned long duration_in_millis);

    // If called while the timer is running, it stops the timer
    // and no event will be fired.
    void Stop();

    // update the internal state of the timer. Should be called at the
    // beginning of each loop()
    void update();

    bool is_running() { return is_running_; }

    float GetSecondsRemaining() {
      float remaining = 0.0;
      if (is_running_) {
	unsigned long elapsed = millis() - start_millis_;
	remaining = (float)(duration_ - elapsed)/1000.0;
      }
      return remaining;
    }
  private:
    bool is_running_;
    unsigned long start_millis_;
    unsigned long duration_;
  };
}
#endif // TIMERS_H

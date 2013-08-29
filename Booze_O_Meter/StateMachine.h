// Copyright (c) 2013 Mark Dyer. All rights reserved.
#ifndef STATE_MACHINE_H__
#define STATE_MACHINE_H__

#include "EventQueue.h"
#include "Timers.h"

namespace BOM {
class StateContext;

class State {
 public:
  State() {};
  virtual ~State() {}

  static void set_context(StateContext* context) { s_context = context; }

  virtual void enter_state();
  virtual void leave_state() {};

  virtual State* loop() = 0;
  virtual State* handle_event(mdlib::Event e) = 0;

  void set_next_state(State* next_state) {next_state_ = next_state;}
  void set_timeout_next_state(State* timeout_next_state) {timeout_next_state_ = timeout_next_state;}

  virtual const char* name() const = 0;

 protected:
  static StateContext* s_context;
  unsigned long start_time_;
  State* next_state_;
  State* timeout_next_state_;

  void SetStartTime() { start_time_ = millis(); }
};

class TimedState : public State {
 public:
 TimedState() : timeout_(5*60*1000) {}
  virtual ~TimedState() {}

  virtual void enter_state() { State::enter_state(); StartTimer(); }
  virtual void leave_state() { StopTimer(); }

 protected:
  void StartTimer() { timer_.Start(timeout_); }
  void StopTimer() { timer_.Stop(); }
  void SetTimeout(unsigned long t) { timeout_ = t; }
  void UpdateTimer() { timer_.update(); }

 protected:
  mdlib::CountdownTimer timer_;

 private:
  unsigned long timeout_;
};
 
class StartUpState : public State {
 public:
  StartUpState() {}

  virtual void enter_state();
  virtual void leave_state();

  virtual State* loop();
  virtual State* handle_event(mdlib::Event e);

  virtual ~StartUpState() {}

  virtual const char* name() const { return "StartUpState"; }
 private:
  static const unsigned int DURATION = 8000;
  // 0 : 0x7B : digit 1 segments ( 0 -  7)
  // 1 : 0x7C : digit 2 segments ( 8 - 15)
  // 2 : 0x7D : digit 3 segments (16 - 23)
  // 3 : 0x7E : digit 4 segments (24 - 31)
  // 4 : 0x77 : decimal point segments (32 - 37)
  uint8_t segments[5];

  // 'n' is between 0 and 37 inclusive
  // returns 'true' if segment is set successfully
  bool TryToSetSegment(int n);
};

class WarmUpState : public State {
 public:
 WarmUpState() : display_mode_(GRAPHIC) {}
  ~WarmUpState() {}

  virtual void enter_state();
  virtual void leave_state();

  virtual State* loop();
  virtual State* handle_event(mdlib::Event e);

  virtual const char* name() const { return "WarmUpState"; }

 private:
  void RotateDisplayMode();
  void UpdateGraphicDisplay();
  
  enum DisplayMode {
    GRAPHIC,
    ALCOHOL_STDDEV,
    THERMISTOR_STDDEV,
    ALCOHOL_VALUE,
    THERMISTOR_VALUE
  };

  DisplayMode display_mode_;

  // BoozeSensor::DataTrend
  int alcohol_trend_;
  int temperature_trend_;
 
  float display_value_;
  
  unsigned long pulse_start_;

  void SetDataStableSize(float s);
  double data_stable_size_;
};

class ReadyState : public TimedState {
 public:
  ReadyState() { SetTimeout(3L * 60L * 1000L); }
  ~ReadyState() {}
  
  virtual void enter_state();
  virtual void leave_state();

  virtual State* loop();
  virtual State* handle_event(mdlib::Event e);

  virtual const char* name() const { return "ReadyState"; }
 private:

};

class SamplingState : public TimedState {
 public:
  SamplingState() {
    // go to PostSampleState after 10 seconds
    SetTimeout(10L*1000L);
  }
  ~SamplingState() {}

  virtual void enter_state();
  virtual void leave_state();

  virtual State* loop();
  virtual State* handle_event(mdlib::Event e);

  virtual const char* name() const { return "SamplingState"; }
 private:
  void UpdateDisplay();
  int start_sample_;
};

class PostSampleState : public TimedState {
 public:
  PostSampleState() { SetTimeout(60L*1000L); }
  ~PostSampleState() {}

  virtual void enter_state();
  virtual void leave_state();

  virtual State* loop();
  virtual State* handle_event(mdlib::Event e);

  virtual const char* name() const { return "PostSampleState"; }
 private:
};

class PostSample2State : public TimedState {
 public:
  PostSample2State() { SetTimeout(60L*1000L); }
  ~PostSample2State() {}

  virtual void enter_state();
  virtual void leave_state();

  virtual State* loop() { return 0;}
  virtual State* handle_event(mdlib::Event e);

  virtual const char* name() const { return "PostSampleState 2"; }
 private:
};

class SleepState : public State {
 public:
  SleepState() {}
  ~SleepState() {}

  virtual void enter_state();

  virtual State* loop();
  virtual State* handle_event(mdlib::Event e);

  virtual const char* name() const { return "SleepState"; }
 private:
};

class PowerSaverState : public TimedState {
 public:
  PowerSaverState() { SetTimeout(20L * 60L * 1000L); }
  ~PowerSaverState() {}

  virtual void enter_state();
  virtual void leave_state();
  
  virtual State* loop();
  virtual State* handle_event(mdlib::Event e);

  virtual const char* name() const { return "PowerSaverState"; }
 private:
};
}

#endif // STATE_MACHINE_H__

// Copyright (c) 2013 Mark Dyer. All rights reserved.
#ifndef STATE_MACHINE_H__
#define STATE_MACHINE_H__

#include "EventQueue.h"

namespace BOM {
class State {
 public:
  State() {};
  virtual ~State() {}

  virtual void enter_state() {};
  virtual void leave_state() {};

  virtual State* loop() = 0;
  virtual State* handle_event(mdlib::Event e) = 0;

  void set_next_state(State* next_state) {next_state_ = next_state;}
  void set_timeout_next_state(State* timeout_next_state) {timeout_next_state_ = timeout_next_state;}

 private:
  State* next_state_;
  State* timeout_next_state_;
};

class StartUpState : public State {
 public:
  StartUpState() {}

  virtual State* loop();
  virtual State* handle_event(mdlib::Event e);

  virtual ~StartUpState() {}
 private:
};

class WarmUpState : public State {
 public:
  WarmUpState() {}
  ~WarmUpState() {}

  virtual State* loop() { return 0;}
  virtual State* handle_event(mdlib::Event e) {return 0;}
 private:
};

class ReadyState : public State {
 public:
  ReadyState() {}
  ~ReadyState() {}
  
  virtual State* loop() { return 0;}
  virtual State* handle_event(mdlib::Event e) {return 0;}
 private:
};

class SamplingState : public State {
 public:
  SamplingState() {}
  ~SamplingState() {}

  virtual State* loop() { return 0;}
  virtual State* handle_event(mdlib::Event e) { return 0;}
 private:
};

class PostSampleState : public State {
 public:
  PostSampleState() {}
  ~PostSampleState() {}

  virtual State* loop() { return 0;}
  virtual State* handle_event(mdlib::Event e) { return 0;}
 private:
};

class PostSample2State : public State {
 public:
  PostSample2State() {}
  ~PostSample2State() {}

  virtual State* loop() { return 0;}
  virtual State* handle_event(mdlib::Event e) { return 0;}
 private:
};

class SleepState : public State {
 public:
  SleepState() {}
  ~SleepState() {}

  virtual State* loop() { return 0;}
  virtual State* handle_event(mdlib::Event e) { return 0;}
 private:
};

class PowerSaverState : public State {
 public:
  PowerSaverState() {}
  ~PowerSaverState() {}

  virtual State* loop() { return 0;}
  virtual State* handle_event(mdlib::Event e) { return 0;}
 private:
};
}

#endif // STATE_MACHINE_H__

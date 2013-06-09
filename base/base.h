#ifndef BASE_H
#define BASE_H

#include <arduino.h>

/**
 * Basic classes to simplify arduino projects.
 */

namespace mdlib {

  class BaseIO {
  public:
    BaseIO() { }

    virtual void setup() const = 0;
    virtual void update() {};

    virtual ~BaseIO() {}

    // set_pin() must be called BEFORE calling setup()
    void set_pin(int pin) { pin_ = pin; }
    int pin() const {return pin_;}
    
  private:
    int pin_;
  };
  
  class DigitalInput : public BaseIO {
  public:
    DigitalInput()
      : BaseIO(),
      pullup_(false)
    {
    }

    virtual ~DigitalInput() {}

    bool getState() const { return (digitalRead(pin()) == HIGH); }

    virtual void setup() const {
      pinMode(pin(), pullup_ ? INPUT : INPUT_PULLUP);
    }

  private:
    bool pullup_;
  
  };

  class DigitalOutput : public BaseIO {
  public:
    DigitalOutput()
      : BaseIO()
    {
    }

    virtual ~DigitalOutput() { }

    
    virtual void setup() const {
      pinMode(pin(), OUTPUT);
    }

    void set_state(bool state) {
      if(state_ != state) {
	state_ = state;
	digitalWrite(pin(), state ? HIGH : LOW);
      }
    }

    void TurnOn()  { set_state(true); }
    void TurnOff() { set_state(false); }

    bool IsOn() { return state_; }

    bool getState() const { return state_; }
    
  private:
    bool state_;
  };

  class AnalogInput : public BaseIO {
  public:
    AnalogInput(bool use_external_aref = false): BaseIO() {
      use_external_aref_ = use_external_aref;
    }

    void set_use_external_aref(bool use) { use_external_aref_ = use; }

    virtual void setup() const {
      // NOTE: there is also an INTERNAL mode, which is different
      // http://arduino.cc/en/Reference/AnalogReference
      analogReference( use_external_aref_ ? EXTERNAL : DEFAULT );
    }

    // returns 0 - 1023
    // It takes about 100 microseconds (0.0001 s) to read an analog input,
    // so the maximum reading rate is about 10,000 times a second.
    int read() const {
      return analogRead(pin());
    }
  private:
    bool use_external_aref_;
  };

  // PWM
  class AnalogOutput : public BaseIO {
  public:
    AnalogOutput() : BaseIO() {}

    virtual void setup() const { }

    void setLevel(byte level) const {
      analogWrite(pin(), level);
    }
    
    // level is clamped to [0.0 - 1.0]
    void setLevel(float level) const {
      if (level < 0.0) level = 0.0;
      if (level > 1.0) level = 1.0;

      setLevel((byte)(level * 255.0));
    }
  };

  // MomentaryButton wiring:
  //
  //               /  
  //  pin  -------/  ------- 5V
  //          |
  //          ---/\/\/\/---- GND
  //              10K R
  //
  // update() is what sets the state of the button. update should be called
  // before the state of the button is queried.
  // 
  class MomentaryButton : public DigitalInput {
  public:
  MomentaryButton() : is_pressed_(false) {}

    virtual void update() {
      is_pressed_ = debounce();
    }

    bool isPressed() const {return is_pressed_;}

  private:
    bool is_pressed_;
    bool debounce() const {
      const int debounceDelay = 10;
      bool state;
      bool previousState;
      previousState = getState();
    
      for (int counter = 0; counter < debounceDelay; counter++) {
	delay(1);
	state = getState();
	if (state != previousState) {
	  counter = 0;
	  previousState = state;
	}
      }
    
      return (state == HIGH);
    }
  };
}

#endif // BASE_H

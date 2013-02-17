#ifndef BUTTON_H
#define BUTTON_H

namespace mdlib {

  // Button wiring:
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
  class Button : public DigitalInput {
  public:
  Button() : is_pressed_(false) {}

    virtual void update() {
      is_pressed_ = debounce();
    }

    bool isPressed() {return is_pressed_;}

  private:
    bool is_pressed_;
    bool debounce() {
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

#endif // BUTTON_H

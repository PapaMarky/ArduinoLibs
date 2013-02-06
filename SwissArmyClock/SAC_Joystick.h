#ifndef SAC_JOYSTICK
#define SAC_JOYSTICK

typedef void(*jsCallback)();

class SAC_Joystick {
 public:
  SAC_Joystick(int (*reader)(int), int xChannel, int yChannel, int selPin);

  void setup();

  void RegisterOnSelect(jsCallback onSelect) {mOnSelect = onSelect;}
  void RegisterOnLeft(jsCallback onLeft) {mOnLeft = onLeft;}
  void RegisterOnRight(jsCallback onRight) {mOnRight = onRight;}
  void RegisterOnUp(jsCallback onUp) {mOnUp = onUp;}
  void RegisterOnDown(jsCallback onDown) {mOnDown = onDown;}

  void update();

  bool ReadSelect();

  int ReadX();
  int ReadY();
  
 private:
  int (*mReaderFunc)(int);

  jsCallback mOnSelect;
  jsCallback mOnLeft;
  jsCallback mOnRight;
  jsCallback mOnUp;
  jsCallback mOnDown;

  int mXChannel;
  int mYChannel;
  int mSelPin;
  
  bool mSelect;
  bool mLeft;
  bool mRight;
  bool mUp;
  bool mDown;
};


#endif // SAC_JOYSTICK

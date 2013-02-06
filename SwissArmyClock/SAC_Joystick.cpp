#include <arduino.h>
#include "SAC_Joystick.h"
#include "../SwissArmyClock/SwissArmyClock.h"

SAC_Joystick::SAC_Joystick(int (*reader)(int), int xChannel, int yChannel, int selPin)
  : mReaderFunc(reader),
    mXChannel(xChannel),
    mYChannel(yChannel),
    mSelPin(selPin),
    mSelect(false), mLeft(false), mRight(false), mUp(false), mDown(false)
{
}

void SAC_Joystick::setup() {
  pinMode(mSelPin, INPUT);
  digitalWrite(mSelPin, HIGH);
  
  Debug("SAC_Joystick(reader, ");
  Debug(mXChannel); Debug(", ");
  Debug(mYChannel); Debug(", ");
  DebugLn(mSelPin); Debug(")");
}

void SAC_Joystick::update() {
  bool sel = ReadSelect();
  if (sel != mSelect) {
    mSelect = sel;
    if (sel && mOnSelect) (*mOnSelect)();
  }

  int X = ReadX();

  // Use two limit values to prevent spurious events.
  const int LIMIT_ON = 100;
  const int LIMIT_OFF = 200;
  bool left = mLeft ? (X < LIMIT_OFF) : (X < LIMIT_ON);
  if (left != mLeft) {
    mLeft = left;
    if (left && mOnLeft) (*mOnLeft)();
  }

  bool right = mRight ? (X > 1023 - LIMIT_OFF) : (X > 1023 - LIMIT_ON);
  if (right != mRight) {
    mRight = right;
    if (right && mOnRight) (*mOnRight)();
  }

  int Y = ReadY();
  bool up = mUp ? (Y < LIMIT_OFF) : (Y < LIMIT_ON);
  if (up != mUp) {
    mUp = up;
    if (up && mOnUp) (*mOnUp)();
  }

  bool down = mDown ? (Y > 1023 - LIMIT_OFF) : (Y > 1023 - LIMIT_ON);
  if (down != mDown) {
    mDown = down;
    if (down && mOnDown) (*mOnDown)();
  }
}

int SAC_Joystick::ReadX() {
  return (*mReaderFunc)(mXChannel);
}

int SAC_Joystick::ReadY() {
  return (*mReaderFunc)(mYChannel);
}

bool SAC_Joystick::ReadSelect() {
  return digitalRead(mSelPin) != HIGH;
}

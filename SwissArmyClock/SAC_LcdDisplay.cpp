#include <Arduino.h>

#include "../LiquidCrystal_SR3W/LiquidCrystal_SR3W.h"
#include "../SAC_Tlc5940/SAC_Tlc5940.h"

#include "LcdDisplayMode.h"
#include "SAC_LcdDisplay.h"

const byte RR[] = {255, 175,   0,   0,   0, 150, 150};
const byte GG[] = {  0, 255, 250, 250,   0,   0, 255};
const byte BB[] = {  0,   0,   0, 250, 250, 250, 200};
 
enum colors { RED=0, YELLOW=1, GREEN=2, TURQUOISE=3, BLUE=4, MAGENTA=5, WHITE=6 };

// XXX Must match enum SAC_LcdDisplay::Mode
const int SAC_LcdDisplay::nModes = 5;

SAC_LcdDisplay::SAC_LcdDisplay(int data, int clock, int latch, int r, int g, int b, SAC_ClockDisplay* clockDisplay)
  : mRed(r), mGreen(g), mBlue(b)
{
  mLcd = new LiquidCrystal_SR3W(data, clock, latch);

  // build the list of modes
  // mMode = new LcdDisplayModeEntry[nModes];
  mMode = (LcdDisplayModeEntry*)malloc(sizeof(LcdDisplayModeEntry) * nModes);

  mCurrentMode = BANNER;
  
  // BANNER
  mMode[BANNER].mode = new BannerMode(mLcd);
  mMode[BANNER].next = TIME;
  mMode[BANNER].color = MAGENTA;
  mMode[BANNER].modified = true;
  // TIME
  mMode[TIME].mode = new TimeMode(mLcd, clockDisplay);
  mMode[TIME].next = TEMP;
  mMode[TIME].color = GREEN;
  mMode[TIME].modified = true;
  // TEMP
  mMode[TEMP].mode = new TemperatureMode(mLcd);
  mMode[TEMP].next = BANNER;
  mMode[TEMP].color = YELLOW;
  mMode[TEMP].modified = true;
  // VOLTAGES
  mMode[VOLTAGES].mode = new PressureMode(mLcd);
  mMode[VOLTAGES].next = BANNER;
  mMode[VOLTAGES].color = RED;
  mMode[VOLTAGES].modified = true;
  // PRESSURE
}

void SAC_LcdDisplay::setup() {
  mLcd->begin(16, 2);
  StartMode(BANNER);
}

void SAC_LcdDisplay::SetTemperature(short temperature) {
  ((TemperatureMode*)mMode[TEMP].mode)->Set(temperature);
}

void SAC_LcdDisplay::Update() {
  if (mMode[mCurrentMode].mode->Update()) {
    StartMode(mMode[mCurrentMode].next);
  }
}

void SAC_LcdDisplay::StartMode(Mode m) {
  mCurrentMode = m;
  SetColor(mMode[mCurrentMode].color);
  mMode[mCurrentMode].mode->Start();
  mMode[mCurrentMode].mode->Update();
}

void SAC_LcdDisplay::SetColor(byte r, byte g, byte b) {
  uint16_t R = 4095 * r;
  uint16_t G = 4095 * g;
  uint16_t B = 4095 * b;
   
  Tlc.set(mRed, R);
  Tlc.set(mGreen, G);
  Tlc.set(mBlue, B);
   
  while(Tlc.update());
}

void SAC_LcdDisplay::SetColor(int c) {
  SetColor(RR[c], GG[c], BB[c]);
}

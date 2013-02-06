// Bits and pieces for the Swiss Army Clock
#include <Arduino.h>

// Debug Printing
//#define DEBUG 1

#if defined(DEBUG)
#define Debug Serial.print
#define DebugLn Serial.println
#else
#define Debug //
#define DebugLn //
#endif

const int display_address = 4;
// Display Commands

const byte SET_LED_MODE = 0x01;  // Set the LED Display Mode - (byte) mode
const byte SET_TIME = 0x02;      // Set Time - (uint32_t) time
const byte SET_TEMP = 0x03;      // Set the temperature 
const byte SET_PRESSURE = 0x04;  // Set the barometric pressure
const byte SET_BRIGHTNESS = 0x05;// Set the display brightness
const byte CHECK_TIMESET = 0x06; // See if the time needs setting 
const byte SET_VOLTAGES = 0x07;  // Set the voltages

// Display Status Codes
const byte STARTUP = 0x01;
const byte TIMESET = 0x02;

// LED Display Modes

// Time mode
const byte TIME_MODE = 0x01; // Just time
const byte BANK_MODE = 0x02; // rotate: time / degrees F / degrees C

// Copyright (c) 2013 Mark Dyer. All rights reserved.
#include "StateContext.h"

#include "../base/base.h"
#include "MultiColorLED.h"
#include "SevenSegmentDisplay.h"
#include "BoozeSensor.h"
#include "LightedButton.h"

namespace BOM {

  void StateContext::setup() {
    fan()->setup();
    display()->setup();
    led()->setup();
    sensor()->setup();
    button()->setup();
  }

  void StateContext::update() {
    fan()->update();
    display()->update();
    led()->update();
    sensor()->update();
    button()->update();
  }
} // namespace BOM


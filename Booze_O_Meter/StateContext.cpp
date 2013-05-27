// Copyright (c) 2013 Mark Dyer. All rights reserved.
#include "StateContext.h"

#include "../base/base.h"
#include "MultiColorLED.h"

namespace BOM {
  void StateContext::update() {
    if (fan_)
      fan_->update();
  }
} // namespace BOM


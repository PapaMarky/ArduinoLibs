// Copyright (c) 2013 Mark Dyer. All rights reserved.

#include "StateMachine.h"

namespace BOM {
////////////////////////////////////////////////////////////////////
//      StartUpState
////////////////////////////////////////////////////////////////////
  
State* StartUpState::loop() {
  return (State*)0;
}

State* StartUpState::handle_event(mdlib::Event e) {
  return (State*)0;
}
    


} // namespace BOM

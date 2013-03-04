#include "Arduino.h"
#include "EventQueue.h"
#include "Queue.h"

namespace mdlib {
  const int EVENT_QUEUE_LENGTH = 50;
  static Event EVENT_Q_DATA[EVENT_QUEUE_LENGTH];
  
  static Queue<Event> EventQueue(EVENT_Q_DATA, EVENT_QUEUE_LENGTH);

  int CountEvents() {
    return EventQueue.length();
  }

  const char* event_name(Event e) {
    switch(e.event_type) {
    case Event::BUTTON_UP:
      return "BUTTON_UP";
      break;
    case Event::BUTTON_DOWN:
      return "BUTTON_DOWN";
      break;
    case Event::BUTTON_CLICK:
      return "BUTTON_CLICK";
      break;
    case Event::BUTTON_DOUBLE_CLICK:
      return "BUTTON_DOUBLE_CLICK";
      break;
    case Event::BUTTON_LONG_CLICK:
      return "BUTTON_LONG_CLICK";
      break;
    case Event::TIMER_DONE:
      return "TIMER_DONE";
      break;
    default:
      return "*** UNKNOWN EVENT TYPE ***";
      break;
    }
  }
  void PostEvent(Event e) {
    Serial.print("--POST: e: ");
    Serial.print(event_name(e));
    Serial.print(", id: ");
    Serial.println(e.source_id);
    EventQueue.AddToBack(e);
  }

  Event HandleEvent() {
    Event e = EventQueue.TakeFromFront();
    Serial.print("HANDLE: e: ");
    Serial.print(event_name(e));
    Serial.print(", id: ");
    Serial.println(e.source_id);
    return e;
  }
}

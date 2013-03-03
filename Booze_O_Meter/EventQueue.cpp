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
  
  void PostEvent(Event e) {
    Serial.print("--POST: e: ");
    Serial.print(e.event_type);
    Serial.print(", id: ");
    Serial.println(e.source_id);
    EventQueue.AddToBack(e);
  }

  Event HandleEvent() {
    Event e = EventQueue.TakeFromFront();
    Serial.print("HANDLE: e: ");
    Serial.print(e.event_type);
    Serial.print(", id: ");
    Serial.println(e.source_id);
    return e;
  }
}

#include "EventQueue.h"
#include "Queue.h"

namespace mdlib {
  const int EVENT_QUEUE_LENGTH = 50;
  static Event EVENT_Q_DATA[EVENT_QUEUE_LENGTH];
  
  static Queue<Event> EventQueue(EVENT_Q_DATA, EVENT_QUEUE_LENGTH);

  void PostEvent(Event e) {
    EventQueue.AddToBack(e);
  }

  Event HandleEvent() {
    return EventQueue.TakeFromFront();
  }
}

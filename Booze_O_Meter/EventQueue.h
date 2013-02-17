#ifndef EVENT_QUEUE_H
#define EVENT_QUEUE_H

namespace mdlib {
  enum EventType {
    BUTTON_UP,
    BUTTON_DOWN,
    BUTTON_CLICK,
    BUTTON_DOUBLE_CLICK,
    BUTTON_LONG_CLICK,
    TIMER_DONE
  };
  
  class Event {
  public:
  Event() : event_type(0), source_id(0) {}
  Event(int type, int source) :
    event_type(type), source_id(source) {}
    
    int event_type;
    int source_id;
  };

  void PostEvent(Event e);
  Event HandleEvent();
}


#endif // EVENT_QUEUE_H

  

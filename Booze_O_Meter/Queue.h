// Copyright (c) 2013 Mark Dyer. All rights reserved.

#ifndef MDLIB_QUEUE_H
#define MDLIB_QUEUE_H

namespace mdlib {
  template<class T>
    class Queue {
  public:
    Queue();
    ~Queue();
    
    void set_size(int size);
    void AddToBack(T* item);
    T* TakeFromFront();

    int count();
    
  private:
    void Grow();

    T** data_;
    int size_;
    int front_;
    int back_;
  };
}

#endif // MDLIB_QUEUE_H

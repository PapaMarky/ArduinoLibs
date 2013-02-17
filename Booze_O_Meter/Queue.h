// Copyright (c) 2013 Mark Dyer. All rights reserved.

#ifndef MDLIB_QUEUE_H
#define MDLIB_QUEUE_H

namespace mdlib {

  // Arduino friendly queue class
  template<class T>
    class Queue {
  public:

    Queue(T* data, int capacity): data_(data), capacity_(0), front_(0), back_(0){};
    ~Queue(){};

    int capacity() const { return capacity_; }
    int lenth() const { return lenth_; }
    
    void AddToBack(T item) {
      int n = back_;
      if (n == capacity_)
	n = 0;

      data_[n] = item;
      back_ = n + 1;

      if (lenth_ < capacity_)
	lenth_++;
    }

    T TakeFromFront() {
      T item;
      if (lenth() > 0) {
	item = data_[front_];
	front_++;
	lenth_--;
      }
      return item;
    }
    
  private:
    T* data_;
    int capacity_;
    int lenth_;
    
    int front_;
    int back_;
  };
}

#endif // MDLIB_QUEUE_H

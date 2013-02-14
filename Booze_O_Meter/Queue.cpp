// Copyright (c) 2013 Mark Dyer. All rights reserved.

#include "Queue.h"

namespace mdlib {

  template <class T>
  Queue<T>::Queue()
    : data_(0), size_(0), front_(0), back_(0)
  {
    set_size(20);
  }

  template <class T>
  void Queue<T>::set_size(int size) {
    if (size > size_) {
      T** d = new T*[size];
      for (int i = 0; i < size; i++)
	d[i] = (T*)0;
      
      int n = 0;
      if (back_ != front_) {
	if (front_ < back_) {
	  for (int i = front_; i < back_; i++) 
	    d[n++] = data_[i];

	  front_ = 0;
	  back_ = n + 1;
	}
	else {
	  for (int i = front_; i < size_; i++) 
	    d[n++] = data_[i];

	  for (int i = 0; i < back_; i++)
	    d[n++] = data_[i];
	}
      }
      delete [] data_;
      front_ = 0;
      back_ = n;
      data_ = d;
      size_ = size;
    }
  }

  template <class T>
  void Queue<T>::AddToBack(T* item) {
    int n = back_;
    if (n == size_)
      n = 0;

    if (n == front_) {
      Grow();
      n = back_;
    }

    data_[n] = item;
    back_ = n + 1;
  }

  template <class T>
  T* Queue<T>::TakeFromFront() {
    T* item = (T*)0;
    if (count() > 0) {
      item = data_[front_];
      data_[front_] = (T*)0;
      front_++;
    }
    return item;
  }

  template <class T>
  int Queue<T>::count() {

    if (front_ < back_)
      return back_ - front_;
    else
      return (size_ - front_) + back_;
  }

  template <class T>
  void Queue<T>::Grow() {
    set_size(2 * size_);
  }
}


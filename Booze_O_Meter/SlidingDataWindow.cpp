#include <Arduino.h>
#include "SlidingDataWindow.h"

namespace mdlib {

SlidingDataWindow::SlidingDataWindow()
  : window_size_(POOL_SIZE),
    stable_size_(0.01f)
{
}

SlidingDataWindow::~SlidingDataWindow() {
}

void SlidingDataWindow::SetStableSize(float size) {
  stable_size_ = size;
}

void SlidingDataWindow::Reset() {
  next_ = count_ = 0;
}

void SlidingDataWindow::Dump() {
  char buffer[256];
  Serial.print("MIN: ");
  Serial.print(minimum_, 2);
  Serial.print(", MAX: ");
  Serial.print(maximum_, 2);
  Serial.print(", AVE: ");
  Serial.print(average_, 2);
  Serial.print(", MED: ");
  Serial.print(Median(), 2);
  Serial.print(", STD DEV: ");
  Serial.print(standard_deviation_);
  if (IsStable()) {
    Serial.print(" -- (");
    Serial.print(stable_size_);
    Serial.print(") -- STABLE");
  }
  Serial.println("");
}

void SlidingDataWindow::AddSample(float sample) {
  data_[next_] = sample;
  next_++;
  if (next_ >= window_size_)
    next_ = 0;

  if (count_ < window_size_)
    count_++;

  if (IsReady()) {
    float total = 0.0;
    float variance = 0.0;
    float *d = data_;
    minimum_ = maximum_ = *d;
    for (int i = 0; i < window_size_; i++) {
      float dd = *d;
      if (dd < minimum_) {
	minimum_ = dd;
      }
      if (data_[i] > maximum_)
	maximum_ = dd;

      total += dd;

      d++;
    }
    average_ = total/(float)window_size_;
    for (int i = 0; i < window_size_; i++) {
      int v = data_[i] - average_;
      variance += (v*v);
    }
    standard_deviation_ = sqrt(variance/(float)window_size_);
  }
}

bool SlidingDataWindow::IsReady() const {
  return (count_ >= window_size_);
}

bool SlidingDataWindow::IsRising() const {
  // TODO: write me
  return false;
}

bool SlidingDataWindow::IsFalling() const {
  // TODO: write me
  return false;
}

bool SlidingDataWindow::IsStable() const {
  return (standard_deviation_ < stable_size_);
}

float SlidingDataWindow::Minimum() const {
  return minimum_;
}

float SlidingDataWindow::Maximum()  const {
  return maximum_;
}

float SlidingDataWindow::Average()  const {
  return average_;
}

float SlidingDataWindow::Median()  const {
  return minimum_ + (maximum_ - minimum_) / 2.0f;
}

float SlidingDataWindow::StandardDeviation()  const {
  // TODO: write me
  return 0.0f;
}


} // namespace mdlib

